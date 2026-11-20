// CCountedPointer<NParticleEngine::CParticleEmitter>::DeleteData(void* data) -- the count block's deleter: delete the emitter through its
// virtual destructor (null-tested; `push 1 ; call [eax]`).
// Leaf: ?DeleteData@?$CCountedPointer@VCParticleEmitter@NParticleEngine@@@@SIXPAX@Z
// Naming (NF173, 2026-11-20): q11962: re-authored from the free stand-in DeleteData(AutoTinyVirtualObject*) as the explicit instantiation of
//   the shared engine/CCountedPointer.h DeleteData (the 0x00BB6330 / 0x008364B0 recipe) on engine/CParticleEmitter.h
//   (NParticleEngine::CParticleEmitter, virtual destructor). FableWin.pdb 0x02EF8D00 names this CCountedPointer<NParticleEngine::CParticleEmitter>
//   instance; Reset 0x00AF1860 (FableWin ?Reset@?$CCountedPointer@VCParticleEmitter@NParticleEngine@@@@...) stores this address in the count
//   block, and all 3 landed referencers (0x00AE8C30 0x00AE9450 0x00AF1860) bind this spelling.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE91D0.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CPARTICLEEMITTER
#include "engine/CParticleEmitter.h"
#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<NParticleEngine::CParticleEmitter>::DeleteData(void*);
