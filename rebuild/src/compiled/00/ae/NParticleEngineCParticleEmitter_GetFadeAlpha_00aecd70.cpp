// NParticleEngine::CParticleEmitter::GetFadeAlpha(float distance) const -- the emitter's distance fade: 0 before the fade-in start or beyond the maximum draw distance, a linear ramp up to the fade-in end, 1 until the fade-out start, then a linear ramp down to 0 at the maximum draw distance
// Leaf: ?GetFadeAlpha@CParticleEmitter@NParticleEngine@@QBEMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AECD70, 142 bytes, __fastcall `ret 4`, result in st(0).
// Identity: FableWin.pdb name set `?GetFadeAlpha@CParticleEmitter@NParticleEngine@@QBEMM@Z` (const, float -> float); the only
//   caller is CParticleEmitter::IsVisible 0x00AED2F0 (landed, which calls it "CalcFadeAlpha"), passing 0 for 2D emitters
//   and DistanceToCam otherwise.
// Layout: retail CParticleEmitter = Ego_r.pdb layout minus 0x0c: MaxDrawDistance +0x15c, FadeOutStart +0x160,
//   FadeInStart +0x164, FadeInEnd +0x168. Floats 1.0 0x0122DED8, 0.0 0x0122DEDC.
// Header: engine/CParticleEmitter.h is not used: it carries the Ego_r offsets (+0x0c high: MaxDrawDistance +0x168) and is not
//   in namespace NParticleEngine, so the mangled leaf would be wrong.
// Label: catalog "IsVisible_CalcFadeAlpha" is descriptive.
// Shape: the `||` of the two out-of-range tests puts the shared `return 0.0f` at the function end.
// Layout (supersedes any local-model note above): engine/CParticleEmitter.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CParticleEmitter CParticleEmitterDonor
#include "engine/CParticleEmitter.h"
#undef CParticleEmitter
namespace NParticleEngine {
class CParticleEmitter : public CParticleEmitterDonor {
public:
    float GetFadeAlpha(float distance) const;
};

float CParticleEmitter::GetFadeAlpha(float distance) const
{
    if (distance < FadeInStart || distance > MaxDrawDistance)
        return 0.0f;
    if (distance < FadeInEnd)
        return (distance - FadeInStart) / (FadeInEnd - FadeInStart);
    if (distance < FadeOutStart)
        return 1.0f;
    return 1.0f - (distance - FadeOutStart) / (MaxDrawDistance - FadeOutStart);
}
}