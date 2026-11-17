// std::_Rb_tree<long, pair<long const, CBlendedParticleEffectSet>, _Select1st<pair<long const, CBlendedParticleEffectSet> >, less<long >, allocator<pair<long const, CBlendedParticleEffectSet> > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then _M_insert (0x00469639) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@JU?$pair@$$CBJVCBlendedParticleEffectSet@@@std@@U?$_Select1st@U?$pair@$$CBJVCBlendedParticleEffectSet@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCBlendedParticleEffectSet@@@std@@@2@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBJVCBlendedParticleEffectSet@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJVCBlendedParticleEffectSet@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBJVCBlendedParticleEffectSet@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004695B3, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the map<long, CBlendedParticleEffectSet> tree; Ego_r.pdb members of this type:
//   CMaterialDef::BlendedSwordHitEffects. Landed callers of its insert_unique 0x004695B3: 0x00469381 _global::GFSerialiseMapBinaryIn.
//   Its callee _M_insert 0x00469639 is the same instance. STLport 4.6.2 source, descriptive-by-library. Was the stand-in
//   `CRetailTree_longkey_ins00469639::insert`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body;
//   FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"
class CBlendedParticleEffectSet {   // Ego_r.pdb, 16 bytes; no shared header defines it (engine/CMaterialDef.h only forward-declares it)
public:
    long Particle1Index;    // +0x0
    float Particle1Value;   // +0x4
    long Particle2Index;    // +0x8
    float Particle2Value;   // +0xC
};
namespace std {
template pair<_Rb_tree<long, pair<long const, CBlendedParticleEffectSet>, _Select1st<pair<long const, CBlendedParticleEffectSet> >, less<long >, allocator<pair<long const, CBlendedParticleEffectSet> > >::iterator, bool> _Rb_tree<long, pair<long const, CBlendedParticleEffectSet>, _Select1st<pair<long const, CBlendedParticleEffectSet> >, less<long >, allocator<pair<long const, CBlendedParticleEffectSet> > >::insert_unique(const pair<long const, CBlendedParticleEffectSet>&);
}
