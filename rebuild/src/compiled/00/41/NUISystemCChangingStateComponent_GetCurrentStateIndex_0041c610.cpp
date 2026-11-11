// NUISystem::CChangingStateComponent::GetCurrentStateIndex() -- returns CurrentState
// Leaf: ?GetCurrentStateIndex@CChangingStateComponent@NUISystem@@UAEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041C610, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 86 of NUISystem::CMorphingSprite's vtable 0x0124A49C (the slot holds this body in 42 vtable(s), owner NUISystem::CChangingStateComponent); the Xbox 360 Test PDB virtual order (third source) aligned on 75 TLC-named slots of that vtable puts GetCurrentStateIndex(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetCurrentStateIndex@CChangingStateComponent@NUISystem@@UAEKXZ; `ret` matches the signature.
// Was: NUISystem::CSprite::vfunc_86 (rebuild/src/compiled/00/41/gapscan_sub0041c610_0041c610.cpp).
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
unsigned long CChangingStateComponent::GetCurrentStateIndex()
{
    return CurrentState;
}
}
