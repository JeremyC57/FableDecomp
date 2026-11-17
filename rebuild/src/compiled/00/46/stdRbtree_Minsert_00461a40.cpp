// std::_Rb_tree<EDamageAttribute, pair<EDamageAttribute const, float>, _Select1st<pair<EDamageAttribute const, float> >, less<EDamageAttribute >, allocator<pair<EDamageAttribute const, float> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x004615C3), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@W4EDamageAttribute@@U?$pair@$$CBW4EDamageAttribute@@M@std@@U?$_Select1st@U?$pair@$$CBW4EDamageAttribute@@M@std@@@3@U?$less@W4EDamageAttribute@@@3@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@M@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EDamageAttribute@@M@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EDamageAttribute@@M@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBW4EDamageAttribute@@M@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00461A40, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("y",off)` + `("s",on)`).
// Identity (NF58, round 7): the map<EDamageAttribute, float> tree; Ego_r.pdb members of this type: CArmourDef::DamageTypeResponse,
//   CArmourDef::ArmourThresholds, CComboMultiplierData::ComboMultiplier, CHitParameters::DamageMap. Landed callers of its insert_unique
//   0x004619BA: 0x0046190A GFSerialiseMapBinaryIn<long,long,std::GFSerialiseMapBinaryIn<long,long; 0x006DBE00
//   CPersistTraits<CComboMultiplierData>::TransferBinaryIn; 0x00868CA0 CHitParameters::AddDamageWithAttribute.
// Retail facts: signed (`jl`) key compare; _M_create_node 0x004615C3 allocates 0x18-byte nodes (`push 18h`) = 0x10 links + the 8-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_longkey_buy004615c3::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "engine/EDamageAttribute.h"
namespace std {
template _Rb_tree<EDamageAttribute, pair<EDamageAttribute const, float>, _Select1st<pair<EDamageAttribute const, float> >, less<EDamageAttribute >, allocator<pair<EDamageAttribute const, float> > >::iterator _Rb_tree<EDamageAttribute, pair<EDamageAttribute const, float>, _Select1st<pair<EDamageAttribute const, float> >, less<EDamageAttribute >, allocator<pair<EDamageAttribute const, float> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<EDamageAttribute const, float>&, _Rb_tree_node_base*);
}
