// std::_Rb_tree<EHeroAbility, pair<EHeroAbility const, SExpSpendUndo>, _Select1st<pair<EHeroAbility const, SExpSpendUndo> >, less<EHeroAbility >, allocator<pair<EHeroAbility const, SExpSpendUndo> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x00579F5F), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@W4EHeroAbility@@U?$pair@$$CBW4EHeroAbility@@USExpSpendUndo@@@std@@U?$_Select1st@U?$pair@$$CBW4EHeroAbility@@USExpSpendUndo@@@std@@@3@U?$less@W4EHeroAbility@@@3@V?$allocator@U?$pair@$$CBW4EHeroAbility@@USExpSpendUndo@@@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EHeroAbility@@USExpSpendUndo@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EHeroAbility@@USExpSpendUndo@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBW4EHeroAbility@@USExpSpendUndo@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057A060, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("y",off)` + `("s",on)`).
// Identity (NF58, round 7): the map<EHeroAbility, SExpSpendUndo> tree; Ego_r.pdb members of this type:
//   CTCInventoryExperience::UndoableAbilityBuys. Landed callers of its insert_unique 0x0057A1AA: 0x005785B3
//   CTCInventoryExperience::BuyAbility.
// Retail facts: signed (`jl`) key compare; _M_create_node 0x00579F5F allocates 0x48-byte nodes (`push 48h`) = 0x10 links + the 56-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_longkey_buy00579f5f::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#define FABLE_RETAIL_SEXPSPENDUNDO_ONLY
#include "engine/EHeroAbility.h"
#include "engine/CTCInventoryExperience.h"
namespace std {
template _Rb_tree<EHeroAbility, pair<EHeroAbility const, SExpSpendUndo>, _Select1st<pair<EHeroAbility const, SExpSpendUndo> >, less<EHeroAbility >, allocator<pair<EHeroAbility const, SExpSpendUndo> > >::iterator _Rb_tree<EHeroAbility, pair<EHeroAbility const, SExpSpendUndo>, _Select1st<pair<EHeroAbility const, SExpSpendUndo> >, less<EHeroAbility >, allocator<pair<EHeroAbility const, SExpSpendUndo> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<EHeroAbility const, SExpSpendUndo>&, _Rb_tree_node_base*);
}
