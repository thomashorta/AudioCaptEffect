/*
 * addeffects.h
 *
 * Code generation for function 'addeffects'
 *
 * C source code generated on: Mon Jun  3 11:52:25 2013
 *
 */

#ifndef __ADDEFFECTS_H__
#define __ADDEFFECTS_H__
/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <jni.h>

#include "rtwtypes.h"
#include "addeffects_types.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
//beginning of the JNI .h usual file
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jshortArray JNICALL Java_com_test_audiocapteffect_MainActivity_addeffects
(JNIEnv* env, jobject thiz, const jshortArray input, int32_T SampleRate, int32_T sizeofx);

#ifdef __cplusplus
};
#endif
// end for the JNI .h usual file

#endif
/* End of code generation (addeffects.h) */
