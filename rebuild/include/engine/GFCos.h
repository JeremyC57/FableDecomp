#ifndef FABLE_ENGINE_GFCOS_H
#define FABLE_ENGINE_GFCOS_H
#include "fable_compiler.h"
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(cos)
#endif
// Both PDBs declare double fastcall GFCos(float); original FableWin01d9e180 scales by the float two-pi value promoted to double.
#ifdef FABLE_RETAIL_MATH_TRIG_TWO_PI
// Retail opt-in (Codex pass 24, 0x00A55F9B): both calls share the
// established double scale at 0x0124F2B8.
extern const double TWO_PI;
inline double FABLE_FASTCALL GFCos(float angle) { return cos(angle * TWO_PI); }
#else
inline double FABLE_FASTCALL GFCos(float angle){return cos(angle*(double)6.2831855f);}
#endif
#endif
