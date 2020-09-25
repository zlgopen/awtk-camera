#include "tkc/platform.h"
#include "v4l2/camera_device_v4l2.h"

int main(int argc, char* argv[]) {
	uint32_t i = 0;
	bitmap_t* image = NULL;
	object_t* camera = NULL;
	platform_prepare();

	camera = camera_device_v4l2_create("/dev/video0", CAMERA_IMAGE_YUYV, 640, 480);
	return_value_if_fail(camera != NULL, 0);

	for(i = 0; i < 5; i++) {
		image = camera_device_take_picture(camera);

		if(image != NULL) {
			bitmap_save_png(image, "test.png");
			bitmap_destroy(image);
		}
	}
	OBJECT_UNREF(camera);

	return 0;
}

