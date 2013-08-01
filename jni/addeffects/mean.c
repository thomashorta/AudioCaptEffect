/*
 * mean.c
 *
 * Code generation for function 'mean'
 *
 * C source code generated on: Mon Jun  3 11:52:25 2013
 *
 */

/* Include files */
#include "addeffects_lib.h"
#include "mean.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */
real_T mean(const int16_T x[], int32_T sizeofx)
{
  real_T y;
  int32_T k;
  y = 0;
  for (k = 0; k < sizeofx; k++) {
    y += (real_T)x[k];
  }

  y /= (real_T)sizeofx;
  return y;
}

/* End of code generation (mean.c) */
