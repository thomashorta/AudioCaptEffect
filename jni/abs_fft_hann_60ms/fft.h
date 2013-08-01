/*
 * fft.h
 *
 * Code generation for function 'fft'
 *
 * C source code generated on: Fri Jun 14 13:22:49 2013
 *
 */

#ifndef __FFT_H__
#define __FFT_H__
/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "rtwtypes.h"
#include "abs_fft_hann_60ms_types.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
//extern void b_fft(const real_T x[1920], creal_T y[65536]);
//extern void c_fft(const real_T x[1324], creal_T y[65536]);
//extern void d_fft(const real_T x[960], creal_T y[65536]);
//extern void e_fft(const real_T x[662], creal_T y[65536]);
//extern void f_fft(const real_T x[480], creal_T y[65536]);
//extern void fft(const real_T x[2646], creal_T y[65536]);
extern void fft(const real_T * x, int32_T point_per_window, creal_T y[65536]);
#endif
/* End of code generation (fft.h) */
