// NUISystem::CSpellContainerList::SetSpellToAssign(long) -- stores SpellToAssign
// Leaf: ?SetSpellToAssign@CSpellContainerList@NUISystem@@UAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00544100, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 144 of NUISystem::CSpellContainerList's vtable 0x012475C4 (the slot holds this body in 1 vtable(s), owner NUISystem::CSpellContainerList); the Xbox 360 Test PDB virtual order (third source) aligned on 69 TLC-named slots of that vtable puts SetSpellToAssign(long) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetSpellToAssign@CSpellContainerList@NUISystem@@UAEXJ@Z; `ret` matches the signature.
// Was: NUISystem::CSpellContainerList::vfunc_144 (rebuild/src/compiled/00/54/gapscan_sub00544100_00544100.cpp).
#define FABLE_RETAIL_CSPELLCONTAINERLIST_VIRTUALS
#include "engine/NUISystem_CSpellContainerList.h"
namespace NUISystem {
void CSpellContainerList::SetSpellToAssign(long spell)
{
    SpellToAssign = spell;
}
}
