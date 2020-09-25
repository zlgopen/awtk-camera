/**
 * File:   camera_decoder_factory.c
 * Author: AWTK Develop Team
 * Brief:  camera decoder_factory
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
 * 2020-09-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "camera_decoder_factory.h"
#include "decoders/camera_decoder_mjpeg.h"
#include "decoders/camera_decoder_yuyv.h"

camera_decoder_t* camera_decoder_factory_create_decoder(camera_image_format_t format) {
  switch (format) {
    case CAMERA_IMAGE_YUYV:
      return camera_decoder_yuyv_create();
    case CAMERA_IMAGE_MJPEG:
      return camera_decoder_mjpeg_create();
    default:
      break;
  }

  return NULL;
}
