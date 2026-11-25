// std::pair<const EObjectAugmentationType, CObjectAugmentationParticleSet>::`scalar deleting destructor'(unsigned int flags) -- the
//   node value of CObjectAugmentationsDef::AugmentationParticles (map<EObjectAugmentationType, CObjectAugmentationParticleSet>): ~pair
//   (~CObjectAugmentationParticleSet 0x004E25F1 on the value at +4), then operator delete when bit 0 is set; returns this
// Leaf: ??_G?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationParticleSet@@@std@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E41A1, 31 bytes, thiscall, `ret 4`; size-optimised TU. Two references, both `push 0 ; call`:
//   the tree's _M_erase 0x004E7772 (at 0x004E7791, `lea ecx,[esi+10h]`: the node value) and the unreferenced __fastcall _Destroy copy
//   0x004E6E87 (at 0x004E6E89). Compiler-generated: STLport _construct.h `_Destroy(p) { p->~T(); }` is emitted by VC7.1 as a call of
//   this ??_G with 0. Byte twin of 0x004E3644 (pair<const EDamageAttribute, CHeroStatIncreaseData>).
// Name (NF409): (1) Ego_r.pdb names exactly this spelling (??_G?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationParticleSet@@@std@@
//   QAEPAXI@Z, donor 0x00504923; FableWin.pdb has the pair's ??1) and Ego_r.pdb types CObjectAugmentationsDef::AugmentationParticles as
//   map<EObjectAugmentationType, CObjectAugmentationParticleSet>; (2) retail: its _M_erase 0x004E7772 is called by that tree's clear
//   0x004E8212 (table-bound ?clear@?$_Rb_tree@W4EObjectAugmentationType@@...), and its only callee is ??1CObjectAugmentationParticleSet
//   0x004E25F1 (table-bound) on +4.
// NF409: was the strong free stand-in ?vector_deleting_destructor@@YIPAXPAVCLandscapeBackgroundPatch@@HI@Z (catalog
//   ScalarDeletingDtor_004e41a1 / manifest CLandscapeBackgroundPatch::`vector_deleting_destructor', propagated) on a row-local model with
//   an unbound callee stand-in @Sub_dtor_004e25f1@4.
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern): the deleting destructor is a compiler-emitted COMDAT; the anonymous-namespace
//   helper below destroys a pair through a typedef so this object emits it pick-any, as 0x004E7772's does. Internal to this TU; never
//   called. `#pragma optimize("s",on)` is the row's own mode.
#pragma optimize("s",on)
#include "stlport/fable_map.h"
#include "engine/EObjectAugmentationType.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/CObjectAugmentationParticleSet.h"

namespace {
typedef std::pair<const EObjectAugmentationType, CObjectAugmentationParticleSet> CAugmentationParticlePair;
void EmitAugmentationParticlePairDeletingDestructor(CAugmentationParticlePair* value)
{
    value->~CAugmentationParticlePair();
}
}
