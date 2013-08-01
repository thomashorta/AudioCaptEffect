/*
 * addeffects.c
 *
 * Code generation for function 'addeffects'
 *
 * Code modified by Thomas Horta
 *
 * C source code generated on: Mon Jun  3 11:52:25 2013
 *
 */

/* Include files */
#include <addeffects_emxutil.c>
#include <mean.c>
#include <abs.c>
#include <limits.h>
#include <android/log.h>

#include "addeffects_lib.h"
#include "addeffects_emxutil.h"
#include "mean.h"
#include "abs.h"

#define DEBUG_TAG "NDK_SimpleCApp"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
static real_T rt_roundd(real_T u);

/* Function Definitions */
static real_T rt_roundd(real_T u)
{
    real_T y;
    if (fabs(u) < 4.503599627370496E+15) {
        if (u >= 0.5) {
            y = floor(u + 0.5);
        } else if (u > -0.5) {
            y = 0.0;
        } else {
            y = ceil(u - 0.5);
        }
    } else {
        y = u;
    }
    
    return y;
}

JNIEXPORT jshortArray JNICALL Java_com_test_audiocapteffect_MainActivity_addeffects
	(JNIEnv* env, jobject thiz, const jshortArray input, int32_T SampleRate, int32_T sizeofx) {

	__android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "NDK: Creating/Getting Array Elements");
    jshort* x = (*env)->GetShortArrayElements(env, input, 0);
    //int16_T* x = (int16_T*)j_input;
    jshortArray output;
    output = (jshortArray)((*env)->NewShortArray(env, sizeofx));
    jshort* y = (*env)->GetShortArrayElements(env, output, 0);
    //int16_T* y = (int16_T*)j_output;
    
    __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "NDK: Creating variables");
    emxArray_real_T *temp;
    int32_T b_y;
    int32_T i;
    int16_T iv0[sizeofx];
    real_T thresh;
    uint32_T delayedindex;
    emxInit_real_T(&temp, 1);
    b_y = (int32_T)rt_roundd(0.3 * (real_T)SampleRate) - 1;

    __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "NDK: Filling y with zeros");
    for(i = 0; i<sizeofx; i++) {
    	y[i] = 0;
    }

    __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "NDK: Adding distortion part 1");
    /* add distortion */
    /* distortion function */
    i = temp->size[0];
    temp->size[0] = sizeofx;
    emxEnsureCapacity((emxArray__common *)temp, i, (int32_T)sizeof(real_T));
    
    __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "NDK: Adding distortion part 2");
    // get the absolute values of each element
    for (i = 0; i < sizeofx; i++) {
        iv0[i] = b_abs(x[i]);
    }
    
    __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "NDK: Adding distortion part 3");
    thresh = 1.2 * mean(iv0, sizeofx);
	for (i = 0; i + 1 <= sizeofx; i++) {
		if ((real_T) x[i] > thresh) {
			temp->data[i] = thresh;
		} else if ((real_T) x[i] < -thresh) {
			temp->data[i] = -thresh;
		} else {
			temp->data[i] = (real_T) x[i];
		}
	}
    

    __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "NDK: Adding delay part 1");
    /* add delay */
    /* delay function */
    /* modified by Thomas Horta */    
    for (i = 0; i <= sizeofx - 1; i++) {
		delayedindex = i - b_y;
		if (1 < delayedindex) {
		} else {
			delayedindex = 1;
		}

        if (i==1) {
        	__android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "NDK: Adding delay part 2");
        }

        if (i - b_y < 1) {
			y[i] = temp->data[i];
		} else {
			y[i] = temp->data[i] + 0.35 * temp->data[delayedindex - 1];
		}
	}
    
    __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "NDK: Freeing resources");
    
    emxFree_real_T(&temp);
    (*env)->ReleaseShortArrayElements(env, input, x, 0);
    (*env)->ReleaseShortArrayElements(env, output, y, 0);
    
    __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "NDK: Returning output");
    return output;
}

/* End of code generation (addeffects.c) */
