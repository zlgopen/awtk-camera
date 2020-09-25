/**
 * File:   camera_decoder_yuyv.c
 * Author: AWTK Develop Team
 * Brief:  camera yuyv decoder
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

#include "tkc/mem.h"
#include "libyuv/convert.h"
#include "decoders/camera_decoder_yuyv.h"

using namespace libyuv;

static bitmap_t* camera_decoder_yuyv_decode(camera_decoder_t* d, const void* buffer, uint32_t size,
                                            uint32_t w, uint32_t h) {
  uint8_t* dst = NULL;
  uint32_t src_stride = w * 2;
  uint32_t dst_stride = w * 4;
  bitmap_t* image = bitmap_create_ex(w, h, w * 4, BITMAP_FMT_RGBA8888);
  return_value_if_fail(image != NULL, NULL);

  dst = (uint8_t*)bitmap_lock_buffer_for_write(image);

  if (dst != NULL) {
    int ret = YUY2ToARGB((const uint8_t*)buffer, src_stride, dst, dst_stride, w, h);
    ENSURE(ret == 0);
    bitmap_unlock_buffer(image);
    image->flags |= BITMAP_FLAG_OPAQUE;
  } else {
    bitmap_destroy(image);
    image = NULL;
  }

  return image;
}

static ret_t camera_decoder_yuyv_destroy(camera_decoder_t* d) {
  TKMEM_FREE(d);

  return RET_OK;
}

camera_decoder_t* camera_decoder_yuyv_create(void) {
  camera_decoder_yuyv_t* yuyv = TKMEM_ZALLOC(camera_decoder_yuyv_t);
  return_value_if_fail(yuyv != NULL, NULL);

  yuyv->camera_decoder.name = "yuyv";
  yuyv->camera_decoder.format = CAMERA_IMAGE_YUYV;
  yuyv->camera_decoder.decode = camera_decoder_yuyv_decode;
  yuyv->camera_decoder.destroy = camera_decoder_yuyv_destroy;

  return (camera_decoder_t*)(yuyv);
}
