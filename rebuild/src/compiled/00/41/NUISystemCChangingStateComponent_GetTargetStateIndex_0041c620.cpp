// NUISystem::CChangingStateComponent::GetTargetStateIndex() const -- returns TargetState
// Leaf: ?GetTargetStateIndex@CChangingStateComponent@NUISystem@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041C620, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 87 of NUISystem::CMorphingSprite's vtable 0x0124A49C (the slot holds this body in 42 vtable(s), owner NUISystem::CChangingStateComponent); the Xbox 360 Test PDB virtual order (third source) aligned on 75 TLC-named slots of that vtable puts GetTargetStateIndex(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetTargetStateIndex@CChangingStateComponent@NUISystem@@UBEKXZ; `ret` matches the signature.
// Was: NUISystem::CSprite::vfunc_87 (rebuild/src/compiled/00/41/gapscan_sub0041c620_0041c620.cpp).
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
unsigned long CChangingStateComponent::GetTargetStateIndex() const
{
    return TargetState;
}
}
