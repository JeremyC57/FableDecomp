// NUISystem::CMenuEntry::RemoveChild(unsigned long) -- forwards to CComponent::RemoveChild (tail jump 0x00533BC0)
// Leaf: ?RemoveChild@CMenuEntry@NUISystem@@UAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053D120, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 64 of NUISystem::CMenuEntry's vtable 0x0124662C (the slot holds this body in 1 vtable(s), owner NUISystem::CMenuEntry); the Xbox 360 Test PDB virtual order (third source) aligned on 76 TLC-named slots of that vtable puts RemoveChild(unsigned long) at this slot [gap kind GS]; Ego_r/FableWin name set: ?RemoveChild@CMenuEntry@NUISystem@@UAEXK@Z; no `ret` to check (NORET).
// Was: NUISystem::CMenuEntry::vfunc_64 (rebuild/src/compiled/00/53/gapscan_sub0053d120_0053d120.cpp).
#define FABLE_RETAIL_CMENUENTRY_VSLOTS
#include "engine/NUISystem_CMenuEntry.h"
namespace NUISystem {
void CMenuEntry::RemoveChild(unsigned long index)
{
    CComponent::RemoveChild(index);
}
}
