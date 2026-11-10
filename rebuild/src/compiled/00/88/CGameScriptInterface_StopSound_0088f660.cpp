// CGameScriptInterface::StopSound(unsigned long soundId) const -- script command: stop that sample id through the sound manager
// Leaf: ?StopSound@CGameScriptInterface@@UBEXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088F660, thiscall: `mov ecx,[PSoundManager] ; mov eax,[ecx] ; jmp [eax+38h]` (the sibling
//   call leaves the argument for the callee's `ret 4`).
// Identity: Ego_r.pdb / FableWin.pdb ?StopSound@CGameScriptInterface@@UBEXK@Z (virtual, slot 695 of engine/CGameScriptInterface.h).
// Sound manager (agentSND2, 2026-11-10): engine/CASoundManager.h slot 14 StopSound on PSoundManager 0x013B8394; the landed row's
//   local CGSIStopSoundEmitter model (Slot00..Slot34 placeholders) and the local CGameScriptInterface declaration are gone.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
#include "engine/CASoundManager.h"

void CGameScriptInterface::StopSound(unsigned long soundId) const
{
    PSoundManager->StopSound(soundId);
}
