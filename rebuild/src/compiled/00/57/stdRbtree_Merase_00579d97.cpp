// std::_Rb_tree<EHeroTrainableStatType, pair<const EHeroTrainableStatType, SExpSpendUndo>, ...>::_M_erase(_Rb_tree_node*) -- STLport 4.6
//   recursive subtree erase: erase the right subtree, destroy the node's value (the pair's scalar deleting destructor 0x005799DE with
//   flags 0), free the node (free 0x00BFEA14), continue with the left child
// Leaf: ?_M_erase@?$_Rb_tree@W4EHeroTrainableStatType@@U?$pair@$$CBW4EHeroTrainableStatType@@USExpSpendUndo@@@std@@U?$_Select1st@U?$pair@$$CBW4EHeroTrainableStatType@@USExpSpendUndo@@@std@@@3@U?$less@W4EHeroTrainableStatType@@@3@V?$allocator@U?$pair@$$CBW4EHeroTrainableStatType@@USExpSpendUndo@@@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBW4EHeroTrainableStatType@@USExpSpendUndo@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00579D97, 55 bytes (size TU, `ret 4`).
// Name fix (NF202, 2026-11-20, q12235): the landed binder 0x00579EA3 (_Rb_tree<EHeroTrainableStatType, ...>::clear, NF189) calls this
//   address as the STLport _M_erase of that tree (the clear's header-root argument), and the body is STLport's self-recursive
//   _M_erase; the junk LTextTreeWalkThrough::BuildTreeArray name (and its CTextTreeNode / CInner stand-ins) is replaced by
//   stlport/fable_map.h + the SExpSpendUndo value type of engine/CTCInventoryExperience.h (as 0x00579EA3), explicit instantiation.
//   STL instance names are in no PDB name set: descriptive-by-library (the repo STLport header spelling).
#pragma optimize("s", on)
#include "stlport/fable_map.h"
#define FABLE_RETAIL_SEXPSPENDUNDO_ONLY
#include "engine/EHeroTrainableStatType.h"
#include "engine/CTCInventoryExperience.h"
namespace std {
template void _Rb_tree<EHeroTrainableStatType, pair<EHeroTrainableStatType const, SExpSpendUndo>, _Select1st<pair<EHeroTrainableStatType const, SExpSpendUndo> >, less<EHeroTrainableStatType >, allocator<pair<EHeroTrainableStatType const, SExpSpendUndo> > >::_M_erase(_Rb_tree_node<pair<EHeroTrainableStatType const, SExpSpendUndo> >*);
}
