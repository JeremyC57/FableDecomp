// std::_Rb_tree<long, pair<long const, CBlendedParticleEffectSet>, std::_Select1st<pair<long const, CBlendedParticleEffectSet> >, less<long >, allocator<pair<long const, CBlendedParticleEffectSet> > >::_M_create_node(const value_type& x) -- STLport 4.6 _tree.h: allocate one tree node (the 0x10-byte links plus
//   value_type) and copy-construct the value into it (`_Construct`, placement new, null-tested); `this` is never read
// Leaf: ?_M_create_node@?$_Rb_tree@JU?$pair@$$CBJVCBlendedParticleEffectSet@@@std@@U?$_Select1st@U?$pair@$$CBJVCBlendedParticleEffectSet@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCBlendedParticleEffectSet@@@std@@@2@@std@@IAEPAU?$_Rb_tree_node@U?$pair@$$CBJVCBlendedParticleEffectSet@@@std@@@2@ABU?$pair@$$CBJVCBlendedParticleEffectSet@@@2@@Z
// Flags: /d2inlS20
// Inline-budget: /d2inlS20 -- STLport's inline _Construct (placement new of the pair copy) inside _M_create_node: retail expands
//   it in this size TU (`lea ..,[..+10h] ; test ; je`, then the value copy), plain VC7.1 /Os keeps it a call. NF61 hand-proof:
//   0x00431471 calls `_Construct` under plain /Os, /O1 and /d2inlT200, expands it under /d2inlS15 / S25 / S40 (/O2 expands it
//   but cleans malloc's argument with `add esp,4`, retail `pop ecx`); 0x00466255 (CCharString value) still calls the pair copy
//   constructor under /d2inlS15 and matches under S20 / S25 / S40. S20 is the smallest value that matches the whole family.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045C2A2, 40 bytes, size TU, thiscall `ret 4`.
// NF61 (round 7, q10591): the protected STLport member (IAE), an explicit instantiation of stlport/fable_map.h _M_create_node for the
//   instance whose _M_insert 0x00469639 (landed by NF58, its template arguments proven there from the Ego_r.pdb member and the node
//   size) calls it with exactly this spelling; the value-type declarations are that row's. Was the stand-in `CRetailTree_0045c2a2::BuyNode`.
//   Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds).
#pragma optimize("s", on)   // the 0x0045 module is size-optimised: `pop ecx` after malloc
#include "stlport/fable_map.h"
class CBlendedParticleEffectSet {   // Ego_r.pdb, 16 bytes; no shared header defines it (engine/CMaterialDef.h only forward-declares it)
public:
    long Particle1Index;    // +0x0
    float Particle1Value;   // +0x4
    long Particle2Index;    // +0x8
    float Particle2Value;   // +0xC
};
namespace std {
template _Rb_tree_node<pair<long const, CBlendedParticleEffectSet> >* _Rb_tree<long, pair<long const, CBlendedParticleEffectSet>, _Select1st<pair<long const, CBlendedParticleEffectSet> >, less<long >, allocator<pair<long const, CBlendedParticleEffectSet> > >::_M_create_node(const pair<long const, CBlendedParticleEffectSet>&);
}
