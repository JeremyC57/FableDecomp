// CCountedPointer::operator=(const CCountedPointer&) -- share the other pointer: ShareData(Data, Info), return *this (T = NParticleEngine::CParticleEmitter)
// Leaf: ??4?$CCountedPointer@VCParticleEmitter@NParticleEngine@@@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE96E0, 72 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Set_00ae96e0 (stand-in name).
// T = NParticleEngine::CParticleEmitter, proven by: NParticleEngine::CParticleEmitterInfo::GetEmitterTemplate 0x00AF1480 binds CCountedPointer<NParticleEngine::CParticleEmitter>::operator= (EmitterTemplate: Ego_r CEmitterData +0x24 CCountedPointer<NParticleEngine::CParticleEmitter>); the same callers release with ~CCountedPointer<CParticleEmitter> 0x00AE94A0; FableWin.pdb ??4 name.
// Member: the in-class operator= of engine/CCountedPointer.h (loads Data / Info, then the ShareData body), emitted as its own COMDAT.
// Model: opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (ReleaseData zeroes Data first; its stores die under ShareData's own).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
namespace NParticleEngine { class CParticleEmitter; }
template CCountedPointer<NParticleEngine::CParticleEmitter>& CCountedPointer<NParticleEngine::CParticleEmitter>::operator=(const CCountedPointer<NParticleEngine::CParticleEmitter>&);
