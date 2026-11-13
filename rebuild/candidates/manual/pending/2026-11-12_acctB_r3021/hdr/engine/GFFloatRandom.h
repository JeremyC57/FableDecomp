#ifndef FABLE_GFFLOATRANDOM_H
#define FABLE_GFFLOATRANDOM_H
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_GFFLOATRANDOM_INLINE
// Retail opt-in (chunk 2005, row 0x00B51570): the inline body (Ego_r.pdb / FableWin.pdb ?GFFloatRandom@@YIMMPAK@Z; its emitted COMDAT
// is retail 0x004978A0, 113 bytes; expanded at 0x00B51750): the seed stepped as in GFRandom, then the rotated seed (unsigned) modulo
// max * 65536 through fmod (_CIfmod), scaled back by 1/65536; 0 when max * 65536 <= 0.0001.
#include "engine/GFRandom.h"
extern "C" double __cdecl fmod(double, double);
#pragma intrinsic(fmod)
inline float FABLE_FASTCALL GFFloatRandom(float max, unsigned long* seed)
{
    double range = max;
    range *= 65536.0;
    *seed *= 0x24a1;
    *seed += 0x24df;
    *seed = GFROR13(*seed);
    if (range > 0.0001)
    {
        double result = fmod((double)*seed, range);
        result *= (1.0 / 65536.0);
        return (float)result;
    }
    return 0.0f;
}
#else
float FABLE_FASTCALL GFFloatRandom(float,unsigned long*);
#endif
#endif
