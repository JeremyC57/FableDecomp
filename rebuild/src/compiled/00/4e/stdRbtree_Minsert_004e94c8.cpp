// std::_Rb_tree<EObjectAugmentationType, pair<EObjectAugmentationType const, long>, _Select1st<pair<EObjectAugmentationType const, long> >, less<EObjectAugmentationType >, allocator<pair<EObjectAugmentationType const, long> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x004E4D19), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@W4EObjectAugmentationType@@U?$pair@$$CBW4EObjectAugmentationType@@J@std@@U?$_Select1st@U?$pair@$$CBW4EObjectAugmentationType@@J@std@@@3@U?$less@W4EObjectAugmentationType@@@3@V?$allocator@U?$pair@$$CBW4EObjectAugmentationType@@J@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EObjectAugmentationType@@J@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EObjectAugmentationType@@J@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBW4EObjectAugmentationType@@J@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E94C8, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("y",off)` + `("s",on)`).
// Identity (NF58, round 7): the map<EObjectAugmentationType, long> tree; Ego_r.pdb members of this type: CWeaponDef::ArrowTrails.
//   Landed callers of its insert_unique 0x004E9442: 0x004E9393 GFSerialiseMapBinaryIn<long, EObjectAugmentationType, std::less<EObjec;
//   0x005C4F42 CTCInventoryWeapons::ConstructAugmentationsLeftDescription.
// Retail facts: signed (`jl`) key compare; _M_create_node 0x004E4D19 allocates 0x18-byte nodes (`push 18h`) = 0x10 links + the 8-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_longkey_buy004e4d19::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "engine/EObjectAugmentationType.h"
namespace std {
template _Rb_tree<EObjectAugmentationType, pair<EObjectAugmentationType const, long>, _Select1st<pair<EObjectAugmentationType const, long> >, less<EObjectAugmentationType >, allocator<pair<EObjectAugmentationType const, long> > >::iterator _Rb_tree<EObjectAugmentationType, pair<EObjectAugmentationType const, long>, _Select1st<pair<EObjectAugmentationType const, long> >, less<EObjectAugmentationType >, allocator<pair<EObjectAugmentationType const, long> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<EObjectAugmentationType const, long>&, _Rb_tree_node_base*);
}
