import os
import scripts.app_helper as app

helper = app.Helper(ARGUMENTS);

APP_CPPPATH=[
	os.path.join(helper.APP_ROOT, '3rd/libv4l2cpp/inc'),
	os.path.join(helper.APP_ROOT, '3rd/libyuv/include')
]

APP_CCFLAGS=' -DHAVE_JPEG '
helper.add_ccflags(APP_CCFLAGS)
helper.add_libs(['camera_device_v4l2', 'v4l2cpp', 'yuv', 'jpeg'])
helper.add_cpppath(APP_CPPPATH).call(DefaultEnvironment)

SConscriptFiles = ['src/SConscript', '3rd/SConscript', 'demos/SConscript']
SConscript(SConscriptFiles)
