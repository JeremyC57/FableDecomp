// NUISystem::CComponent::GetTargetStateIndex() const -- returns 0
// Leaf: ?GetTargetStateIndex@CComponent@NUISystem@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052F130, thiscall, `ret`.
// Identity: slot 87 of NUISystem::CComponent's vtable 0x0124608C (defined here: the slot holds this body in 1 vtable(s), all of NUISystem::CComponent or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 55 TLC-named slots of that vtable puts GetTargetStateIndex(void) at this slot; Ego_r/FableWin name set: ?GetTargetStateIndex@CComponent@NUISystem@@UBEKXZ; `ret` matches the signature.
// Header: engine/NUISystem_CComponent.h (opt-ins as in the landed row NUISystemCComponent_IsPositionIndependent_0052f1a0.cpp).
#include "engine/NUISystem_CComponent.h"
unsigned long NUISystem::CComponent::GetTargetStateIndex() const
{
    return 0;
}
