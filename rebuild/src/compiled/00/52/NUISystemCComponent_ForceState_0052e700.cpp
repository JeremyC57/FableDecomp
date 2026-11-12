// NUISystem::CComponent::ForceState(unsigned long, bool) -- does nothing
// Leaf: ?ForceState@CComponent@NUISystem@@UAEXK_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052E700, thiscall, `ret 8`.
// Identity: slot 42 of NUISystem::CComponent's vtable 0x0124608C (defined here: the slot holds this body in 1 vtable(s), all of NUISystem::CComponent or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 55 TLC-named slots of that vtable puts ForceState(unsigned long, bool) at this slot; Ego_r/FableWin name set: ?ForceState@CComponent@NUISystem@@UAEXK_N@Z; `ret` matches the signature.
// Header: engine/NUISystem_CComponent.h (opt-ins as in the landed row NUISystemCComponent_IsPositionIndependent_0052f1a0.cpp).
#include "engine/NUISystem_CComponent.h"
void NUISystem::CComponent::ForceState(unsigned long, bool)
{
}
