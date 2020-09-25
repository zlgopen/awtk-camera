/**
 * File:   camera_decoder.h
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

#ifndef TK_CAMERA_DECODER_MJPEG_H
#define TK_CAMERA_DECODER_MJPEG_H

#include "camera_decoder.h"

BEGIN_C_DECLS

/**
 * @class camera_decoder_mjpeg_t
 * @parent camera_decoder_t
 * @annotation ["fake"]
 * mjpeg 解码器。
 */

/**
 * @method camera_decoder_mjpeg_create
 *
 * 创建decoder。
 *
 * @return {camera_decoder_t*} 返回decoder对象。
 *
 */
camera_decoder_t* camera_decoder_mjpeg_create(void);

END_C_DECLS

#endif /*TK_CAMERA_DECODER_MJPEG_H*/
