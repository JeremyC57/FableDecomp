// NUISystem::CSpellContainer::GetBaseSpells() -- returns BaseSpells
// Leaf: ?GetBaseSpells@CSpellContainer@NUISystem@@UAEAAV?$vector@V?$CCountedPointer@VCComponent@NUISystem@@@@V?$allocator@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@std@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00546070, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 148 of NUISystem::CSpellContainer's vtable 0x0124786C (the slot holds this body in 1 vtable(s), owner NUISystem::CSpellContainer); the Xbox 360 Test PDB virtual order (third source) aligned on 68 TLC-named slots of that vtable puts GetBaseSpells(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetBaseSpells@CSpellContainer@NUISystem@@UAEAAV?$vector@V?$CCountedPointer@VCComponent@NUISystem@@@@V?$allocator@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@std@@XZ; `ret` matches the signature.
// Was: NUISystem::CSpellContainer::vfunc_148 (rebuild/src/compiled/00/54/gapscan_sub00546070_00546070.cpp).
#define FABLE_RETAIL_CSPELLCONTAINER_VIRTUALS
#define FABLE_RETAIL_CSPELLCONTAINER_MEMBERS
#include "engine/NUISystem_CSpellContainer.h"
namespace NUISystem {
std::vector<CCountedPointer<CComponent> >& CSpellContainer::GetBaseSpells()
{
    return BaseSpells;
}
}
