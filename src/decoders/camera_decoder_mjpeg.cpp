/**
 * File:   camera_decoder_mjpeg.c
 * Author: AWTK Develop Team
 * Brief:  camera decoder
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
#include "libyuv.h"
#include <libyuv/mjpeg_decoder.h>

#include "decoders/camera_decoder_mjpeg.h"

using namespace libyuv;

typedef struct _camera_decoder_mjpeg_t {
  camera_decoder_t camera_decoder;

  uint32_t w;
  uint32_t h;
  uint32_t size;
  uint32_t capability;

  uint8_t* c1;
  uint8_t* c2;
  uint8_t* c3;
  uint8_t* planes[4];
  MJpegDecoder* decoder;
} camera_decoder_mjpeg_t;

static ret_t mjpeg_decoder_info_ensure_buffer(camera_decoder_t* d, uint32_t w, uint32_t h,
                                              uint32_t colorspace, uint32_t ncomponents) {
  camera_decoder_mjpeg_t* mjpeg = (camera_decoder_mjpeg_t*)d;
  uint32_t size = w * h;
  return_value_if_fail(mjpeg != NULL, RET_BAD_PARAMS);

  if (size < mjpeg->capability) {
    mjpeg->w = w;
    mjpeg->h = h;
    mjpeg->size = size;

    return RET_OK;
  }

  TKMEM_FREE(mjpeg->c1);
  TKMEM_FREE(mjpeg->c2);
  TKMEM_FREE(mjpeg->c3);

  mjpeg->size = 0;
  mjpeg->capability = 0;
  mjpeg->c1 = (uint8_t*)TKMEM_ALLOC(size);
  return_value_if_fail(mjpeg->c1 != NULL, RET_OOM);

  mjpeg->c2 = (uint8_t*)TKMEM_ALLOC(size);
  return_value_if_fail(mjpeg->c2 != NULL, RET_OOM);

  mjpeg->c3 = (uint8_t*)TKMEM_ALLOC(size);
  return_value_if_fail(mjpeg->c3 != NULL, RET_OOM);

  mjpeg->size = size;
  mjpeg->capability = size;
  mjpeg->planes[0] = mjpeg->c1;
  mjpeg->planes[1] = mjpeg->c2;
  mjpeg->planes[2] = mjpeg->c3;
  mjpeg->planes[3] = NULL;

  return RET_OK;
}

static bitmap_t* camera_decoder_mjpeg_decode(camera_decoder_t* d, const void* buffer, uint32_t size,
                                             uint32_t w, uint32_t h) {
  camera_decoder_mjpeg_t* mjpeg = (camera_decoder_mjpeg_t*)d;
  int32_t ncomponent = 0;
  int32_t color_space = 0;
  MJpegDecoder* decoder = mjpeg->decoder;
  ENSURE(decoder->LoadFrame((uint8_t*)(buffer), size));

  w = decoder->GetWidth();
  h = decoder->GetHeight();
  color_space = decoder->GetColorSpace();
  ncomponent = decoder->GetNumComponents();

  log_debug("w=%d h=%d color_space=%d ncomponent=%d buffer_size=%u\n", w, h, color_space,
            ncomponent, size);
  return_value_if_fail(mjpeg_decoder_info_ensure_buffer(d, w, h, color_space, ncomponent) == RET_OK,
                       NULL);
  decoder->DecodeToBuffers(mjpeg->planes, w, h);

  /*TODO: convert yuv to rgb*/

  decoder->UnloadFrame();

  return NULL;
}

static ret_t camera_decoder_mjpeg_destroy(camera_decoder_t* d) {
  camera_decoder_mjpeg_t* mjpeg = (camera_decoder_mjpeg_t*)d;

  delete mjpeg->decoder;
  TKMEM_FREE(mjpeg->c1);
  TKMEM_FREE(mjpeg->c2);
  TKMEM_FREE(mjpeg->c3);
  memset(mjpeg, 0x00, sizeof(*mjpeg));
  TKMEM_FREE(mjpeg);

  return RET_OK;
}

camera_decoder_t* camera_decoder_mjpeg_create(void) {
  camera_decoder_mjpeg_t* mjpeg = TKMEM_ZALLOC(camera_decoder_mjpeg_t);
  return_value_if_fail(mjpeg != NULL, NULL);

  mjpeg->camera_decoder.name = "mjpeg";
  mjpeg->camera_decoder.format = CAMERA_IMAGE_MJPEG;
  mjpeg->camera_decoder.decode = camera_decoder_mjpeg_decode;
  mjpeg->camera_decoder.destroy = camera_decoder_mjpeg_destroy;
  mjpeg->decoder = new MJpegDecoder();

  return (camera_decoder_t*)(mjpeg);
}

