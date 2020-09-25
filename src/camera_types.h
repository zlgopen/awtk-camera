/**
 * File:   camera_device.h
 * Author: AWTK Develop Team
 * Brief:  camera device
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

#ifndef TK_CAMERA_TYPES_H
#define TK_CAMERA_TYPES_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @enum camera_image_format_t
 * @annotation ["scriptable"]
 * 相机图片格式常量定义。
 */
typedef enum _camera_image_format_t {
  /**
   * @const CAMERA_IMAGE_NONE
   * 无效格式
   */
  CAMERA_IMAGE_NONE = 0,
  /**
   * @const CAMERA_IMAGE_YUYV
   * YUYV
   */
  CAMERA_IMAGE_YUYV,
  /**
   * @const CAMERA_IMAGE_MJPEG
   * MJPEG
   */
  CAMERA_IMAGE_MJPEG,
  /**
   * @const CAMERA_IMAGE_RGB565
   * RGB565
   */
  CAMERA_IMAGE_RGB565,
  /**
   * @const CAMERA_IMAGE_BGR565
   * BGR565
   */
  CAMERA_IMAGE_BGR565,
  /**
   * @const CAMERA_IMAGE_RGBA8888
   * RGBA8888
   */
  CAMERA_IMAGE_RGBA8888
} camera_image_format_t;

END_C_DECLS

#endif /*TK_CAMERA_TYPES_H*/
