/*
 * addeffects_emxutil.h
 *
 * Code generation for function 'addeffects_emxutil'
 *
 * C source code generated on: Mon Jun 10 15:41:50 2013
 *
 */

#ifndef __ADDEFFECTS_EMXUTIL_H__
#define __ADDEFFECTS_EMXUTIL_H__
/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "rtwtypes.h"
#include "addeffects_types.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
extern void b_emxInit_real_T(emxArray_real_T **pEmxArray, int32_T numDimensions);
extern void emxEnsureCapacity(emxArray__common *emxArray, int32_T oldNumel, int32_T elementSize);
extern void emxFree_real_T(emxArray_real_T **pEmxArray);
extern void emxInit_real_T(emxArray_real_T **pEmxArray, int32_T numDimensions);
#endif
/* End of code generation (addeffects_emxutil.h) */
