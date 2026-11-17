// std::_Rb_tree<EOpinionAttitudeType, pair<EOpinionAttitudeType const, float>, _Select1st<pair<EOpinionAttitudeType const, float> >, less<EOpinionAttitudeType >, allocator<pair<EOpinionAttitudeType const, float> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x004E9AB1), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@W4EOpinionAttitudeType@@U?$pair@$$CBW4EOpinionAttitudeType@@M@std@@U?$_Select1st@U?$pair@$$CBW4EOpinionAttitudeType@@M@std@@@3@U?$less@W4EOpinionAttitudeType@@@3@V?$allocator@U?$pair@$$CBW4EOpinionAttitudeType@@M@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EOpinionAttitudeType@@M@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EOpinionAttitudeType@@M@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBW4EOpinionAttitudeType@@M@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004ECEE6, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("y",off)` + `("s",on)`).
// Identity (NF58, round 7): the map<EOpinionAttitudeType, float> tree; Ego_r.pdb members of this type:
//   COpinionReactionManagerDef::ToleranceToBeingHit, CShopDef::AttitudeBuyPriceMultiplier, CShopDef::AttitudeSellPriceMultiplier.
//   Landed callers of its insert_unique 0x004ECE60: 0x004ECDAC GFSerialiseMapBinaryIn<float, EOpinionAttitudeType, std::less<EOpinion.
// Retail facts: signed (`jl`) key compare; _M_create_node 0x004E9AB1 allocates 0x18-byte nodes (`push 18h`) = 0x10 links + the 8-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_longkey_buy004e9ab1::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "engine/EOpinionAttitudeType.h"
namespace std {
template _Rb_tree<EOpinionAttitudeType, pair<EOpinionAttitudeType const, float>, _Select1st<pair<EOpinionAttitudeType const, float> >, less<EOpinionAttitudeType >, allocator<pair<EOpinionAttitudeType const, float> > >::iterator _Rb_tree<EOpinionAttitudeType, pair<EOpinionAttitudeType const, float>, _Select1st<pair<EOpinionAttitudeType const, float> >, less<EOpinionAttitudeType >, allocator<pair<EOpinionAttitudeType const, float> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<EOpinionAttitudeType const, float>&, _Rb_tree_node_base*);
}
