// CTCAnimationComplex::GetMeshAnims(C3DAnimationBlendState&, C3DAnimationBlendState&) const -- the mesh animations of the owned CAnimateThing (PAnimation +0x0C), tail-called
// Leaf: ?GetMeshAnims@CTCAnimationComplex@@UBEXAAVC3DAnimationBlendState@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070B450, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 29 of CTCAnimationComplex's vtable 0x012650A4 (the slot holds this body in 1 vtable(s), owner CTCAnimationComplex); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts GetMeshAnims(C3DAnimationBlendState &, C3DAnimationBlendState &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetMeshAnims@CTCAnimationComplex@@UBEXAAVC3DAnimationBlendState@@0@Z; no `ret` to check (NORET).
// Was: CTCAnimationComplex::vfunc_29 (rebuild/src/compiled/00/70/gapscan_sub0070b450_0070b450.cpp).
#define FABLE_RETAIL_CTCANIMATIONCOMPLEX_FULL
#define FABLE_RETAIL_CANIMATETHING_MESHANIMS
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCAnimationComplex.h"
void CTCAnimationComplex::GetMeshAnims(C3DAnimationBlendState& state, C3DAnimationBlendState& blend) const
{
    PAnimation->GetMeshAnims(state, blend);
}
