/**
 * File:   camera_device_v4l.c
 * Author: AWTK Develop Team
 * Brief:  video4linux implemented camera device
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-09-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/types.h>
#include <linux/videodev2.h>

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/time_now.h"
#include "v4l2/camera_device_v4l2.h"

#include "logger.h"
#include "V4l2Device.h"
#include "V4l2Capture.h"
#include "V4l2Output.h"

static camera_device_t* camera_device_cast(object_t* obj);

#define CAMERA_DEVICE(obj) camera_device_cast(obj)

typedef struct _camera_info_t {
  /*props*/
  uint32_t w;
  uint32_t h;
  uint32_t format;
	bitmap_format_t desire_format;

  /*cameraate*/
  char* buffer;
  uint32_t buffer_size;
  V4l2Capture* capture;
  camera_decoder_t* decoder;
} camera_info_t;

static camera_info_t* camera_info_create(V4l2Capture* capture) {
  camera_info_t* info = TKMEM_ZALLOC(camera_info_t);
  return_value_if_fail(info != NULL, NULL);

  info->capture = capture;
	info->desire_format = BITMAP_FMT_RGBA8888;

  return info;
}

static ret_t camera_info_destroy(camera_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  if (info->capture != NULL) {
    delete info->capture;
  }

  if (info->decoder != NULL) {
    camera_decoder_destroy(info->decoder);
  }

  TKMEM_FREE(info->buffer);

  memset(info, 0x00, sizeof(camera_info_t));
  TKMEM_FREE(info);

  return RET_OK;
}

static ret_t camera_device_on_destroy(object_t* obj) {
  camera_device_t* o = CAMERA_DEVICE(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  camera_info_destroy((camera_info_t*)(o->priv));

  return RET_OK;
}

static int32_t camera_device_compare(object_t* obj, object_t* other) {
  return tk_str_cmp(obj->name, other->name);
}

static uint32_t format_to_v4l2(camera_image_format_t format) {
  switch (format) {
    case CAMERA_IMAGE_YUYV:
      return V4L2_PIX_FMT_YUYV;
    case CAMERA_IMAGE_MJPEG:
      return V4L2_PIX_FMT_MJPEG;
    case CAMERA_IMAGE_RGB565:
      return V4L2_PIX_FMT_RGB565;
    default:
      return CAMERA_IMAGE_YUYV;
  }
}

static camera_image_format_t format_from_v4l2(uint32_t format) {
  switch (format) {
    case V4L2_PIX_FMT_YUYV:
      return CAMERA_IMAGE_YUYV;
    case V4L2_PIX_FMT_MJPEG:
      return CAMERA_IMAGE_MJPEG;
    case V4L2_PIX_FMT_RGB565:
      return CAMERA_IMAGE_RGB565;
    default:
      return CAMERA_IMAGE_NONE;
  }
}

static ret_t camera_device_v4l2_set_prop(object_t* obj, const char* name, const value_t* v) {
  camera_info_t* info = NULL;
  ret_t ret = RET_OK;
  camera_device_t* o = CAMERA_DEVICE(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  info = (camera_info_t*)(o->priv);

  if (tk_str_eq(name, CAMERA_PROP_WIDTH)) {
    info->w = value_int(v);
  } else if (tk_str_eq(name, CAMERA_PROP_HEIGHT)) {
    info->h = value_int(v);
  } else if (tk_str_eq(name, CAMERA_PROP_FORMAT)) {
    uint32_t format = value_uint32(v);
    info->format = format_to_v4l2((camera_image_format_t)format);
  } else if (tk_str_eq(name, CAMERA_PROP_DESIRE_FORMAT)) {
    info->desire_format = (bitmap_format_t)value_uint32(v);
  } else {
    ret = RET_NOT_FOUND;
  }

  return ret;
}

static ret_t camera_device_v4l2_get_prop(object_t* obj, const char* name, value_t* v) {
  camera_info_t* info = NULL;
  ret_t ret = RET_OK;
  camera_device_t* o = CAMERA_DEVICE(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  info = (camera_info_t*)(o->priv);

  if (tk_str_eq(name, CAMERA_PROP_WIDTH)) {
    value_set_uint32(v, info->w);
  } else if (tk_str_eq(name, CAMERA_PROP_HEIGHT)) {
    value_set_uint32(v, info->h);
  } else if (tk_str_eq(name, CAMERA_PROP_FORMAT)) {
    value_set_uint32(v, info->format);
  } else if (tk_str_eq(name, CAMERA_PROP_DESIRE_FORMAT)) {
    value_set_uint32(v, info->desire_format);
  } else {
    ret = RET_NOT_FOUND;
  }

  return ret;
}

static ret_t camera_device_v4l2_exec(object_t* obj, const char* name, const char* args) {
  camera_info_t* info = NULL;
  ret_t ret = RET_OK;
  camera_device_t* o = CAMERA_DEVICE(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  info = (camera_info_t*)(o->priv);

  if (tk_str_eq(name, CAMERA_CMD_APPLY)) {
    info->capture->setFormat(info->format, info->w, info->h);
  } else {
    ret = RET_NOT_FOUND;
  }

  return ret;
}

static const object_vtable_t s_camera_device_vtable = {.type = "camera_device",
                                                       .desc = "camera_device",
                                                       .size = sizeof(camera_device_t),
                                                       .is_collection = FALSE,
                                                       .on_destroy = camera_device_on_destroy,
                                                       .compare = camera_device_compare,
                                                       .get_prop = camera_device_v4l2_get_prop,
                                                       .set_prop = camera_device_v4l2_set_prop,
                                                       .exec = camera_device_v4l2_exec};

static ret_t camera_device_v4l2_ensure_buffer(object_t* obj, uint32_t size) {
  camera_info_t* info = NULL;
  camera_device_t* o = CAMERA_DEVICE(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  info = (camera_info_t*)(o->priv);
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  if (info->buffer_size >= size) {
    return RET_OK;
  }

  if (info->buffer_size < size) {
    if (info->buffer != NULL) {
      TKMEM_FREE(info->buffer);
    }

    info->buffer_size = 0;
    info->buffer = (char*)TKMEM_ALLOC(size + 1);

    if (info->buffer != NULL) {
      info->buffer_size = size;
    }
  }

  return info->buffer != NULL ? RET_OK : RET_OOM;
}

static camera_decoder_t* camera_device_v4l2_get_decoder(object_t* obj, uint32_t format) {
  camera_info_t* info = NULL;
  camera_device_t* o = CAMERA_DEVICE(obj);
  return_value_if_fail(o != NULL, NULL);

  info = (camera_info_t*)(o->priv);
  return_value_if_fail(info != NULL, NULL);

  if (info->decoder != NULL && info->decoder->format == format) {
    return info->decoder;
  }

  if (info->decoder != NULL) {
    camera_decoder_destroy(info->decoder);
    info->decoder = NULL;
  }

  info->decoder = camera_decoder_factory_create_decoder(format_from_v4l2(format));

  return info->decoder;
}

static bitmap_t* camera_device_v4l2_take_picture_impl(object_t* obj) {
  bitmap_t* bitmap = NULL;
  camera_info_t* info = NULL;
  camera_device_t* o = CAMERA_DEVICE(obj);
  return_value_if_fail(o != NULL, NULL);

  info = (camera_info_t*)(o->priv);
  return_value_if_fail(info != NULL, NULL);

  if (info->capture != NULL) {
    timeval tv;
    uint64_t start = time_now_ms();
    V4l2Capture* capture = info->capture;

    tv.tv_sec = 1;
    tv.tv_usec = 0;
    return_value_if_fail(camera_device_v4l2_ensure_buffer(obj, capture->getBufferSize()) == RET_OK,
                         NULL);

    if (capture->isReadable(&tv) == 1) {
      int rsize = 0;
      log_debug("wait for ready cost: %d\n", (int)(time_now_ms() - start));

      start = time_now_ms();
      rsize = capture->read(info->buffer, info->buffer_size);
      log_debug("read cost: %d\n", (int)(time_now_ms() - start));

      if (rsize >= 0) {
        uint32_t format = capture->getFormat();
        camera_decoder_t* decoder = camera_device_v4l2_get_decoder(obj, format);
        return_value_if_fail(decoder != NULL, NULL);

        start = time_now_ms();
        camera_decoder_set_output_format(decoder, info->desire_format);
        bitmap = camera_decoder_decode(decoder, info->buffer, rsize, info->w, info->h);
        log_debug("decode cost: %d\n", (int)(time_now_ms() - start));
      }
    }
  }

  return bitmap;
}

static bitmap_t* camera_device_v4l2_take_picture(object_t* obj) {
  uint64_t start = time_now_ms();
  bitmap_t* image = camera_device_v4l2_take_picture_impl(obj);
  log_debug("take picture cost: %d\n", (int)(time_now_ms() - start));

  return image;
}

static ret_t camera_device_open(camera_device_t* o, const char* device,
                                camera_image_format_t format, uint32_t w, uint32_t h) {
  bool_t verbose = TRUE;
  camera_info_t* info = NULL;
  V4l2Access::IoType ioTypeIn = V4l2Access::IOTYPE_MMAP;
  V4L2DeviceParameters param(device, format_to_v4l2(format), w, h, 0, verbose);
  V4l2Capture* capture = V4l2Capture::create(param, ioTypeIn);
  return_value_if_fail(capture != NULL, RET_BAD_PARAMS);

  info = camera_info_create(capture);
  assert(info != NULL);

  info->w = w;
  info->h = h;
  info->format = format;
  o->priv = info;

  return RET_OK;
}

object_t* camera_device_v4l2_create(const char* device, camera_image_format_t format, uint32_t w,
                                   uint32_t h) {
  object_t* obj = object_create(&s_camera_device_vtable);
  camera_device_t* o = CAMERA_DEVICE(obj);
  return_value_if_fail(o != NULL, NULL);

  initLogger(TRUE);
  o->take_picture = camera_device_v4l2_take_picture;

  if (camera_device_open(o, device, format, w, h) != RET_OK) {
    OBJECT_UNREF(obj);
  }

  return obj;
}

static camera_device_t* camera_device_cast(object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_camera_device_vtable, NULL);

  return (camera_device_t*)obj;
}
