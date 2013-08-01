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

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */
void b_fft(const real_T x[1920], creal_T y[65536])
{
	int32_T i;
	real_T costab1q[16385];
	int32_T k;
	static real_T costab[32769];
	static real_T sintab[32769];
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

	costab1q[0] = 1.0;
	for (k = 0; k < 8192; k++) {
		costab1q[k + 1] = cos(9.5873799242852573E-5 * ((real_T)k + 1.0));
	}

	for (k = 0; k < 8191; k++) {
		costab1q[k + 8193] = sin(9.5873799242852573E-5 * (16384.0 - ((real_T)k +
				8193.0)));
	}

	costab1q[16384] = 0.0;
	costab[0] = 1.0;
	sintab[0] = 0.0;
	for (k = 0; k < 16384; k++) {
		costab[k + 1] = costab1q[k + 1];
		sintab[k + 1] = -costab1q[16383 - k];
	}

	for (k = 0; k < 16384; k++) {
		costab[k + 16385] = -costab1q[16383 - k];
		sintab[k + 16385] = -costab1q[k + 1];
	}

	ix = 0;
	ju = 0U;
	iy = 0;
	for (i = 0; i < 1919; i++) {
		y[iy].re = x[ix];
		y[iy].im = 0.0;
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
		y[i + 1].im = 0.0;
		y[i].re += temp_re;
		y[i].im = 0.0;
	}

	iDelta = 2;
	iDelta2 = 4;
	k = 16384;
	iheight = 65533;
	while (k > 0) {
		for (i = 0; i < iheight; i += iDelta2) {
			ix = i + iDelta;
			temp_re = y[ix].re;
			temp_im = y[ix].im;
			y[i + iDelta].re = y[i].re - y[ix].re;
			y[i + iDelta].im = y[i].im - y[ix].im;
			y[i].re += temp_re;
			y[i].im += temp_im;
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

void c_fft(const real_T x[1324], creal_T y[65536])
{
	int32_T i;
	real_T costab1q[16385];
	int32_T k;
	static real_T costab[32769];
	static real_T sintab[32769];
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

	costab1q[0] = 1.0;
	for (k = 0; k < 8192; k++) {
		costab1q[k + 1] = cos(9.5873799242852573E-5 * ((real_T)k + 1.0));
	}

	for (k = 0; k < 8191; k++) {
		costab1q[k + 8193] = sin(9.5873799242852573E-5 * (16384.0 - ((real_T)k +
				8193.0)));
	}

	costab1q[16384] = 0.0;
	costab[0] = 1.0;
	sintab[0] = 0.0;
	for (k = 0; k < 16384; k++) {
		costab[k + 1] = costab1q[k + 1];
		sintab[k + 1] = -costab1q[16383 - k];
	}

	for (k = 0; k < 16384; k++) {
		costab[k + 16385] = -costab1q[16383 - k];
		sintab[k + 16385] = -costab1q[k + 1];
	}

	ix = 0;
	ju = 0U;
	iy = 0;
	for (i = 0; i < 1323; i++) {
		y[iy].re = x[ix];
		y[iy].im = 0.0;
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
		y[i + 1].im = 0.0;
		y[i].re += temp_re;
		y[i].im = 0.0;
	}

	iDelta = 2;
	iDelta2 = 4;
	k = 16384;
	iheight = 65533;
	while (k > 0) {
		for (i = 0; i < iheight; i += iDelta2) {
			ix = i + iDelta;
			temp_re = y[ix].re;
			temp_im = y[ix].im;
			y[i + iDelta].re = y[i].re - y[ix].re;
			y[i + iDelta].im = y[i].im - y[ix].im;
			y[i].re += temp_re;
			y[i].im += temp_im;
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

void d_fft(const real_T x[960], creal_T y[65536])
{
	int32_T i;
	real_T costab1q[16385];
	int32_T k;
	static real_T costab[32769];
	static real_T sintab[32769];
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

	costab1q[0] = 1.0;
	for (k = 0; k < 8192; k++) {
		costab1q[k + 1] = cos(9.5873799242852573E-5 * ((real_T)k + 1.0));
	}

	for (k = 0; k < 8191; k++) {
		costab1q[k + 8193] = sin(9.5873799242852573E-5 * (16384.0 - ((real_T)k +
				8193.0)));
	}

	costab1q[16384] = 0.0;
	costab[0] = 1.0;
	sintab[0] = 0.0;
	for (k = 0; k < 16384; k++) {
		costab[k + 1] = costab1q[k + 1];
		sintab[k + 1] = -costab1q[16383 - k];
	}

	for (k = 0; k < 16384; k++) {
		costab[k + 16385] = -costab1q[16383 - k];
		sintab[k + 16385] = -costab1q[k + 1];
	}

	ix = 0;
	ju = 0U;
	iy = 0;
	for (i = 0; i < 959; i++) {
		y[iy].re = x[ix];
		y[iy].im = 0.0;
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
		y[i + 1].im = 0.0;
		y[i].re += temp_re;
		y[i].im = 0.0;
	}

	iDelta = 2;
	iDelta2 = 4;
	k = 16384;
	iheight = 65533;
	while (k > 0) {
		for (i = 0; i < iheight; i += iDelta2) {
			ix = i + iDelta;
			temp_re = y[ix].re;
			temp_im = y[ix].im;
			y[i + iDelta].re = y[i].re - y[ix].re;
			y[i + iDelta].im = y[i].im - y[ix].im;
			y[i].re += temp_re;
			y[i].im += temp_im;
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

void e_fft(const real_T x[662], creal_T y[65536])
{
	int32_T i;
	real_T costab1q[16385];
	int32_T k;
	static real_T costab[32769];
	static real_T sintab[32769];
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

	costab1q[0] = 1.0;
	for (k = 0; k < 8192; k++) {
		costab1q[k + 1] = cos(9.5873799242852573E-5 * ((real_T)k + 1.0));
	}

	for (k = 0; k < 8191; k++) {
		costab1q[k + 8193] = sin(9.5873799242852573E-5 * (16384.0 - ((real_T)k +
				8193.0)));
	}

	costab1q[16384] = 0.0;
	costab[0] = 1.0;
	sintab[0] = 0.0;
	for (k = 0; k < 16384; k++) {
		costab[k + 1] = costab1q[k + 1];
		sintab[k + 1] = -costab1q[16383 - k];
	}

	for (k = 0; k < 16384; k++) {
		costab[k + 16385] = -costab1q[16383 - k];
		sintab[k + 16385] = -costab1q[k + 1];
	}

	ix = 0;
	ju = 0U;
	iy = 0;
	for (i = 0; i < 661; i++) {
		y[iy].re = x[ix];
		y[iy].im = 0.0;
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
		y[i + 1].im = 0.0;
		y[i].re += temp_re;
		y[i].im = 0.0;
	}

	iDelta = 2;
	iDelta2 = 4;
	k = 16384;
	iheight = 65533;
	while (k > 0) {
		for (i = 0; i < iheight; i += iDelta2) {
			ix = i + iDelta;
			temp_re = y[ix].re;
			temp_im = y[ix].im;
			y[i + iDelta].re = y[i].re - y[ix].re;
			y[i + iDelta].im = y[i].im - y[ix].im;
			y[i].re += temp_re;
			y[i].im += temp_im;
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

void f_fft(const real_T x[480], creal_T y[65536])
{
	int32_T i;
	real_T costab1q[16385];
	int32_T k;
	static real_T costab[32769];
	static real_T sintab[32769];
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

	costab1q[0] = 1.0;
	for (k = 0; k < 8192; k++) {
		costab1q[k + 1] = cos(9.5873799242852573E-5 * ((real_T)k + 1.0));
	}

	for (k = 0; k < 8191; k++) {
		costab1q[k + 8193] = sin(9.5873799242852573E-5 * (16384.0 - ((real_T)k +
				8193.0)));
	}

	costab1q[16384] = 0.0;
	costab[0] = 1.0;
	sintab[0] = 0.0;
	for (k = 0; k < 16384; k++) {
		costab[k + 1] = costab1q[k + 1];
		sintab[k + 1] = -costab1q[16383 - k];
	}

	for (k = 0; k < 16384; k++) {
		costab[k + 16385] = -costab1q[16383 - k];
		sintab[k + 16385] = -costab1q[k + 1];
	}

	ix = 0;
	ju = 0U;
	iy = 0;
	for (i = 0; i < 479; i++) {
		y[iy].re = x[ix];
		y[iy].im = 0.0;
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
		y[i + 1].im = 0.0;
		y[i].re += temp_re;
		y[i].im = 0.0;
	}

	iDelta = 2;
	iDelta2 = 4;
	k = 16384;
	iheight = 65533;
	while (k > 0) {
		for (i = 0; i < iheight; i += iDelta2) {
			ix = i + iDelta;
			temp_re = y[ix].re;
			temp_im = y[ix].im;
			y[i + iDelta].re = y[i].re - y[ix].re;
			y[i + iDelta].im = y[i].im - y[ix].im;
			y[i].re += temp_re;
			y[i].im += temp_im;
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

void fft(const real_T x[2646], creal_T y[65536])
{
	int32_T i;
	real_T costab1q[16385];
	int32_T k;
	static real_T costab[32769];
	static real_T sintab[32769];
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

	costab1q[0] = 1.0;
	for (k = 0; k < 8192; k++) {
		costab1q[k + 1] = cos(9.5873799242852573E-5 * ((real_T)k + 1.0));
	}

	for (k = 0; k < 8191; k++) {
		costab1q[k + 8193] = sin(9.5873799242852573E-5 * (16384.0 - ((real_T)k +
				8193.0)));
	}

	costab1q[16384] = 0.0;
	costab[0] = 1.0;
	sintab[0] = 0.0;
	for (k = 0; k < 16384; k++) {
		costab[k + 1] = costab1q[k + 1];
		sintab[k + 1] = -costab1q[16383 - k];
	}

	for (k = 0; k < 16384; k++) {
		costab[k + 16385] = -costab1q[16383 - k];
		sintab[k + 16385] = -costab1q[k + 1];
	}

	ix = 0;
	ju = 0U;
	iy = 0;
	for (i = 0; i < 2645; i++) {
		y[iy].re = x[ix];
		y[iy].im = 0.0;
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
		y[i + 1].im = 0.0;
		y[i].re += temp_re;
		y[i].im = 0.0;
	}

	iDelta = 2;
	iDelta2 = 4;
	k = 16384;
	iheight = 65533;
	while (k > 0) {
		for (i = 0; i < iheight; i += iDelta2) {
			ix = i + iDelta;
			temp_re = y[ix].re;
			temp_im = y[ix].im;
			y[i + iDelta].re = y[i].re - y[ix].re;
			y[i + iDelta].im = y[i].im - y[ix].im;
			y[i].re += temp_re;
			y[i].im += temp_im;
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
