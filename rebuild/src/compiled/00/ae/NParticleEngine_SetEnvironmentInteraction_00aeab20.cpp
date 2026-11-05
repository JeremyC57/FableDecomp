// NParticleEngine::SetEnvironmentInteraction(CCountedPointer<CAEnvironmentInteraction> interaction) -- install the particle engine's environment-interaction object (gravity / wind queries): counted-pointer assign into the engine global, then release the by-value argument
// Leaf: ?SetEnvironmentInteraction@NParticleEngine@@YIXV?$CCountedPointer@VCAEnvironmentInteraction@NParticleEngine@@@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members Block->Info are the header CCountedPointer's (same offsets); the local CCountBlock (= CCPPointerInfo) members DeleteData->DeleteFunc are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEAB20, 99 bytes, __fastcall with the 8-byte counted pointer on the stack (`ret 8`).
// Identity: Ego_r.pdb `?SetEnvironmentInteraction@NParticleEngine@@YIXV?$CCountedPointer@VCAEnvironmentInteraction@NParticleEngine@@@@@Z`
//   (manifest name right); the sibling GetEnvironmentInteraction 0x00AEAB10 returns *Data of the same global. Caller 0x00417531.
// Callees: the count block's per-type destroy function (`call [block+4]`, object in ecx), operator delete 0x00BFE9BC.
// Globals: NParticleEngine::PEnvironmentInteraction, the CCountedPointer<CAEnvironmentInteraction> at 0x013D2E2C (Data) /
//   0x013D2E30 (Block) -- FableWin.pdb's dynamic initializer names it, after PEmitterDatabase / PEmitterManager / PPSCManager.
// Layout: CCountedPointer = {T* Data, count block*}; count block = {RefCount, __fastcall destroy function, object}.
// Shape: CCountedPointer::operator= (`if (Block != other.Block) { ReleaseData(); Data = other.Data; Block = other.Block; if
//   (Block) ++Block->RefCount; }`) inline, then the argument's inline destructor (the same ReleaseData, whose trailing
//   `Block = 0; Data = 0;` stores are dead in both places -- the Uninitialise 0x00AEBEA0 model). The global's block is
//   re-read after its decrement (the store may alias it); the register-held argument decrements with `dec ; jne`.
namespace NParticleEngine { class CAEnvironmentInteraction; }

#include "engine/CCountedPointer.h"


namespace NParticleEngine {
extern CCountedPointer<CAEnvironmentInteraction> PEnvironmentInteraction;   // 0x013D2E2C

void __fastcall SetEnvironmentInteraction(CCountedPointer<CAEnvironmentInteraction> interaction)
{
    PEnvironmentInteraction = interaction;
}
}