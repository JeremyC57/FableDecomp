// CTCAnimationComplex::GetInterpolatedMeshAnim(C3DAnimationBlendState&, float) const -- the interpolated mesh animation of the owned CAnimateThing (PAnimation +0x0C), tail-called
// Leaf: ?GetInterpolatedMeshAnim@CTCAnimationComplex@@UBEXAAVC3DAnimationBlendState@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070B440, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 28 of CTCAnimationComplex's vtable 0x012650A4 (the slot holds this body in 1 vtable(s), owner CTCAnimationComplex); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts GetInterpolatedMeshAnim(C3DAnimationBlendState &, float) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetInterpolatedMeshAnim@CTCAnimationComplex@@UBEXAAVC3DAnimationBlendState@@M@Z; no `ret` to check (NORET).
// Was: CTCAnimationComplex::vfunc_28 (rebuild/src/compiled/00/70/gapscan_sub0070b440_0070b440.cpp).
#define FABLE_RETAIL_CTCANIMATIONCOMPLEX_FULL
#define FABLE_RETAIL_CANIMATETHING_MESHANIMS
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCAnimationComplex.h"
void CTCAnimationComplex::GetInterpolatedMeshAnim(C3DAnimationBlendState& state, float time) const
{
    PAnimation->GetInterpolatedMeshAnim(state, time);
}
