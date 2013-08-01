/*
 * abs.c
 *
 * Code generation for function 'abs'
 *
 * Code modified by Thomas Horta
 *
 * C source code generated on: Mon Jun  3 11:52:25 2013
 *
 */

/* Include files */
#include "addeffects_lib.h"
#include "abs.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */
int16_T b_abs(const int16_T a)
{
    int16_T b;
    if (a < 0) {
        b = (int16_T)-a;
    } else {
        b = a;
    }
    
    return b;
}

/* End of code generation (abs.c) */
