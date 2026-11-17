// std::_Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, long>, ...>::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c:
// create the node (_M_create_node 0x0042B5A8), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance
// (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@PAVCComponent@NUISystem@@U?$pair@QAVCComponent@NUISystem@@J@std@@U?$_Select1st@U?$pair@QAVCComponent@NUISystem@@J@std@@@4@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@U?$pair@QAVCComponent@NUISystem@@J@std@@@4@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@QAVCComponent@NUISystem@@J@std@@U?$_Nonconst_traits@U?$pair@QAVCComponent@NUISystem@@J@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@QAVCComponent@NUISystem@@J@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042B98B, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("y",off)` + `("s",on)`).
// Identity (NF58, round 7): the map<NUISystem::CComponent*, long> tree (Ego_r.pdb CTCInventoryClothing::ClothingItemMap /
//   CTCInventoryWeapons::WeaponMenuItemMap, map<NUISystem::CComponent *, long>): its landed hinted insert_unique 0x0042C082 calls this
//   address at +0x35 with this instance, and its insert_unique(const value_type&) 0x0042BA13 calls it twice. The unsigned key compare
//   (`jb`, pointer key) and the 0x18-byte node with the 8-byte pair copied in _M_create_node 0x0042B5A8 (`push 18h`) agree.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_ulongkey_buy0042b5a8::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
namespace NUISystem { class CComponent; }
namespace std {
template _Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, long>, _Select1st<pair<NUISystem::CComponent* const, long> >, less<NUISystem::CComponent*>, allocator<pair<NUISystem::CComponent* const, long> > >::iterator _Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, long>, _Select1st<pair<NUISystem::CComponent* const, long> >, less<NUISystem::CComponent*>, allocator<pair<NUISystem::CComponent* const, long> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<NUISystem::CComponent* const, long>&, _Rb_tree_node_base*);
}
