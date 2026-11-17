// std::_Rb_tree<unsigned long, pair<unsigned long const, unsigned long>, _Select1st<pair<unsigned long const, unsigned long> >, less<unsigned long >, allocator<pair<unsigned long const, unsigned long> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x00431471), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@KU?$pair@$$CBKK@std@@U?$_Select1st@U?$pair@$$CBKK@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBKK@std@@U?$_Nonconst_traits@U?$pair@$$CBKK@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBKK@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004313E9, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("y",off)` + `("s",on)`).
// Identity (NF58, round 7): the map<unsigned long, unsigned long> tree; Ego_r.pdb members of this type:
//   CEngineScenePrimitiveStatsEnumerator::Destination, CMorphingSprite::GraphicIndices, CUIDef::ActionMapAliases,
//   CUIMiscThingsDef::MapPaths (+1 more). Landed callers of its insert_unique 0x00431363: 0x004312B4 GFSerialiseMapBinaryIn<unsigned
//   long, unsigned long, std::less<unsigne; 0x005537F0 std::_Rb_tree<unsigned long, std::pair<unsigned long const, unsigned l;
//   0x009CDA70 CBankFile::InternalWriteEntryList. Landed callers of _M_insert besides 0x00431363: 0x005537F0 std::_Rb_tree<unsigned
//   long, std::pair<unsigned long const, unsigned l.
// Retail facts: unsigned (`jb`) key compare; _M_create_node 0x00431471 allocates 0x18-byte nodes (`push 18h`) = 0x10 links + the 8-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_ulongkey_buy00431471::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"

namespace std {
template _Rb_tree<unsigned long, pair<unsigned long const, unsigned long>, _Select1st<pair<unsigned long const, unsigned long> >, less<unsigned long >, allocator<pair<unsigned long const, unsigned long> > >::iterator _Rb_tree<unsigned long, pair<unsigned long const, unsigned long>, _Select1st<pair<unsigned long const, unsigned long> >, less<unsigned long >, allocator<pair<unsigned long const, unsigned long> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<unsigned long const, unsigned long>&, _Rb_tree_node_base*);
}
