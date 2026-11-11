// NUISystem::CChangingStateComponent::GetUpdateTime() const -- returns UpdateTime
// Leaf: ?GetUpdateTime@CChangingStateComponent@NUISystem@@UBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041C630, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 54 of NUISystem::CMorphingSprite's vtable 0x0124A49C (the slot holds this body in 42 vtable(s), owner NUISystem::CChangingStateComponent); the Xbox 360 Test PDB virtual order (third source) aligned on 75 TLC-named slots of that vtable puts GetUpdateTime(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetUpdateTime@CChangingStateComponent@NUISystem@@UBEMXZ; `ret` matches the signature.
// Was: NUISystem::CSprite::vfunc_54 (rebuild/src/compiled/00/41/gapscan_sub0041c630_0041c630.cpp).
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
float CChangingStateComponent::GetUpdateTime() const
{
    return UpdateTime;
}
}
