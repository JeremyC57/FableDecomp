// std::_Rb_tree<long, pair<long const, long>, _Select1st<pair<long const, long> >, less<long >, allocator<pair<long const, long> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x004E4ECA), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@JU?$pair@$$CBJJ@std@@U?$_Select1st@U?$pair@$$CBJJ@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJJ@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBJJ@std@@U?$_Nonconst_traits@U?$pair@$$CBJJ@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBJJ@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E5425, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("y",off)` + `("s",on)`).
// Identity (NF58, round 7): the map<long, long> tree; Ego_r.pdb members of this type:
//   CExperienceDef::PickupParticleEffectsPerExperiencePoints, CGameScriptInterface::ScriptTimers,
//   CGameScriptInterface.conflict::ScriptTimers, CTCInventoryTrade::SpecialItemsToDelete (+1 more). Landed callers of its insert_unique
//   0x004E539F: 0x005B2A37 std::_Rb_tree<long, std::pair<long const, long>, std::_Select1st<std::. Landed callers of _M_insert besides
//   0x004E539F: 0x005B2A37 std::_Rb_tree<long, std::pair<long const, long>, std::_Select1st<std::.
// Retail facts: signed (`jl`) key compare; _M_create_node 0x004E4ECA allocates 0x18-byte nodes (`push 18h`) = 0x10 links + the 8-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `std::_Rb_tree<...>::_M_insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"

namespace std {
template _Rb_tree<long, pair<long const, long>, _Select1st<pair<long const, long> >, less<long >, allocator<pair<long const, long> > >::iterator _Rb_tree<long, pair<long const, long>, _Select1st<pair<long const, long> >, less<long >, allocator<pair<long const, long> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<long const, long>&, _Rb_tree_node_base*);
}
