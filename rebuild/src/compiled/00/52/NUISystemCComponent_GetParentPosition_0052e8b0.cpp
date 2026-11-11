// NUISystem::CComponent::GetParentPosition() const -- returns ParentPosition
// Leaf: ?GetParentPosition@CComponent@NUISystem@@UBE?AVC2DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052E8B0, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 17 of NUISystem::CMorphingSprite's vtable 0x0124A49C (the slot holds this body in 43 vtable(s), owner NUISystem::CComponent); the Xbox 360 Test PDB virtual order (third source) aligned on 75 TLC-named slots of that vtable puts GetParentPosition(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetParentPosition@CComponent@NUISystem@@UBE?AVC2DVector@@XZ; `ret` matches the signature.
// Was: NUISystem::CSprite::vfunc_17 (rebuild/src/compiled/00/52/NUISystemCSprite_vfunc17_0052e8b0.cpp).
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
C2DVector CComponent::GetParentPosition() const
{
    return ParentPosition;
}
}
