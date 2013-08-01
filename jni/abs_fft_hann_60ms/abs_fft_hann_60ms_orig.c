/*
 * abs_fft_hann_60ms.c
 *
 * Code generation for function 'abs_fft_hann_60ms'
 *
 * C source code generated on: Fri Jun 14 13:22:49 2013
 *
 */

/* Include files */
#include <fft.c>

#include "abs_fft_hann_60ms.h"
#include "fft.h"

/* Type Definitions */

/* Named Constants */
#define PI 3.1415926535897932384626433832795
#define DEBUG_TAG "NDK_ABS_FFT"

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
//static real_T rt_hypotd(real_T u0, real_T u1);
//static real_T* hann(int32_T length);

/* Function Definitions */
static real_T rt_hypotd(real_T u0, real_T u1)
{
    real_T y;
    real_T a;
    real_T b;
    a = fabs(u0);
    b = fabs(u1);
    if (a < b) {
        a /= b;
        y = b * sqrt(a * a + 1.0);
    } else if (a > b) {
        b /= a;
        y = a * sqrt(b * b + 1.0);
    } else {
        y = a * 1.4142135623730951;
    }
    
    return y;
}

static real_T hann(int32_T length, int32_T n) {
	return 0.5*(1-cos(2*PI*((real_T)n/((real_T)length-1))));
}

extern JNIEXPORT jdoubleArray JNICALL Java_com_test_audiocapteffect_MainActivity_absfft60ms
	(JNIEnv* env, jobject thiz, const jdoubleArray input, jint SR)
{
	jdouble* x = (*env)->GetDoubleArrayElements(env, input, 0);

	jdoubleArray output;
	output = (jdoubleArray)((*env)->NewDoubleArray(env, 65536));
	jdouble* y = (*env)->GetDoubleArrayElements(env, output, 0);

    int32_T i;
    
    static creal_T c_x[65536];
    
    switch (SR) {
        case 8000:
        {
            real_T b_x[480];
            
            __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "ABSFFT60MS: Multiplying hann*input");
            for (i = 0; i < 480; i++) {
                b_x[i] = x[i] * hann(480, i);
                //__android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "NDK: x[%d] = %f", i, x[i]);
                //__android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "NDK: wind[%d] = %f", i, hann(480, i));
                //__android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "NDK: b_x[%d] = %f", i, b_x[i]);
            }
            
            __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "ABSFFT60MS: Calling the fft function");
            f_fft(b_x, c_x);

            __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "ABSFFT60MS: Calculating the absolute value");
            for (i = 0; i < 65536; i++) {
                y[i] = rt_hypotd(fabs(c_x[i].re), fabs(c_x[i].im));
                //__android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "NDK: y[%d] = %f", i, y[i]);
                //__android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "NDK: REAL.c_x[%d] = %f | IMAG.c_x[%d] = %f", i, c_x[i].re, i, c_x[i].im);
            }
            __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "ABSFFT60MS: End of this function");
            break;
        }
        case 11025:
        {
            real_T d_x[662];
            
            for (i = 0; i < 662; i++) {
                d_x[i] = x[i] * hann(662, i);
            }
            
            e_fft(d_x, c_x);
            for (i = 0; i < 65536; i++) {
                y[i] = rt_hypotd(fabs(c_x[i].re), fabs(c_x[i].im));
            }
            break;
        }
        case 16000:
        {
            real_T e_x[960];
            
            for (i = 0; i < 960; i++) {
                e_x[i] = x[i] * hann(960, i);
            }
            
            d_fft(e_x, c_x);
            for (i = 0; i < 65536; i++) {
                y[i] = rt_hypotd(fabs(c_x[i].re), fabs(c_x[i].im));
            }
            break;
        }
        case 22050:
        {
            real_T f_x[1323];
            
            for (i = 0; i < 1323; i++) {
                f_x[i] = x[i] * hann(1323, i);
            }
            
            c_fft(f_x, c_x);
            for (i = 0; i < 65536; i++) {
                y[i] = rt_hypotd(fabs(c_x[i].re), fabs(c_x[i].im));
            }
            break;
        }
        case 32000:
        {
            real_T g_x[1920];
            
            for (i = 0; i < 1920; i++) {
                g_x[i] = x[i] * hann(1920, i);
            }
            
            b_fft(g_x, c_x);
            for (i = 0; i < 65536; i++) {
                y[i] = rt_hypotd(fabs(c_x[i].re), fabs(c_x[i].im));
            }
            break;
        }
        case 44100:
        {
            real_T h_x[2646];
            
            for (i = 0; i < 2646; i++) {
                h_x[i] = x[i] * hann(2646, i);
            }
            
            fft(h_x, c_x);
            for (i = 0; i < 65536; i++) {
                y[i] = rt_hypotd(fabs(c_x[i].re), fabs(c_x[i].im));
            }
            break;
        }
        default:
            break;
    }

    (*env)->ReleaseDoubleArrayElements(env, input, x, 0);
    (*env)->ReleaseDoubleArrayElements(env, output, y, 0);

    return output;
}

/* End of code generation (abs_fft_hann_60ms.c) */
