// std::_Rb_tree<unsigned long, pair<unsigned long const, long>, _Select1st<pair<unsigned long const, long> >, less<unsigned long >, allocator<pair<unsigned long const, long> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x0045C8A1), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@KU?$pair@$$CBKJ@std@@U?$_Select1st@U?$pair@$$CBKJ@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKJ@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBKJ@std@@U?$_Nonconst_traits@U?$pair@$$CBKJ@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBKJ@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045CA8F, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("y",off)` + `("s",on)`).
// Identity (NF58, round 7): the map<unsigned long, long> tree; Ego_r.pdb members of this type:
//   CSpecialEffectsStringMap::ContainmentMap, CTCInventoryStats::CreaturesKilled, CVertexGroupBlock::GroupToIndexMap. Landed callers of
//   its insert_unique 0x0045CA09: 0x0045C95A GFSerialiseMapBinaryIn<long, unsigned long, std::less<unsigned long>>; 0x005CDCCB
//   CTCInventoryStats::FrameUpdate; 0x005D87B6 std::_Rb_tree<unsigned long, std::pair<unsigned long const, long>, std; 0x005D8978
//   CPersistContext::TransferMap<long,unsigned long>; 0x00AD91F0 CVertexGroupBlock::AddGroup. Landed callers of _M_insert besides
//   0x0045CA09: 0x005D87B6 std::_Rb_tree<unsigned long, std::pair<unsigned long const, long>, std.
// Retail facts: unsigned (`jb`) key compare; _M_create_node 0x0045C8A1 allocates 0x18-byte nodes (`push 18h`) = 0x10 links + the 8-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_ulongkey_buy0045c8a1::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"

namespace std {
template _Rb_tree<unsigned long, pair<unsigned long const, long>, _Select1st<pair<unsigned long const, long> >, less<unsigned long >, allocator<pair<unsigned long const, long> > >::iterator _Rb_tree<unsigned long, pair<unsigned long const, long>, _Select1st<pair<unsigned long const, long> >, less<unsigned long >, allocator<pair<unsigned long const, long> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<unsigned long const, long>&, _Rb_tree_node_base*);
}
