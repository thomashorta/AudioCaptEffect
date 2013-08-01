/*
= * fft.c
 *
 * Code generation for function 'fft'
 *
 * C source code generated on: Fri Jun 14 13:22:49 2013
 *
 */

/* Include files */
#include "abs_fft_hann_60ms.h"
#include "fft.h"

/* Type Definitions */
#define PI 3.1415926535897932384626433832795
/* Named Constants */

/* Variable Declarations */
static real_T costab[32769];
static real_T sintab[32769];

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */
static trigtabgen() {
	int32_T k;
	for (k = 0; k < 32769; k++) {
		costab[k] = cos(((real_T)k/32768)*PI);
		sintab[k] = sin(((real_T)k/32768)*PI);
	}
}

void fft(const real_T * x, const int32_T point_per_window, creal_T y[65536])
{
	int32_T i;
	int32_T k;
	int32_T length;
	int32_T ix;
	uint32_T ju;
	int32_T iy;
	uint32_T n;
	boolean_T tst;
	real_T temp_re;
	int32_T iDelta;
	int32_T iDelta2;
	int32_T iheight;
	real_T temp_im;
	int32_T ihi;
	for (i = 0; i < 65536; i++) {
		y[i].re = 0.0;
		y[i].im = 0.0;
	}

	length = (point_per_window % 2) == 1 ? point_per_window : point_per_window - 1;

	ix = 0;
	ju = 0U;
	iy = 0;
	for (i = 0; i < length; i++) {
		y[iy].re = x[ix];
		//__android_log_print(ANDROID_LOG_DEBUG, "NDK", "FFT: iy=%d | ix=%d", iy, ix);
		//y[iy].im = 0.0;
		n = 65536U;
		tst = TRUE;
		while (tst) {
			n >>= 1U;
			ju ^= n;
			tst = ((int32_T)(ju & n) == 0);
		}

		iy = (int32_T)ju;
		ix++;
	}

	y[iy].re = x[ix];
	y[iy].im = 0.0;

	for (i = 0; i < 65536; i += 2) {
		temp_re = y[i + 1].re;
		y[i + 1].re = y[i].re - y[i + 1].re;
		//y[i + 1].im = 0.0;
		y[i].re += temp_re;
		//y[i].im = 0.0;
	}

	iDelta = 2;
	iDelta2 = 4;
	k = 16384;
	iheight = 65533;
	while (k > 0) {
		for (i = 0; i < iheight; i += iDelta2) {
			ix = i + iDelta;
			temp_re = y[ix].re;
			//temp_im = y[ix].im;
			y[i + iDelta].re = y[i].re - y[ix].re;
			//y[i + iDelta].im = y[i].im - y[ix].im;
			y[i].re += temp_re;
			//y[i].im += temp_im;
		}

		ix = 1;
		for (iy = k; iy < 32768; iy += k) {
			i = ix;
			ihi = ix + iheight;
			while (i < ihi) {
				temp_re = costab[iy] * y[i + iDelta].re - sintab[iy] * y[i + iDelta].im;
				temp_im = costab[iy] * y[i + iDelta].im + sintab[iy] * y[i + iDelta].re;
				y[i + iDelta].re = y[i].re - temp_re;
				y[i + iDelta].im = y[i].im - temp_im;
				y[i].re += temp_re;
				y[i].im += temp_im;
				i += iDelta2;
			}

			ix++;
		}

		k /= 2;
		iDelta = iDelta2;
		iDelta2 <<= 1;
		iheight -= iDelta;
	}
}

/* End of code generation (fft.c) */
