#by Thomas Oliveira Horta
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := nativefunc
LOCAL_SRC_FILES := addeffects/addeffects_lib.c 
LOCAL_SRC_FILES += abs_fft_hann_60ms/abs_fft_hann_60ms.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/abs_fft_hann_60ms
LOCAL_C_INCLUDES += $(LOCAL_PATH)/addeffects
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
