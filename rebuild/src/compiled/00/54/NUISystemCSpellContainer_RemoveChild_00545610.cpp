// NUISystem::CSpellContainer::RemoveChild(unsigned long) -- forwards to CComponent::RemoveChild (tail jump 0x00533BC0)
// Leaf: ?RemoveChild@CSpellContainer@NUISystem@@UAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00545610, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 64 of NUISystem::CSpellContainer's vtable 0x0124786C (the slot holds this body in 1 vtable(s), owner NUISystem::CSpellContainer); the Xbox 360 Test PDB virtual order (third source) aligned on 68 TLC-named slots of that vtable puts RemoveChild(unsigned long) at this slot [gap kind GS]; Ego_r/FableWin name set: ?RemoveChild@CSpellContainer@NUISystem@@UAEXK@Z; no `ret` to check (NORET).
// Was: NUISystem::CSpellContainer::vfunc_64 (rebuild/src/compiled/00/54/gapscan_sub00545610_00545610.cpp).
#define FABLE_RETAIL_CSPELLCONTAINER_VSLOTS
#include "engine/NUISystem_CSpellContainer.h"
namespace NUISystem {
void CSpellContainer::RemoveChild(unsigned long index)
{
    CComponent::RemoveChild(index);
}
}
