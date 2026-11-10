// CGameScriptInterface::IsSoundPlaying(unsigned long soundId) const -- script query: whether the sound manager still plays that sample id
// Leaf: ?IsSoundPlaying@CGameScriptInterface@@UBE_NK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088F610, 11 bytes, thiscall: `mov ecx,[PSoundManager] ; mov eax,[ecx] ; jmp [eax+3Ch]`
//   (the sibling call leaves the argument for the callee's `ret 4`).
// Identity: Ego_r.pdb ?IsSoundPlaying@CGameScriptInterface@@UBE_NK@Z (virtual, slot 693 of engine/CGameScriptInterface.h).
// Sound manager (agentSND2, 2026-11-10): engine/CASoundManager.h slot 15 IsSoundPlaying on PSoundManager 0x013B8394; was a raw-vtable
//   function-pointer stand-in on a __fastcall local class (respelled to the PDB thiscall member: NAME_FIXES).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
#include "engine/CASoundManager.h"

bool CGameScriptInterface::IsSoundPlaying(unsigned long soundId) const
{
    return PSoundManager->IsSoundPlaying(soundId);
}
