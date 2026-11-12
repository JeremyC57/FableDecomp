// NUISystem::CComponent::GetParentViewport() -- returns PParentViewport
// Leaf: ?GetParentViewport@CComponent@NUISystem@@UAEPAV12@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052F170, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 99 of NUISystem::CMorphingSprite's vtable 0x0124A49C (the slot holds this body in 43 vtable(s), owner NUISystem::CComponent); the Xbox 360 Test PDB virtual order (third source) aligned on 75 TLC-named slots of that vtable puts GetParentViewport(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetParentViewport@CComponent@NUISystem@@UAEPAV12@XZ; `ret` matches the signature.
// Was: NUISystem::CSprite::vfunc_99 (rebuild/src/compiled/00/52/gapscan_sub0052f170_0052f170.cpp).
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
CComponent* CComponent::GetParentViewport()
{
    return PParentViewport;
}
}
