/**
 * File:   camera_device.h
 * Author: AWTK Develop Team
 * Brief:  camera device interface
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

#ifndef TK_CAMERA_DEVICE_H
#define TK_CAMERA_DEVICE_H

#include "tkc/object.h"
#include "base/bitmap.h"
#include "camera_types.h"

BEGIN_C_DECLS

typedef bitmap_t* (*camera_device_take_picture_t)(object_t* obj);

/**
 * @class camera_device_t
 * @parent object_t
 * @annotation ["scriptable"]
 *
 * 相机接口。
 *
 */
typedef struct _camera_device_t {
  object_t object;

	/*private*/
  camera_device_take_picture_t take_picture;
  void* priv;
} camera_device_t;

/**
 * @method camera_device_take_picture
 *
 * 拍照。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj 对象。
 *
 * @return {bitmap_t*} 返回位图对象。
 *
 */
bitmap_t* camera_device_take_picture(object_t* obj);

/**
 * @method camera_device_set_format
 *
 * 设置相机的图片格式。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj 对象。
 * @param {camera_image_format_t} format 相机输出的图像格式。
 * @param {uint32_t} w 图像的宽度。
 * @param {uint32_t} h 图像的高度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t camera_device_set_format(object_t* obj, camera_image_format_t format, uint32_t w, uint32_t h);

#define CAMERA_PROP_HUE "hue"
#define CAMERA_PROP_DEPTH "depth"
#define CAMERA_PROP_PALETTE "palette"
#define CAMERA_PROP_WITENESS "whiteness"
#define CAMERA_PROP_WIDTH "width"
#define CAMERA_PROP_HEIGHT "height"
#define CAMERA_PROP_FORMAT "format"
#define CAMERA_PROP_MIN_WIDTH "min_width"
#define CAMERA_PROP_MIN_HEIGHT "min_height"
#define CAMERA_PROP_MAX_HEIGHT "max_height"
#define CAMERA_PROP_BRIGHTNESS "brightness"
#define CAMERA_PROP_DESIRE_FORMAT "desire_format"

#define CAMERA_CMD_OPEN "open"
#define CAMERA_CMD_APPLY "apply"
#define CAMERA_CMD_CLOSE "close"

END_C_DECLS

#endif /*TK_CAMERA_DEVICE_H*/
