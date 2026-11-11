// NUISystem::CSpellContainer::SetEmptySlotDef(unsigned long) -- stores EmptySlot
// Leaf: ?SetEmptySlotDef@CSpellContainer@NUISystem@@UAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00546060, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 145 of NUISystem::CSpellContainer's vtable 0x0124786C (the slot holds this body in 1 vtable(s), owner NUISystem::CSpellContainer); the Xbox 360 Test PDB virtual order (third source) aligned on 68 TLC-named slots of that vtable puts SetEmptySlotDef(unsigned long) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetEmptySlotDef@CSpellContainer@NUISystem@@UAEXK@Z; `ret` matches the signature.
// Was: NUISystem::CSpellContainer::vfunc_145 (rebuild/src/compiled/00/54/gapscan_sub00546060_00546060.cpp).
#define FABLE_RETAIL_CSPELLCONTAINER_VIRTUALS
#define FABLE_RETAIL_CSPELLCONTAINER_MEMBERS
#include "engine/NUISystem_CSpellContainer.h"
namespace NUISystem {
void CSpellContainer::SetEmptySlotDef(unsigned long def)
{
    EmptySlot = def;
}
}
