// std::_Rb_tree<EObjectAugmentationType, pair<EObjectAugmentationType const, CDefString>, _Select1st<pair<EObjectAugmentationType const, CDefString> >, less<EObjectAugmentationType >, allocator<pair<EObjectAugmentationType const, CDefString> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x004EC31C), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@W4EObjectAugmentationType@@U?$pair@$$CBW4EObjectAugmentationType@@VCDefString@@@std@@U?$_Select1st@U?$pair@$$CBW4EObjectAugmentationType@@VCDefString@@@std@@@3@U?$less@W4EObjectAugmentationType@@@3@V?$allocator@U?$pair@$$CBW4EObjectAugmentationType@@VCDefString@@@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EObjectAugmentationType@@VCDefString@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EObjectAugmentationType@@VCDefString@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBW4EObjectAugmentationType@@VCDefString@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EC79D, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("y",off)` + `("s",on)`).
// Identity (NF58, round 7): the map<EObjectAugmentationType, CDefString> tree; Ego_r.pdb members of this type:
//   CObjectAugmentationsDef::InitSounds, CObjectAugmentationsDef::LoopingSounds. Landed callers of its insert_unique 0x004EC717:
//   0x004EC66C _global::GFSerialiseMapBinaryIn.
// Retail facts: signed (`jl`) key compare; _M_create_node 0x004EC31C allocates 0x18-byte nodes (`push 18h`) = 0x10 links + the 8-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_longkey_buy004ec31c::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "engine/EObjectAugmentationType.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/CDefString.h"
namespace std {
template _Rb_tree<EObjectAugmentationType, pair<EObjectAugmentationType const, CDefString>, _Select1st<pair<EObjectAugmentationType const, CDefString> >, less<EObjectAugmentationType >, allocator<pair<EObjectAugmentationType const, CDefString> > >::iterator _Rb_tree<EObjectAugmentationType, pair<EObjectAugmentationType const, CDefString>, _Select1st<pair<EObjectAugmentationType const, CDefString> >, less<EObjectAugmentationType >, allocator<pair<EObjectAugmentationType const, CDefString> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<EObjectAugmentationType const, CDefString>&, _Rb_tree_node_base*);
}
