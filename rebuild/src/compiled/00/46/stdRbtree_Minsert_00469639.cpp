// std::_Rb_tree<long, pair<long const, CBlendedParticleEffectSet>, _Select1st<pair<long const, CBlendedParticleEffectSet> >, less<long >, allocator<pair<long const, CBlendedParticleEffectSet> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x0045C2A2), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@JU?$pair@$$CBJVCBlendedParticleEffectSet@@@std@@U?$_Select1st@U?$pair@$$CBJVCBlendedParticleEffectSet@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCBlendedParticleEffectSet@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBJVCBlendedParticleEffectSet@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJVCBlendedParticleEffectSet@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBJVCBlendedParticleEffectSet@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00469639, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("y",off)` + `("s",on)`).
// Identity (NF58, round 7): the map<long, CBlendedParticleEffectSet> tree; Ego_r.pdb members of this type:
//   CMaterialDef::BlendedSwordHitEffects. Landed callers of its insert_unique 0x004695B3: 0x00469381 _global::GFSerialiseMapBinaryIn.
// Retail facts: signed (`jl`) key compare; _M_create_node 0x0045C2A2 allocates 0x24-byte nodes (`push 24h`) = 0x10 links + the 20-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_longkey_buy0045c2a2::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
class CBlendedParticleEffectSet {   // Ego_r.pdb, 16 bytes; no shared header defines it (engine/CMaterialDef.h only forward-declares it)
public:
    long Particle1Index;    // +0x0
    float Particle1Value;   // +0x4
    long Particle2Index;    // +0x8
    float Particle2Value;   // +0xC
};
namespace std {
template _Rb_tree<long, pair<long const, CBlendedParticleEffectSet>, _Select1st<pair<long const, CBlendedParticleEffectSet> >, less<long >, allocator<pair<long const, CBlendedParticleEffectSet> > >::iterator _Rb_tree<long, pair<long const, CBlendedParticleEffectSet>, _Select1st<pair<long const, CBlendedParticleEffectSet> >, less<long >, allocator<pair<long const, CBlendedParticleEffectSet> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<long const, CBlendedParticleEffectSet>&, _Rb_tree_node_base*);
}
