#ifndef FABLE_ENGINE_GFPOWER_H
#define FABLE_ENGINE_GFPOWER_H
#include "fable_compiler.h"
#include <math.h>
// FableWin original symbol GFPower YIMMM, body01C954E0 and Update02F3F630/02F3FDA0.
// The original helper converts both floats for pow and returns a float.
inline float FABLE_FASTCALL GFPower(float base,float exponent){return (float)pow(base,exponent);}
#endif
