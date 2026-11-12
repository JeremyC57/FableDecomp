// NUISystem::CComponent::GetCurrentStateIndex() -- returns 0
// Leaf: ?GetCurrentStateIndex@CComponent@NUISystem@@UAEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052F120, thiscall, `ret`.
// Identity: slot 86 of NUISystem::CComponent's vtable 0x0124608C (defined here: the slot holds this body in 1 vtable(s), all of NUISystem::CComponent or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 55 TLC-named slots of that vtable puts GetCurrentStateIndex(void) at this slot; Ego_r/FableWin name set: ?GetCurrentStateIndex@CComponent@NUISystem@@UAEKXZ; `ret` matches the signature.
// Header: engine/NUISystem_CComponent.h (opt-ins as in the landed row NUISystemCComponent_IsPositionIndependent_0052f1a0.cpp).
#include "engine/NUISystem_CComponent.h"
unsigned long NUISystem::CComponent::GetCurrentStateIndex()
{
    return 0;
}
