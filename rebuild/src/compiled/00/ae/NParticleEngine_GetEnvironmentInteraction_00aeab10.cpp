// NParticleEngine::GetEnvironmentInteraction() -- the environment-interaction object (collision, wind, gravity) SetEnvironmentInteraction installed
// Leaf: ?GetEnvironmentInteraction@NParticleEngine@@YIAAVCAEnvironmentInteraction@1@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEAB10, 6 bytes: mov eax,[0x013D2E2C]; ret (__fastcall, no `this`): the inline CCountedPointer<CAEnvironmentInteraction>::operator*.
// Naming wave 35c: Ego_r.pdb + FableWin.pdb ?GetEnvironmentInteraction@NParticleEngine@@YIAAVCAEnvironmentInteraction@1@XZ (the NParticleEngine accessor family 0x00AEAA00..0x00AEAB20 in
//   declaration order); the catalog carries the name. Was the stand-in `extern "C" int GetGlobal()` on a placeholder int
//   (old file global_GFGetPreMainMemoryUsed_00aeab10.cpp).
// Global: NParticleEngine::PEnvironmentInteraction, the CCountedPointer<CAEnvironmentInteraction> at 0x013D2E2C (Data) -- the name the landed NParticleEngine::Initialise
//   0x00AEBE20 / SetEnvironmentInteraction 0x00AEAB20 already bind (FableWin.pdb dynamic initializers, in address order).
#include "engine/CCountedPointer.h"
namespace NParticleEngine {
class CAEnvironmentInteraction;
extern CCountedPointer<CAEnvironmentInteraction> PEnvironmentInteraction;   // 0x013D2E2C
CAEnvironmentInteraction& __fastcall GetEnvironmentInteraction()
{
    return *PEnvironmentInteraction;
}
}
