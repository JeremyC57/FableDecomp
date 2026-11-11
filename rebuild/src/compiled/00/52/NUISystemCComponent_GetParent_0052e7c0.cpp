// NUISystem::CComponent::GetParent() const -- returns PParent
// Leaf: ?GetParent@CComponent@NUISystem@@UBEPAV12@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052E7C0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 52 of NUISystem::CMorphingSprite's vtable 0x0124A49C (the slot holds this body in 43 vtable(s), owner NUISystem::CComponent); the Xbox 360 Test PDB virtual order (third source) aligned on 75 TLC-named slots of that vtable puts GetParent(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetParent@CComponent@NUISystem@@UBEPAV12@XZ; `ret` matches the signature.
// Was: NUISystem::CSprite::vfunc_52 (rebuild/src/compiled/00/52/gapscan_sub0052e7c0_0052e7c0.cpp).
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
CComponent* CComponent::GetParent() const
{
    return PParent;
}
}
