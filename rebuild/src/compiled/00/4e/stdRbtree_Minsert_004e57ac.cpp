// std::_Rb_tree<EClothingSuitPart, pair<EClothingSuitPart const, long>, _Select1st<pair<EClothingSuitPart const, long> >, less<EClothingSuitPart >, allocator<pair<EClothingSuitPart const, long> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x004E55CF), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@W4EClothingSuitPart@@U?$pair@$$CBW4EClothingSuitPart@@J@std@@U?$_Select1st@U?$pair@$$CBW4EClothingSuitPart@@J@std@@@3@U?$less@W4EClothingSuitPart@@@3@V?$allocator@U?$pair@$$CBW4EClothingSuitPart@@J@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EClothingSuitPart@@J@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EClothingSuitPart@@J@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBW4EClothingSuitPart@@J@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E57AC, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("y",off)` + `("s",on)`).
// Identity (NF58, round 7): the map<EClothingSuitPart, long> tree; Ego_r.pdb members of this type: CHeroSuitDef::SuitParts,
//   CTCInventoryClothing::CurrentClothingSelected. Landed callers of its insert_unique 0x004E5726: 0x004E5673
//   GFSerialiseMapBinaryIn<long, EClothingSuitPart, std::less<EClothingSui; 0x005BB5EF std::_Rb_tree<EClothingSuitPart,
//   std::pair<EClothingSuitPart const, lo. Landed callers of _M_insert besides 0x004E5726: 0x005BB5EF std::_Rb_tree<EClothingSuitPart,
//   std::pair<EClothingSuitPart const, lo.
// Retail facts: signed (`jl`) key compare; _M_create_node 0x004E55CF allocates 0x18-byte nodes (`push 18h`) = 0x10 links + the 8-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_longkey_buy004e55cf::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "engine/EClothingSuitPart.h"
namespace std {
template _Rb_tree<EClothingSuitPart, pair<EClothingSuitPart const, long>, _Select1st<pair<EClothingSuitPart const, long> >, less<EClothingSuitPart >, allocator<pair<EClothingSuitPart const, long> > >::iterator _Rb_tree<EClothingSuitPart, pair<EClothingSuitPart const, long>, _Select1st<pair<EClothingSuitPart const, long> >, less<EClothingSuitPart >, allocator<pair<EClothingSuitPart const, long> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<EClothingSuitPart const, long>&, _Rb_tree_node_base*);
}
