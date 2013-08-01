/*
 * addeffects_types.h
 *
 * Code generation for function 'addeffects'
 *
 * C source code generated on: Mon Jun  3 11:52:25 2013
 *
 */

#ifndef __ADDEFFECTS_TYPES_H__
#define __ADDEFFECTS_TYPES_H__

/* Type Definitions */
#ifndef struct_emxArray__common
#define struct_emxArray__common
typedef struct emxArray__common
{
    void *data;
    int32_T *size;
    int32_T allocatedSize;
    int32_T numDimensions;
    boolean_T canFreeData;
} emxArray__common;
#endif
#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T
typedef struct emxArray_real_T
{
    real_T *data;
    int32_T *size;
    int32_T allocatedSize;
    int32_T numDimensions;
    boolean_T canFreeData;
} emxArray_real_T;
#endif

#endif
/* End of code generation (addeffects_types.h) */
