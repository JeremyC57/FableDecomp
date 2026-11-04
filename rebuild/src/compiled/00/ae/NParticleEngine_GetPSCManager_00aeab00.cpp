// NParticleEngine::GetPSCManager() -- the engine's particle-system-component manager
// Leaf: ?GetPSCManager@NParticleEngine@@YIAAVCPSCManager@1@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEAB00, 6 bytes: mov eax,[0x013D2E24]; ret (__fastcall, no `this`): the inline CCountedPointer<CPSCManager>::operator*.
// Naming wave 35c: Ego_r.pdb + FableWin.pdb ?GetPSCManager@NParticleEngine@@YIAAVCPSCManager@1@XZ (the NParticleEngine accessor family 0x00AEAA00..0x00AEAB20 in
//   declaration order); the catalog carries the name. Was the stand-in `extern "C" int GetGlobal()` on a placeholder int
//   (old file global_GFGetPreMainMemoryUsed_00aeab00.cpp).
// Global: NParticleEngine::PPSCManager, the CCountedPointer<CPSCManager> at 0x013D2E24 (Data) -- the name the landed NParticleEngine::Initialise
//   0x00AEBE20 / SetEnvironmentInteraction 0x00AEAB20 already bind (FableWin.pdb dynamic initializers, in address order).
#include "engine/CCountedPointer.h"
namespace NParticleEngine {
class CPSCManager;
extern CCountedPointer<CPSCManager> PPSCManager;   // 0x013D2E24
CPSCManager& __fastcall GetPSCManager()
{
    return *PPSCManager;
}
}
