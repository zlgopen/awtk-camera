/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** V4l2Capture.cpp
** 
** V4L2 wrapper 
**
** -------------------------------------------------------------------------*/


// libv4l2
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <string.h>

// project
#include "logger.h"
#include "V4l2Capture.h"
#include "V4l2MmapDevice.h"
#include "V4l2ReadWriteDevice.h"


static void list_formats(int fd);
// -----------------------------------------
//    create video capture interface
// -----------------------------------------
V4l2Capture* V4l2Capture::create(const V4L2DeviceParameters & param, IoType iotype)
{
	V4l2Capture* videoCapture = NULL;
	V4l2Device* videoDevice = NULL; 
	int caps = V4L2_CAP_VIDEO_CAPTURE;
	switch (iotype)
	{
		case IOTYPE_MMAP: 
			videoDevice = new V4l2MmapDevice(param, V4L2_BUF_TYPE_VIDEO_CAPTURE); 
			caps |= V4L2_CAP_STREAMING;
		break;
		case IOTYPE_READWRITE:
			videoDevice = new V4l2ReadWriteDevice(param, V4L2_BUF_TYPE_VIDEO_CAPTURE); 
			caps |= V4L2_CAP_READWRITE;
		break;
	}
	
	if (videoDevice &&  !videoDevice->init(caps))
	{
		delete videoDevice;
		videoDevice=NULL; 
	}
	
	if (videoDevice)
	{
		videoCapture = new V4l2Capture(videoDevice);
	}	

	list_formats(videoDevice->getFd());

	return videoCapture;
}

// -----------------------------------------
//    constructor
// -----------------------------------------
V4l2Capture::V4l2Capture(V4l2Device* device) : V4l2Access(device)
{
}

// -----------------------------------------
//    destructor
// -----------------------------------------
V4l2Capture::~V4l2Capture() 
{
}

// -----------------------------------------
//    check readability
// -----------------------------------------
int V4l2Capture::isReadable(timeval* tv)
{
	int fd = m_device->getFd();
	fd_set fdset;
	FD_ZERO(&fdset);	
	FD_SET(fd, &fdset);
	return select(fd+1, &fdset, NULL, NULL, tv);
}

// -----------------------------------------
//    read from V4l2Device
// -----------------------------------------
size_t V4l2Capture::read(char* buffer, size_t bufferSize)
{
	return m_device->readInternal(buffer, bufferSize);
}

static void list_framesizes(int fd, uint32_t pixelformat)
{
    struct v4l2_frmsizeenum vfse = { .pixel_format = pixelformat };

    while(!ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &vfse)) {
        switch (vfse.type) {
        case V4L2_FRMSIZE_TYPE_DISCRETE:
            printf(" %ux%u",
                   vfse.discrete.width, vfse.discrete.height);
        break;
        case V4L2_FRMSIZE_TYPE_CONTINUOUS:
        case V4L2_FRMSIZE_TYPE_STEPWISE:
            printf(" {%u-%u, %u}x{%u-%u, %u}",
                   vfse.stepwise.min_width,
                   vfse.stepwise.max_width,
                   vfse.stepwise.step_width,
                   vfse.stepwise.min_height,
                   vfse.stepwise.max_height,
                   vfse.stepwise.step_height);
        }
        vfse.index++;
    }
}

static void list_formats(int fd)
{
    struct v4l2_fmtdesc vfd = { .type = V4L2_BUF_TYPE_VIDEO_CAPTURE };

		memset(&vfd, 0x00, sizeof(vfd));
		vfd.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    while(!ioctl(fd, VIDIOC_ENUM_FMT, &vfd)) {
        list_framesizes(fd, vfd.pixelformat);
        printf("%d: %d %s\n", vfd.index, vfd.pixelformat, vfd.description);
        vfd.index++;
    }
}
	
