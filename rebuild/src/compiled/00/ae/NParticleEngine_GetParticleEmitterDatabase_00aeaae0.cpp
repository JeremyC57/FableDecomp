// NParticleEngine::GetParticleEmitterDatabase() -- the engine's emitter template database
// Leaf: ?GetParticleEmitterDatabase@NParticleEngine@@YIAAVCParticleEmitterDatabase@1@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEAAE0, 6 bytes: mov eax,[0x013D2E14]; ret (__fastcall, no `this`): the inline CCountedPointer<CParticleEmitterDatabase>::operator*.
// Naming wave 35c: Ego_r.pdb + FableWin.pdb ?GetParticleEmitterDatabase@NParticleEngine@@YIAAVCParticleEmitterDatabase@1@XZ (the NParticleEngine accessor family 0x00AEAA00..0x00AEAB20 in
//   declaration order); the catalog carries the name. Was the stand-in `extern "C" int GetGlobal()` on a placeholder int
//   (old file global_GFGetPreMainMemoryUsed_00aeaae0.cpp).
// Global: NParticleEngine::PEmitterDatabase, the CCountedPointer<CParticleEmitterDatabase> at 0x013D2E14 (Data) -- the name the landed NParticleEngine::Initialise
//   0x00AEBE20 / SetEnvironmentInteraction 0x00AEAB20 already bind (FableWin.pdb dynamic initializers, in address order).
#include "engine/CCountedPointer.h"
namespace NParticleEngine {
class CParticleEmitterDatabase;
extern CCountedPointer<CParticleEmitterDatabase> PEmitterDatabase;   // 0x013D2E14
CParticleEmitterDatabase& __fastcall GetParticleEmitterDatabase()
{
    return *PEmitterDatabase;
}
}
