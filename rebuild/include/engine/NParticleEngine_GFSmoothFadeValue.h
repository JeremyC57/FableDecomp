#ifndef FABLE_ENGINE_NPARTICLEENGINE_GFSMOOTHFADEVALUE_H
#define FABLE_ENGINE_NPARTICLEENGINE_GFSMOOTHFADEVALUE_H
#ifdef FABLE_RETAIL_PARTICLE_FADE_VALUE_PASS30
#include "fable_compiler.h"
#include "engine/GFFloatToLongNear.h"
extern float GFastCosTable[1025];
namespace NParticleEngine {
float FABLE_FASTCALL GetOneOverTickRate();
// Original PDB identity. The two calls expand in 00B0A750, using the same helper body as 00AF9000.
FABLE_FORCEINLINE float FABLE_FASTCALL GFSmoothFadeValue(float t)
{
    t = t * 0.5f;
    t = t * 1024.0f;
    long i = GFFloatToLongNear(t - 0.5f);
    float frac = t - (float)i;
    i &= 0x3ff;
    return 0.5f - ((1.0f - frac) * GFastCosTable[i] + frac * GFastCosTable[i + 1]) * 0.5f;
}
}
#endif
#endif
