// std::_Rb_tree<long, pair<long const, CCharString>, _Select1st<pair<long const, CCharString> >, less<long >, allocator<pair<long const, CCharString> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x00466255), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@JU?$pair@$$CBJVCCharString@@@std@@U?$_Select1st@U?$pair@$$CBJVCCharString@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCCharString@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBJVCCharString@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJVCCharString@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBJVCCharString@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00472A05, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("y",off)` + `("s",on)`).
// Identity (NF58, round 7): the map<long, CCharString> tree; Ego_r.pdb members of this type: CCreatureActionBase::CutSceneEvents,
//   CGlobalDef::ConversationCameras, CProfileManager::IDToProfileName. Landed callers of its insert_unique 0x0047297F: 0x00472896
//   _global::GFSerialiseMapBinaryIn.
// Retail facts: signed (`jl`) key compare; _M_create_node 0x00466255 allocates 0x18-byte nodes (`push 18h`) = 0x10 links + the 8-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_longkey_buy00466255::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
namespace std {
template _Rb_tree<long, pair<long const, CCharString>, _Select1st<pair<long const, CCharString> >, less<long >, allocator<pair<long const, CCharString> > >::iterator _Rb_tree<long, pair<long const, CCharString>, _Select1st<pair<long const, CCharString> >, less<long >, allocator<pair<long const, CCharString> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<long const, CCharString>&, _Rb_tree_node_base*);
}
