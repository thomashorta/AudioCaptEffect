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

static real_T fft_peak_pos(real_T* x, int32_T SR){
	int32_T i;
	int32_T ppw;
	real_T peak_val;
	real_T peak_freq;
	real_T freq_res;
	real_T y[65536];
	static creal_T c_x[65536];

	freq_res = (real_T)SR/65535;
	ppw = (int)round(0.06*SR);

	fft(x, ppw, c_x);

	for (i = 0; i < 65536; i++) {
		y[i] = rt_hypotd(fabs(c_x[i].re), fabs(c_x[i].im));
	}

    peak_val = 0;
    peak_freq = 0;
    for (i = 0; i < (65536/2); i++) {
    	if(y[i] > peak_val && y[i] > 0.15) {
    		peak_val = y[i];
    		peak_freq = (real_T)i*freq_res;
    	}
    }

    //__android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "PEAKSFun: Peak value = %f | Peak freq = %f", peak_val, peak_freq);

    return peak_freq;
}

extern JNIEXPORT jdoubleArray JNICALL Java_com_test_audiocapteffect_MainActivity_absfft60ms
	(JNIEnv* env, jobject thiz, const jdoubleArray input, jint SR)
{
	jdouble* x = (*env)->GetDoubleArrayElements(env, input, 0);
	jsize length = (*env)->GetArrayLength(env, input);

	trigtabgen();

	int32_T i;
    int32_T point_per_window;
    int32_T point_per_timestep;
    int32_T minframeIndex;
    int32_T nfrm;
    int32_T cur_frm;

    point_per_window = (int)round(0.06*SR);
    point_per_timestep = (int)round(0.01*SR);
    minframeIndex = 0;
    nfrm = (int32_T)floor((real_T)(length-point_per_window)/(real_T)point_per_timestep) + 1;
    cur_frm = 0;

    real_T* frame = (real_T*)malloc(sizeof(real_T)*point_per_window);

    jdoubleArray output;
    output = (jdoubleArray)((*env)->NewDoubleArray(env, nfrm));
    jdouble* y = (*env)->GetDoubleArrayElements(env, output, 0);

    while(cur_frm < nfrm) {
    	for (i=0; i<point_per_window; i++){
    		frame[i] = x[i+minframeIndex] * hann(point_per_window, i);
    	}

    	y[cur_frm] = fft_peak_pos(frame, SR);

    	minframeIndex += point_per_timestep;
    	cur_frm += 1;
    }

    free(frame);
    (*env)->ReleaseDoubleArrayElements(env, input, x, 0);
    (*env)->ReleaseDoubleArrayElements(env, output, y, 0);

    return output;
}

/* End of code generation (abs_fft_hann_60ms.c) */
