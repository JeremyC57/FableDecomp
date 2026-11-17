// std::_Rb_tree<EObjectAugmentationType, pair<EObjectAugmentationType const, CObjectAugmentationParticleSet>, _Select1st<pair<EObjectAugmentationType const, CObjectAugmentationParticleSet> >, less<EObjectAugmentationType >, allocator<pair<EObjectAugmentationType const, CObjectAugmentationParticleSet> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x004EC441), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@W4EObjectAugmentationType@@U?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationParticleSet@@@std@@U?$_Select1st@U?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationParticleSet@@@std@@@3@U?$less@W4EObjectAugmentationType@@@3@V?$allocator@U?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationParticleSet@@@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationParticleSet@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationParticleSet@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationParticleSet@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004ECA59, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("y",off)` + `("s",on)`).
// Identity (NF58, round 7): the map<EObjectAugmentationType, CObjectAugmentationParticleSet> tree; Ego_r.pdb members of this type:
//   CObjectAugmentationsDef::AugmentationParticles. Landed callers of its insert_unique 0x004EC9D3: 0x004EC8C2
//   GFSerialiseMapBinaryIn<CObjectAugmentationParticleSet, EObjectAugmenta.
// Retail facts: signed (`jl`) key compare; _M_create_node 0x004EC441 allocates 0x40-byte nodes (`push 40h`) = 0x10 links + the 48-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_longkey_buy004ec441::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "engine/EObjectAugmentationType.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/CObjectAugmentationParticleSet.h"
namespace std {
template _Rb_tree<EObjectAugmentationType, pair<EObjectAugmentationType const, CObjectAugmentationParticleSet>, _Select1st<pair<EObjectAugmentationType const, CObjectAugmentationParticleSet> >, less<EObjectAugmentationType >, allocator<pair<EObjectAugmentationType const, CObjectAugmentationParticleSet> > >::iterator _Rb_tree<EObjectAugmentationType, pair<EObjectAugmentationType const, CObjectAugmentationParticleSet>, _Select1st<pair<EObjectAugmentationType const, CObjectAugmentationParticleSet> >, less<EObjectAugmentationType >, allocator<pair<EObjectAugmentationType const, CObjectAugmentationParticleSet> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<EObjectAugmentationType const, CObjectAugmentationParticleSet>&, _Rb_tree_node_base*);
}
