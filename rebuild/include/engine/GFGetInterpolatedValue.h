#ifndef FABLE_ENGINE_GFGETINTERPOLATEDVALUE_H
#define FABLE_ENGINE_GFGETINTERPOLATEDVALUE_H
#include "rebuild_abi.h"
// Original FableWin01d8e920 function and debug006fd5e0 caller graph; by-value float parameters.
inline float FABLE_FASTCALL GFGetInterpolatedValue(float old_val,float new_val,float interp){return (new_val-old_val)*interp+old_val;}
#endif
