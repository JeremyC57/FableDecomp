// NUISystem::CManager::ClearBastardChild() -- release every component of the bastard-child map (and the current bastard child), then clear the map
// Leaf: ?ClearBastardChild@CManager@NUISystem@@UAEXXZ
// Retail 0x0041DED2, 62 bytes (vtable 0x01230134 slot 37, its only reference, .rdata 0x012301C8). Callees:
// CCountedPointer<CComponent>::ReleaseData 0x004291DE (on each node value at +0x14 and on BastardChild +0x80), STLport
// _Rb_global<bool>::_M_increment 0x004292C0 (cdecl), the map's tree clear 0x0042C02F (tail jump). The end node is re-read every
// iteration (member). Name: Ego_r.pdb / FableWin.pdb / Ego_d.pdb ?ClearBastardChild@CManager@NUISystem@@UAEXXZ (public virtual).
// Name fix (agentNF 2026-11-10): 0x004291DE is CCountedPointer<NUISystem::CComponent>::ReleaseData (the destructor is the 5-byte
//   `jmp` COMDAT 0x00429C53; Ego_r.pdb ??1 is 5 bytes): FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (engine/CCountedPointer.h).
// Port (NF227, 2026-11-22, q12582 / q12583): onto engine/NUISystem_CManager.h (retail layout: BastardChild +0x80, BastardChildren +0x88,
//   the header's STLport map<CCharString,CCountedPointer<CComponent> >). Was a row-local CManager / CBastardChildMap / CRbNodeBase model
//   whose non-virtual leaf was ?ClearBastardChild@CManager@NUISystem@@QAEXXZ, while the header vtable COMDAT of 0x0041E641 / 0x0041E3F6
//   binds the slot as UAE. The map clear is the tree's clear, called out of line (FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE; map::clear
//   is the inline `_M_t.clear()` wrapper).
// Pragma below the includes: the header inlines are defined in the default /O2 mode, so the pick-any _Rb_tree_base_iterator::
//   operator!= copy equals its owner 0x0041C830 (22 B; above the include it is the 18-B size body 0x0041E6D3 emits).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
// CManager TU is size-optimised; the pragmas follow the includes (EXTRA_RULES 2026-11-20, nf189 0x005D66BA placement).
#pragma optimize("s", on)
namespace NUISystem {
void CManager::ClearBastardChild()
{
    for (std::map<CCharString,CCountedPointer<CComponent> >::iterator it = BastardChildren.begin(); it != BastardChildren.end(); ++it) {
        (*it).second.ReleaseData();
        BastardChild.ReleaseData();
    }
    BastardChildren.clear();
}
}
