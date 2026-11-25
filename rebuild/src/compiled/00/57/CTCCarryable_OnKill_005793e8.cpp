// std::_Rb_tree<EHeroTrainableStatType, pair<const EHeroTrainableStatType, NUISystem::CComponent*>, ...>::clear() -- (map<EHeroTrainableStatType, NUISystem::CComponent*>) STLport 4.6 _tree.h clear: when the tree has nodes, erase them from the root, reset the header's leftmost / root / rightmost, zero the count
// Leaf: ?clear@?$_Rb_tree@W4EHeroTrainableStatType@@U?$pair@$$CBW4EHeroTrainableStatType@@PAVCComponent@NUISystem@@@std@@U?$_Select1st@U?$pair@$$CBW4EHeroTrainableStatType@@PAVCComponent@NUISystem@@@std@@@3@U?$less@W4EHeroTrainableStatType@@@3@V?$allocator@U?$pair@$$CBW4EHeroTrainableStatType@@PAVCComponent@NUISystem@@@std@@@3@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005793E8, 41 bytes, thiscall (this in esi from ecx), plain `ret`; size-optimised TU.
// Identity (NF391 trace, NF396 re-check): its only reference is the call at 0x005796AE in 0x005796AB (`call clear ; free(header)`), the
//   ~_Rb_tree of this tree, which the table already binds as ??1?$_Rb_tree<EHeroTrainableStatType, pair<const EHeroTrainableStatType,
//   NUISystem::CComponent*>, ...> (witness ~CTCInventoryExperience 0x0057846F, the member at +0x224 in its destroy order); a tree
//   destructor calls the tree's own clear (STLport `~_Rb_tree() { clear(); }`), never map::clear, so the caller path names the _Rb_tree
//   member. The body calls _M_erase 0x00578FF5 on the root. FableWin ~CTCInventoryExperience destroys the same
//   map<EHeroTrainableStatType, NUISystem::CComponent*> member. Both PDBs are Dinkumware builds, so the spelling is the one the repo
//   STLport emits (descriptive-by-library). Sibling: 0x00579EA3, the same TU's clear of the map<EHeroTrainableStatType, SExpSpendUndo>.
// NF396: was the free stand-in OnKill(AutoTinyOwnedList*) calling AutoTinyFreeListAllocation_00578ff5 (catalog RbTree_Clear, manifest
//   CTCCarryable::OnKill, propagated). Model: the shared stlport/fable_map.h body, as 0x00579EA3; _M_erase is declared only
//   (FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY), so the TU emits no copy of 0x00578FF5.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY
#include "stlport/fable_map.h"
#include "engine/EHeroTrainableStatType.h"
namespace NUISystem { class CComponent; }
namespace std {
template void _Rb_tree<EHeroTrainableStatType, pair<EHeroTrainableStatType const, NUISystem::CComponent*>, _Select1st<pair<EHeroTrainableStatType const, NUISystem::CComponent*> >, less<EHeroTrainableStatType >, allocator<pair<EHeroTrainableStatType const, NUISystem::CComponent*> > >::clear();
}
