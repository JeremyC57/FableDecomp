// NParticleEngine::GetParticleEmitterManager() -- the engine's particle emitter manager
// Leaf: ?GetParticleEmitterManager@NParticleEngine@@YIAAVCParticleEmitterManager@1@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEAAF0, 6 bytes: mov eax,[0x013D2E1C]; ret (__fastcall, no `this`): the inline CCountedPointer<CParticleEmitterManager>::operator*.
// Flags: none
// Name-fix pass (2026-11-12): Ego_r.pdb 0x00AAEF80 + FableWin.pdb 0x02EF2810 ?GetParticleEmitterManager@NParticleEngine@@YIAAVCParticleEmitterManager@1@XZ
//   (the NParticleEngine accessor family 0x00AEAA00..0x00AEAB20 in declaration order, next to GetParticleEmitterDatabase 0x00AEAAE0);
//   the landed CDrawMeleeActionButtonEffects::Draw 0x00646580 and 0x00AFB7C0 already bind this name. Was the made-up
//   `extern "C" int GetGlobal()` on a placeholder int.
// Global: NParticleEngine::PEmitterManager, the CCountedPointer<CParticleEmitterManager> at 0x013D2E1C (Data) -- the name the landed
//   NParticleEngine::Initialise 0x00AEBE20 already binds.
#include "engine/CCountedPointer.h"
namespace NParticleEngine {
class CParticleEmitterManager;
extern CCountedPointer<CParticleEmitterManager> PEmitterManager;   // 0x013D2E1C
CParticleEmitterManager& __fastcall GetParticleEmitterManager()
{
    return *PEmitterManager;
}
}
