#ifndef FABLE_ENGINE_GFCOS_H
#define FABLE_ENGINE_GFCOS_H
#include "fable_compiler.h"
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(cos)
#endif
// Both PDBs declare double fastcall GFCos(float); original FableWin01d9e180 scales by the float two-pi value promoted to double.
inline double FABLE_FASTCALL GFCos(float angle){return cos(angle*(double)6.2831855f);}
#endif
