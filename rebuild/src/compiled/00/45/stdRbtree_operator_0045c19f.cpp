// std::_Rb_tree<long, pair<long const, CBlendedParticleEffectSet>, _Select1st<pair<long const, CBlendedParticleEffectSet> >, less<long >, allocator<pair<long const, CBlendedParticleEffectSet> > >::operator=(const _Rb_tree&) -- STLport 4.6 _tree.c tree assignment: unless self-assigned, clear 0x00450FAC, zero the count, then
//   reset the header to an empty tree or _M_copy 0x0045C212 the source root under the header, walk leftmost / rightmost, copy the count
// Leaf: ??4?$_Rb_tree@JU?$pair@$$CBJVCBlendedParticleEffectSet@@@std@@U?$_Select1st@U?$pair@$$CBJVCBlendedParticleEffectSet@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCBlendedParticleEffectSet@@@std@@@2@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045C19F, 115 bytes, size TU (`and dword ptr [..],0`), thiscall `ret 4`.
// NF64 (round 7, q10689): an explicit instantiation of stlport/fable_map.h operator= (overlay opt-in FABLE_RETAIL_STLP_TREE_ASSIGN_BODY:
//   STLport's _tree.c text, _M_copy declared only) for the tree whose _M_copy 0x0045C212 NF61 re-authored; the value-type declarations
//   are that row's. It now binds _M_copy and clear under their STLport spellings. Was the stand-in `CRetailTree_0045c212::operator=` (bound
//   ?Copy@CRetailTree_0045c212 / ?Clear@CRetailTree_0045c212). Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds; FableWin.pdb
//   lists the class-level ??4CHeroAbilityStatWeight at 0x0191AD74, 31 bytes before the next symbol, as its own function).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_ASSIGN_BODY
#include "stlport/fable_map.h"
class CBlendedParticleEffectSet {   // Ego_r.pdb, 16 bytes; no shared header defines it (engine/CMaterialDef.h only forward-declares it)
public:
    long Particle1Index;    // +0x0
    float Particle1Value;   // +0x4
    long Particle2Index;    // +0x8
    float Particle2Value;   // +0xC
};
namespace std {
template _Rb_tree<long, pair<long const, CBlendedParticleEffectSet>, _Select1st<pair<long const, CBlendedParticleEffectSet> >, less<long >, allocator<pair<long const, CBlendedParticleEffectSet> > >& _Rb_tree<long, pair<long const, CBlendedParticleEffectSet>, _Select1st<pair<long const, CBlendedParticleEffectSet> >, less<long >, allocator<pair<long const, CBlendedParticleEffectSet> > >::operator=(const _Rb_tree<long, pair<long const, CBlendedParticleEffectSet>, _Select1st<pair<long const, CBlendedParticleEffectSet> >, less<long >, allocator<pair<long const, CBlendedParticleEffectSet> > >&);
}
