#ifndef FABLE_ENGINE_GFFASTSIN_H
#define FABLE_ENGINE_GFFASTSIN_H
#ifdef FABLE_RETAIL_ANIMATION_KEYFRAME_TRACKS_PASS30
#include "engine/GFFloatToLongNear.h"
extern float GFastCosTable[1025];
// Inline-budget: SetSlerp expands the two angle lookups even with the shared rounding helper.
FABLE_FORCEINLINE float FABLE_FASTCALL GFFastSin(float angle)
{
    float pos = angle * 1024.0f;
    long index = GFFloatToLongNear(pos - 0.5f);
    float interp = pos - index;
    index -= 256;
    index &= 1023;
    float result = (1.0f - interp) * GFastCosTable[index] + interp * GFastCosTable[index + 1];
    return result;
}
#else
#include "engine/GFFastCos.h"
#endif
#ifndef FABLE_RETAIL_ANIMATION_KEYFRAME_TRACKS_PASS30
// Original FableWin public ?GFFastSin@@YIMM@Z and02DCFC30, called by the
// original occlusion module973. Same cosine table, quarter-turn index shift.
#ifdef FABLE_RETAIL_RIGHTHANDEDSET_INTERPOLATE_PASS30
// Inline-budget: native A234F8 and A23564 expand the two table reads and rounding operations.
FABLE_FORCEINLINE float FABLE_FASTCALL GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i -= 256;
    i &= 0x3ff;
    float a = GFastCosTable[i];
    float b = GFastCosTable[i + 1];
    return (1.0f - frac) * a + frac * b;
}
#else
inline float FABLE_FASTCALL GFFastSin(float angle)
{
    float pos=angle*1024.0f;
    long index=GFFloatToLongNear(pos-0.5f);
    float interp=pos-(float)index;
    index=(index-256)&0x3ff;
    return (1.0f-interp)*GFastCosTable[index]+interp*GFastCosTable[index+1];
}
#endif
#endif
#endif
