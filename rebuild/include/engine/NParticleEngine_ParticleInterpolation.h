#ifndef FABLE_ENGINE_PARTICLE_INTERPOLATION_H
#define FABLE_ENGINE_PARTICLE_INTERPOLATION_H
#ifdef FABLE_RETAIL_CPSCATTRACTOR_INTERPOLATE_PASS30
#include "engine/C3DVector.h"
#include "engine/GFFloatToLongNear.h"
namespace NParticleEngine {
inline bool FABLE_FASTCALL GFInterpolate(bool from, bool to, float interpolate)
{
    return interpolate > 0.5f ? to : from;
}
inline unsigned long FABLE_FASTCALL GFInterpolateVectorParams(unsigned long from, unsigned long to, float interpolate)
{
    return interpolate > 0.5f ? to : from;
}
inline unsigned long FABLE_FASTCALL GFInterpolateUpdateParams(unsigned long from, unsigned long to, float interpolate)
{
    return interpolate > 0.5f ? to : from;
}
inline float FABLE_FASTCALL GFInterpolate(float from, float to, float interpolate)
{
    return from * (1.0f - interpolate) + to * interpolate;
}
inline long FABLE_FASTCALL GFInterpolate(long from, long to, float interpolate)
{
    return GFFloatToLongNear(from * (1.0f - interpolate) + to * interpolate);
}
inline C3DVector FABLE_FASTCALL GFInterpolate(const C3DVector& from, const C3DVector& to, float interpolate)
{
    return from * (1.0f - interpolate) + to * interpolate;
}
}
#endif
#endif
