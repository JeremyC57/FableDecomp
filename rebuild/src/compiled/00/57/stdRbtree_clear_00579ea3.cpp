// std::_Rb_tree<EHeroTrainableStatType, pair<const EHeroTrainableStatType, SExpSpendUndo>, ...>::clear() -- (map<EHeroTrainableStatType,SExpSpendUndo>) STLport 4.6 _tree.h clear: when the tree has nodes, erase them from the root, reset the header's leftmost / root / rightmost, zero the count
// Leaf: ?clear@?$_Rb_tree@W4EHeroTrainableStatType@@U?$pair@$$CBW4EHeroTrainableStatType@@USExpSpendUndo@@@std@@U?$_Select1st@U?$pair@$$CBW4EHeroTrainableStatType@@USExpSpendUndo@@@std@@@3@U?$less@W4EHeroTrainableStatType@@@3@V?$allocator@U?$pair@$$CBW4EHeroTrainableStatType@@USExpSpendUndo@@@std@@@3@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00579EA3, 41 bytes, thiscall (this in esi from ecx), plain `ret`; size-optimised TU.
// Identity: the tree's clear of CTCInventoryExperience::UndoableStatBuys (Ego_r.pdb map<EHeroTrainableStatType,SExpSpendUndo>): the
//   landed CTCInventoryExperience rows 0x00577216 / 0x0057770A / 0x005778CA / 0x00577AC0 call it (FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE)
//   under this spelling; the body is STLport's tree clear (count test, _M_erase(root) 0x00579D97, header left = header, root = 0,
//   right = header, count = 0). map::clear (`_M_t.clear();`) would give the same bytes; the tree member is the spelling every landed
//   binder already uses (descriptive-by-library: no PDB holds STL instance names).
// Name fix (NF189, 2026-11-20, q4774): was the free stand-in OnKill(AutoTinyOwnedList*) calling AutoTinyFreeListAllocation_00579d97;
//   now the shared stlport/fable_map.h body over the shared SExpSpendUndo (engine/CTCInventoryExperience.h FABLE_RETAIL_SEXPSPENDUNDO_ONLY,
//   as the landed _M_insert row 0x00579FD8). _M_erase is declared only (FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY), so the TU emits no
//   copy of 0x00579D97.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY
#include "stlport/fable_map.h"
#define FABLE_RETAIL_SEXPSPENDUNDO_ONLY
#include "engine/EHeroTrainableStatType.h"
#include "engine/CTCInventoryExperience.h"
namespace std {
template void _Rb_tree<EHeroTrainableStatType, pair<EHeroTrainableStatType const, SExpSpendUndo>, _Select1st<pair<EHeroTrainableStatType const, SExpSpendUndo> >, less<EHeroTrainableStatType >, allocator<pair<EHeroTrainableStatType const, SExpSpendUndo> > >::clear();
}
