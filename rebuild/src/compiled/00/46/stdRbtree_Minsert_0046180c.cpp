// std::_Rb_tree<EObjectAugmentationType, pair<EObjectAugmentationType const, float>, _Select1st<pair<EObjectAugmentationType const, float> >, less<EObjectAugmentationType >, allocator<pair<EObjectAugmentationType const, float> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x0046149E), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@W4EObjectAugmentationType@@U?$pair@$$CBW4EObjectAugmentationType@@M@std@@U?$_Select1st@U?$pair@$$CBW4EObjectAugmentationType@@M@std@@@3@U?$less@W4EObjectAugmentationType@@@3@V?$allocator@U?$pair@$$CBW4EObjectAugmentationType@@M@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EObjectAugmentationType@@M@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EObjectAugmentationType@@M@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBW4EObjectAugmentationType@@M@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046180C, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("y",off)` + `("s",on)`).
// Identity (NF58, round 7): the map<EObjectAugmentationType, float> tree; Ego_r.pdb members of this type:
//   CArmourDef::AugmentationResponse. Landed callers of its insert_unique 0x00461786: 0x004616D6
//   GFSerialiseMapBinaryIn<long,long,std::GFSerialiseMapBinaryIn<long,long.
// Retail facts: signed (`jl`) key compare; _M_create_node 0x0046149E allocates 0x18-byte nodes (`push 18h`) = 0x10 links + the 8-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_longkey_buy0046149e::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "engine/EObjectAugmentationType.h"
namespace std {
template _Rb_tree<EObjectAugmentationType, pair<EObjectAugmentationType const, float>, _Select1st<pair<EObjectAugmentationType const, float> >, less<EObjectAugmentationType >, allocator<pair<EObjectAugmentationType const, float> > >::iterator _Rb_tree<EObjectAugmentationType, pair<EObjectAugmentationType const, float>, _Select1st<pair<EObjectAugmentationType const, float> >, less<EObjectAugmentationType >, allocator<pair<EObjectAugmentationType const, float> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<EObjectAugmentationType const, float>&, _Rb_tree_node_base*);
}
