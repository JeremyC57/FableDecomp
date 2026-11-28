#ifndef FABLE_ENGINE_GFFASTARCCOS_H
#define FABLE_ENGINE_GFFASTARCCOS_H
#ifdef FABLE_RETAIL_ANIMATION_KEYFRAME_TRACKS_PASS30
#include "engine/GFFloatToLongNear.h"
extern float GFastArcCosTable[1026];
// Inline-budget: both animation routines expand this table lookup through SetSlerp.
FABLE_FORCEINLINE float FABLE_FASTCALL GFFastArcCos(float value)
{
    float pos = (float)((value + 1.0f) * 512.5f);
    long index = GFFloatToLongNear(pos - 0.5f);
    float interp = pos - index;
    float result = (1.0f - interp) * GFastArcCosTable[index] + interp * GFastArcCosTable[index + 1];
    result *= 0.15915494f;
    return result;
}
#elif defined(FABLE_RETAIL_RIGHTHANDEDSET_INTERPOLATE_PASS30)
#include "engine/GFFloatToLongNear.h"
extern float GFastArcCosTable[1026];
// Inline-budget: the original SetSlerp expands GFFastArcCos, with the table indexed in Interpolate at 00A234BB.
FABLE_FORCEINLINE float FABLE_FASTCALL GFFastArcCos(float cosine)
{
    cosine = (cosine + 1.0f) * 512.5f;
    long i = GFFloatToLongNear(cosine - 0.5f);
    float frac = cosine - (float)i;
    float a = GFastArcCosTable[i];
    float b = GFastArcCosTable[i + 1];
    return ((1.0f - frac) * a + frac * b) * 0.15915494f;
}
#endif
#endif
