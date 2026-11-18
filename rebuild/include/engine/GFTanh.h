#ifndef FABLE_ENGINE_GFTANH_H
#define FABLE_ENGINE_GFTANH_H
#include "rebuild_abi.h"
#include <math.h>

// FableWin 0x024F4F60 and native calls to the tanh intrinsic.
inline double FABLE_FASTCALL GFTanh(double value) { return tanh(value); }
#endif
