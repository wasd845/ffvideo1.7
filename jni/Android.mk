LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CFLAGS := -D__STDC_CONSTANT_MACROS

LOCAL_SRC_FILES := \
		onLoad.cpp \
		FFMpegPlayerAndroid.cpp

LOCAL_SHARED_LIBRARIES := libffplay

LOCAL_MODULE := libffvideo

include $(BUILD_SHARED_LIBRARY)
