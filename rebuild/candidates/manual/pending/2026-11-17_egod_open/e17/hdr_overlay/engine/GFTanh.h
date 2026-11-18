#ifndef FABLE_ENGINE_GFTANH_H
#define FABLE_ENGINE_GFTANH_H
#include "fable_compiler.h"
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(tanh)
#endif
// New header (e17, row 0x00875FB0 NCameraModes::DrawSquare): FableWin.pdb ?GFTanh@@YINN@Z (double fastcall GFTanh(double)), the CRT tanh
// (VC7.1 intrinsic: retail calls _CItanh 0x00BFECE2), written like the shared GFSin.h / GFCos.h.
inline double FABLE_FASTCALL GFTanh(double value){return tanh(value);}
#endif
