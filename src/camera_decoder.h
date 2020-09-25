/**
 * File:   camera_decoder.h
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

#ifndef TK_CAMERA_DECODER_H
#define TK_CAMERA_DECODER_H

#include "base/bitmap.h"
#include "camera_types.h"

BEGIN_C_DECLS

struct _camera_decoder_t;
typedef struct _camera_decoder_t camera_decoder_t;

typedef bitmap_t* (*camera_decoder_decode_t)(camera_decoder_t* d, const void* buffer, uint32_t size,
                                             uint32_t w, uint32_t h);
typedef ret_t (*camera_decoder_destroy_t)(camera_decoder_t* d);

/**
 * @class camera_decoder_t
 * 相机图像解码接口。
 *
 */
struct _camera_decoder_t {
  /**
   * @property {const char*} name
   * @annotation ["readable"]
   * 名称。
   */
  const char* name;
  /**
   * @property {camera_image_format_t} format
   * @annotation ["readable"]
   * 支持的格式(input)。
   */
  camera_image_format_t format;

  /**
   * @property {bitmap_format_t} oformat
   * @annotation ["readable"]
   * 输出图片的格式。
   */
  bitmap_format_t oformat;

	/*private*/
  camera_decoder_decode_t decode;
  camera_decoder_destroy_t destroy;
};

/**
 * @method camera_decoder_decode
 *
 * 解码图像。
 *
 * @param {camera_decoder_t*} d decoder对象。
 * @param {const void*} buffer 图像数据。
 * @param {uint32_t} size 图像数据的长度。
 * @param {uint32_t} w 图像的宽度。
 * @param {uint32_t} h 图像的宽度。
 *
 * @return {bitmap_t*} 返回解码后的位图对象。
 *
 */
bitmap_t* camera_decoder_decode(camera_decoder_t* d, const void* buffer, uint32_t size, uint32_t w,
                                uint32_t h);

/**
 * @method camera_decoder_set_output_format
 *
 * 设置输出图像的格式。
 *
 * @param {camera_decoder_t*} d decoder对象。
 * @param {bitmap_format_t} format 输出图像的格式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t camera_decoder_set_output_format(camera_decoder_t* d, bitmap_format_t oformat);

/**
 * @method camera_decoder_destroy
 *
 * 销毁解码器。
 *
 * @param {camera_decoder_t*} d decoder对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t camera_decoder_destroy(camera_decoder_t* d);

END_C_DECLS

#endif /*TK_CAMERA_DECODER_H*/

