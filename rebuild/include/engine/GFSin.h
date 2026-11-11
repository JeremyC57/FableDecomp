#ifndef FABLE_ENGINE_GFSIN_H
#define FABLE_ENGINE_GFSIN_H
#include "fable_compiler.h"
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(sin)
#endif
// Original PDB signature ?GFSin@@YINM@Z; donor01dcb5d0 scales the float angle
// by the float two-pi value promoted to double before the CRT sine call.
inline double FABLE_FASTCALL GFSin(float angle) { return sin(angle*(double)6.2831855f); }
#endif
