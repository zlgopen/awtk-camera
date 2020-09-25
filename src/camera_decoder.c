/**
 * File:   camera_decoder.c
 * Author: AWTK Develop Team
 * Brief:  camera decoder interface
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

#include "camera_decoder.h"

bitmap_t* camera_decoder_decode(camera_decoder_t* d, const void* buffer, uint32_t size, uint32_t w,
                                uint32_t h) {
  return_value_if_fail(d != NULL && d->decode != NULL && buffer != NULL, NULL);

  return d->decode(d, buffer, size, w, h);
}

ret_t camera_decoder_destroy(camera_decoder_t* d) {
  return_value_if_fail(d != NULL && d->destroy != NULL, RET_BAD_PARAMS);

  return d->destroy(d);
}

ret_t camera_decoder_set_output_format(camera_decoder_t* d, bitmap_format_t oformat) {
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);
  d->oformat = oformat;

  return RET_OK;
}

