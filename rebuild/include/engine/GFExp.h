#ifndef FABLE_ENGINE_GFEXP_PASS26_H
#define FABLE_ENGINE_GFEXP_PASS26_H
#include "fable_compiler.h"
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(exp)
#endif
// Both original PDBs ?GFExp@@YINN@Z; debug donor020D6B10 forwards to the CRT exponential.
inline double FABLE_FASTCALL GFExp(double x) { return exp(x); }
#endif
