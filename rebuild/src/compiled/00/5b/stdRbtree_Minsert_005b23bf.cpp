// std::_Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, NUISystem::CList*>, _Select1st<pair<NUISystem::CComponent* const, NUISystem::CList*> >, less<NUISystem::CComponent* >, allocator<pair<NUISystem::CComponent* const, NUISystem::CList*> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x005B2029), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@PAVCComponent@NUISystem@@U?$pair@QAVCComponent@NUISystem@@PAVCList@2@@std@@U?$_Select1st@U?$pair@QAVCComponent@NUISystem@@PAVCList@2@@std@@@4@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@U?$pair@QAVCComponent@NUISystem@@PAVCList@2@@std@@@4@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@QAVCComponent@NUISystem@@PAVCList@2@@std@@U?$_Nonconst_traits@U?$pair@QAVCComponent@NUISystem@@PAVCList@2@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@QAVCComponent@NUISystem@@PAVCList@2@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B23BF, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("s", on)`).
// Identity (NF58, round 7): the map<NUISystem::CComponent*, NUISystem::CList*> tree; Ego_r.pdb members of this type:
//   CTCInventoryTrade::SellListsByCategory, CTCInventoryTrade::BuyListsByCategory, CTCInventoryTrade::WantedListsByCategory. Landed
//   callers of its insert_unique 0x005B2447: 0x005B28F8 std::_Rb_tree<NUISystem::CComponent *, std::pair<NUISystem::CComponent. Landed
//   callers of _M_insert besides 0x005B2447: 0x005B28F8 std::_Rb_tree<NUISystem::CComponent *, std::pair<NUISystem::CComponent.
// Retail facts: unsigned (`jb`) key compare; _M_create_node 0x005B2029 allocates 0x18-byte nodes (`push 18h`) = 0x10 links + the 8-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_ulongkey_buy005b2029::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
namespace NUISystem { class CComponent; class CList; }
namespace std {
template _Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, NUISystem::CList*>, _Select1st<pair<NUISystem::CComponent* const, NUISystem::CList*> >, less<NUISystem::CComponent* >, allocator<pair<NUISystem::CComponent* const, NUISystem::CList*> > >::iterator _Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, NUISystem::CList*>, _Select1st<pair<NUISystem::CComponent* const, NUISystem::CList*> >, less<NUISystem::CComponent* >, allocator<pair<NUISystem::CComponent* const, NUISystem::CList*> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<NUISystem::CComponent* const, NUISystem::CList*>&, _Rb_tree_node_base*);
}
