/**
 * File:   camera_device_v4l2.h
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

#ifndef TK_CAMERA_DEVICE_V4L2_H
#define TK_CAMERA_DEVICE_V4L2_H

#include "tkc/object.h"
#include "base/bitmap.h"
#include <linux/videodev2.h>

#include "camera_device.h"
#include "camera_decoder.h"
#include "camera_decoder_factory.h"

BEGIN_C_DECLS


/**
 * @class camera_device_v4l2_t
 * @parent camera_device_t
 * @annotation ["fake"]
 *
 * video4linux实现的相机。
 */

/**
 * @method camera_device_v4l2_create
 *
 * 创建camera对象。
 *
 * @annotation ["constructor"]
 * @param {const char*} device 设备名称。
 * @param {camera_image_format_t} format 图像格式。
 * @param {uint32_t} w 图像宽度。
 * @param {uint32_t} h 图像高度。
 *
 * @return {object_t*} 返回object对象。
 *
 */
object_t* camera_device_v4l2_create(const char* device, camera_image_format_t format, uint32_t w,
                                   uint32_t h);
END_C_DECLS

#endif /*TK_CAMERA_DEVICE_V4L2_H*/
