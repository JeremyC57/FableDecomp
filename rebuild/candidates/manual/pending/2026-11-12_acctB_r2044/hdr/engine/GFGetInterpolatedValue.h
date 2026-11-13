#ifndef FABLE_ENGINE_GFGETINTERPOLATEDVALUE_H
#define FABLE_ENGINE_GFGETINTERPOLATEDVALUE_H
#include "rebuild_abi.h"
// Original FableWin01d8e920 function and debug006fd5e0 caller graph; by-value float parameters.
#ifdef FABLE_RETAIL_GFGETINTERPOLATEDVALUE_NAMED
// Retail opt-in (chunk 2044, row 0x00AA0090): FableWin.pdb's /Od copy of ?GFGetInterpolatedValue@@YIMMMM@Z is 12i / 28b, exactly this
// text under VC7.1 /Od (the one-expression body is 8i / 19b); the named step folds away in release.
inline float FABLE_FASTCALL GFGetInterpolatedValue(float old_val,float new_val,float interp){float delta=(new_val-old_val)*interp;return old_val+delta;}
#else
inline float FABLE_FASTCALL GFGetInterpolatedValue(float old_val,float new_val,float interp){return (new_val-old_val)*interp+old_val;}
#endif
#endif
