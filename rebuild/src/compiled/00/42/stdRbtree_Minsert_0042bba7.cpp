// std::_Rb_tree<long, pair<long const, EEngine2DLayer>, _Select1st<pair<long const, EEngine2DLayer> >, less<long >, allocator<pair<long const, EEngine2DLayer> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x0042B5EC), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@JU?$pair@$$CBJW4EEngine2DLayer@@@std@@U?$_Select1st@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBJW4EEngine2DLayer@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBJW4EEngine2DLayer@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042BBA7, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("y",off)` + `("s",on)`).
// Identity (NF58, round 7): the map<long, EEngine2DLayer> tree; Ego_r.pdb members of this type: CManager::Layers. Landed callers of its
//   insert_unique 0x0042BC2F: 0x0042C300 std::_Rb_tree<long, std::pair<long const, EEngine2DLayer>, std::_Selec. Landed callers of
//   _M_insert besides 0x0042BC2F: 0x0042C300 std::_Rb_tree<long, std::pair<long const, EEngine2DLayer>, std::_Selec.
// Retail facts: signed (`jl`) key compare; _M_create_node 0x0042B5EC allocates 0x18-byte nodes (`push 18h`) = 0x10 links + the 8-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_longkey_buy0042b5ec::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "engine/NUISystem_CManager.h"
namespace std {
template _Rb_tree<long, pair<long const, EEngine2DLayer>, _Select1st<pair<long const, EEngine2DLayer> >, less<long >, allocator<pair<long const, EEngine2DLayer> > >::iterator _Rb_tree<long, pair<long const, EEngine2DLayer>, _Select1st<pair<long const, EEngine2DLayer> >, less<long >, allocator<pair<long const, EEngine2DLayer> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<long const, EEngine2DLayer>&, _Rb_tree_node_base*);
}
