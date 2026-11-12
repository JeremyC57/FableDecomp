// CCountedPointer<NParticleEngine::CParticleEmitter>::CCountedPointer(T*) -- take ownership of a new emitter: a fresh count block (count 1, DeleteData, Data) when non-null
// Leaf: ??0?$CCountedPointer@VCParticleEmitter@NParticleEngine@@@@QAE@PAVCParticleEmitter@NParticleEngine@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE9450, 78 bytes, `ret 4`. Callees: operator new 0x00BFEA1A; stores DeleteData 0x00AE91D0.
// Was landed as CCountedPointer_00ae91d0::CCountedPointer_00ae91d0 (stand-in name and local count-block model).
// T (agentNF28, 2026-11-12, queue land1011dx): the local it builds is passed to CParticleEmitterList::Insert(CCountedPointer<CParticleEmitter>&)
//   0x00AEB060 by CreateEmitter 0x00AEB6D0 / Create2DEmitter 0x00AEB790; the block stores DeleteData 0x00AE91D0, which the catalog row
//   0x00AE91D0 and binder 0x00AE8C30 spell ?DeleteData@?$CCountedPointer@VCParticleEmitter@NParticleEngine@@@@SIXPAX@Z; the paired dtor
//   0x00AE94A0 is landed as ~CCountedPointer<NParticleEngine::CParticleEmitter>; FableWin.pdb has this ctor name.
// Member: the in-class (T*) constructor of engine/CCountedPointer.h, emitted out of line as its own COMDAT.
#include "engine/CCountedPointer.h"
namespace NParticleEngine { class CParticleEmitter; }
template CCountedPointer<NParticleEngine::CParticleEmitter>::CCountedPointer(NParticleEngine::CParticleEmitter*);
