// CCountedPointer::Reset(T* data) -- (CCountedPointer<NParticleEngine::CParticleEmitter>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCParticleEmitter@NParticleEngine@@@@QAEXPAVCParticleEmitter@NParticleEngine@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF1860, thiscall, `ret 4`. Stores DeleteData 0x00AE91D0
//   (?DeleteData@?$CCountedPointer@VCParticleEmitter@NParticleEngine@@@@SIXPAX@Z, landed binding).
// Naming (NF158, 2026-11-20, q4919): was the stand-in CCountedPointer_00ae91d0::Reset (?Reset@CCountedPointer_00ae91d0@@QAEXPAX@Z) with a
//   local count-block model. Identity: FableWin.pdb ?Reset@?$CCountedPointer@VCParticleEmitter@NParticleEngine@@@@QAEXPAVCParticleEmitter@NParticleEngine@@@Z
//   (Ego_r keeps Reset out of line for every T); the only landed referencer 0x00AF12A0 calls it on EmitterTemplate
//   (CCountedPointer<NParticleEngine::CParticleEmitter>) and already binds this spelling; the stored deleter 0x00AE91D0 is the
//   CParticleEmitter DeleteData. Now the shared engine/CCountedPointer.h Reset, explicitly instantiated (the 0x00834E40 recipe).
#include "engine/CCountedPointer.h"
namespace NParticleEngine { class CParticleEmitter; }
template void CCountedPointer<NParticleEngine::CParticleEmitter>::Reset(NParticleEngine::CParticleEmitter*);
