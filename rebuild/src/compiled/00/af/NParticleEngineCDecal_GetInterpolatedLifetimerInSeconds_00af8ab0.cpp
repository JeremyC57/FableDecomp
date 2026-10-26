// NParticleEngine::CDecal::GetInterpolatedLifetimerInSeconds(float fraction) const -- the decal's age in seconds: (Lifetimer + fraction) times the global particle update period
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF8AB0, 21 bytes, `ret 4`.
// Leaf: ?GetInterpolatedLifetimerInSeconds@CDecal
// Name: FableWin.pdb ?GetInterpolatedLifetimerInSeconds@CDecal@NParticleEngine@@QBEMM@Z; its sibling just before it, 0x00AF8AA0
//       (`fild [ecx+28h] ; fadd [esp+4]`), is CDecal::GetInterpolatedLifetimer, and the CParticle pair is landed
//       (GetInterpolatedLifetimerInSeconds 0x00AF85E0 multiplies the same getter in front). The catalog label
//       "NCreatureAI::dynamic_initializer_for_COS_OF_THE_ANGLE_NEEDED_TO_FACE_GUARD" is propagated and wrong.
// Callees: the particle update-period getter @ 0x00AEAA00 (`fld [0x01399508] ; ret`; declared as the landed CParticle row does).
// Layout: engine/CDecal.h (Ego_r.pdb): Lifetimer (long) +0x28.
// Shape: the getter runs first (esi keeps `this` across it), its result stays in st(0); `fild Lifetimer ; fadd fraction ; fmulp`.
#include "engine/CDecal.h"

float GetParticleUpdatePeriod();                     // 0x00AEAA00

namespace NParticleEngine {
class CDecal : public ::CDecal { public:
    float GetInterpolatedLifetimerInSeconds(float fraction) const;
};
float CDecal::GetInterpolatedLifetimerInSeconds(float fraction) const
{
    return GetParticleUpdatePeriod() * (Lifetimer + fraction);
}
}