/**
 * File:   camera_decoder_factory.h
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

#ifndef TK_CAMERA_DECODER_FACTORY_H
#define TK_CAMERA_DECODER_FACTORY_H

#include "base/bitmap.h"
#include "camera_types.h"
#include "camera_decoder.h"

BEGIN_C_DECLS


/**
 * @class camera_decoder_factory_t
 * @annotation ["fake"]
 * 相机图像解码接口工厂。
 *
 */

/**
 * @method camera_decoder_factory_create_decoder
 * 创建decoder对象。
 * @param {camera_image_format_t} format 格式。
 *
 * @return {camera_decoder_t*} 返回decoder对象。
 *
 */
camera_decoder_t* camera_decoder_factory_create_decoder(camera_image_format_t format);

END_C_DECLS

#endif /*TK_CAMERA_DECODER_FACTORY_H*/
