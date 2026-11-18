#ifndef FABLE_ENGINE_GFSIN_H
#define FABLE_ENGINE_GFSIN_H
#include "fable_compiler.h"
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(sin)
#endif
// Original PDB signature ?GFSin@@YINM@Z; donor01dcb5d0 scales the float angle
// by the float two-pi value promoted to double before the CRT sine call.
#ifdef FABLE_RETAIL_MATH_TRIG_TWO_PI
// Retail opt-in (Codex pass 24, 0x00A55F9B): both calls share the
// established double scale at 0x0124F2B8.
extern const double TWO_PI;
inline double FABLE_FASTCALL GFSin(float angle) { return sin(angle * TWO_PI); }
#else
inline double FABLE_FASTCALL GFSin(float angle) { return sin(angle*(double)6.2831855f); }
#endif
#endif
