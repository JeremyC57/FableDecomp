// std::_Rb_tree<unsigned long, pair<const unsigned long, NUISystem::CUIState>, ...>::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create
// the node (_M_create_node 0x0042BDF0), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance
// (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@KU?$pair@$$CBKVCUIState@NUISystem@@@std@@U?$_Select1st@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@U?$_Nonconst_traits@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBKVCUIState@NUISystem@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042CB08, thiscall, frame-pointer size TU.
// Identity (NF42, round 5): the map<unsigned long, NUISystem::CUIState> tree (see 0x0042CEE3); callers insert_unique 0x0042CEE3 and
// 0x0042D2C6. STLport 4.6.2 _tree.c (private member), descriptive-by-library. Was the placeholder
// `CRetailTree_ulongkey_buy0042bdf0::Insert`; now an explicit instantiation of stlport/fable_map.h
// FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/NUISystem_CUIState.h"
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
namespace std {
template _Rb_tree<unsigned long, pair<const unsigned long, NUISystem::CUIState>, _Select1st<pair<const unsigned long, NUISystem::CUIState> >, less<unsigned long>, allocator<pair<const unsigned long, NUISystem::CUIState> > >::iterator _Rb_tree<unsigned long, pair<const unsigned long, NUISystem::CUIState>, _Select1st<pair<const unsigned long, NUISystem::CUIState> >, less<unsigned long>, allocator<pair<const unsigned long, NUISystem::CUIState> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<const unsigned long, NUISystem::CUIState>&, _Rb_tree_node_base*);
}
