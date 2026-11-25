// std::_Rb_tree<EHeroTrainableStatType, pair<const EHeroTrainableStatType, NUISystem::CComponent*>, _Select1st<..>, less<EHeroTrainableStatType>, allocator<..> >::_M_erase(x)
//   -- STLport 4.6 _tree.c: erase the subtree under x without rebalancing (recurse right, walk left, free each node) (map<EHeroTrainableStatType, NUISystem::CComponent*>)
// Leaf: ?_M_erase@?$_Rb_tree@W4EHeroTrainableStatType@@U?$pair@$$CBW4EHeroTrainableStatType@@PAVCComponent@NUISystem@@@std@@U?$_Select1st@U?$pair@$$CBW4EHeroTrainableStatType@@PAVCComponent@NUISystem@@@std@@@3@U?$less@W4EHeroTrainableStatType@@@3@V?$allocator@U?$pair@$$CBW4EHeroTrainableStatType@@PAVCComponent@NUISystem@@@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBW4EHeroTrainableStatType@@PAVCComponent@NUISystem@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00578FF5, 45 bytes, thiscall (node on the stack, `ret 4`); size-optimised TU.
// Identity (NF399, T3 nf396): referenced only by its own recursion (call 0x00579007) and by the call at 0x005793F6 in the tree's clear
//   0x005793E8 (landed nf396_b2 as _Rb_tree<EHeroTrainableStatType, pair<const EHeroTrainableStatType, NUISystem::CComponent*>, ..>::clear;
//   the table binds this exact spelling at 0x00578FF5, witness 0x005793E8). That clear's only caller is the tree destructor 0x005796AB,
//   which ~CTCInventoryExperience 0x0057846F calls for CTCInventoryExperience::UpgradableStats (+0x224, Ego_r.pdb
//   map<EHeroTrainableStatType, NUISystem::CComponent*>). STLport 4.6 declares _M_erase private (AAE). Both PDBs are Dinkumware builds,
//   so the spelling is the one the repo STLport emits (descriptive-by-library). Sibling: the same TU's _M_erase of the
//   map<EHeroTrainableStatType, SExpSpendUndo> tree.
// NF399: was the stand-in Tree::BuildTreeArray(Node*) (catalog LTextTreeWalkThrough::BuildTreeArray, a label propagated across the
//   45-byte family of template 0x0042A1B6). Model: explicit instantiation of the stlport/fable_map.h member (the 0x00527950 form).
#pragma optimize("s", on)
#include "stlport/fable_map.h"
#include "engine/EHeroTrainableStatType.h"
namespace NUISystem { class CComponent; }
namespace std {
template void _Rb_tree<EHeroTrainableStatType, pair<EHeroTrainableStatType const, NUISystem::CComponent*>, _Select1st<pair<EHeroTrainableStatType const, NUISystem::CComponent*> >, less<EHeroTrainableStatType >, allocator<pair<EHeroTrainableStatType const, NUISystem::CComponent*> > >::_M_erase(_Rb_tree_node<pair<EHeroTrainableStatType const, NUISystem::CComponent*> >*);
}
