/**
 * File:   camera_device.c
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

#include "camera_device.h"

bitmap_t* camera_device_take_picture(object_t* obj) {
  camera_device_t* device = (camera_device_t*)obj;
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  return device->take_picture(obj);
}

ret_t camera_device_set_format(object_t* obj, camera_image_format_t format, uint32_t w,
                               uint32_t h) {
  object_set_prop_int(obj, CAMERA_PROP_WIDTH, w);
  object_set_prop_int(obj, CAMERA_PROP_HEIGHT, h);
  object_set_prop_int(obj, CAMERA_PROP_FORMAT, format);

  return object_exec(obj, CAMERA_CMD_APPLY, NULL);
}
