#ifndef FABLE_ENGINE_GFISNONZERO_H
#define FABLE_ENGINE_GFISNONZERO_H
#include "engine/GFFloatAbs.h"
// FableWin.pdb / Ego_d.pdb ?GFIsNonZero@@YI_NM@Z (FableWin 0x02F50800), inline in retail: GFFloatAbs(n) against
// ?DELTA_NOTIONAL_ZERO@@3MB 0x0129BA3C (1e-4f) (`fld [n] ; fcomp [129BA3Ch] ; fnstsw ax ; test ah,41h`; e.g. 0x00B1647C in
// NParticleEngine::CPSCOrbit::COrbitState::Update, whose Ego_d call lines are 26 bytes = one bool call and a test).
// Ego_d.pdb: parameter `n`, line table 330..332 = 4 / 46 / 6 bytes (one return statement).
extern const float DELTA_NOTIONAL_ZERO;                 // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB (1e-4f)
inline bool FABLE_FASTCALL GFIsNonZero(float n)
{
    return GFFloatAbs(n) > DELTA_NOTIONAL_ZERO;
}
#endif
