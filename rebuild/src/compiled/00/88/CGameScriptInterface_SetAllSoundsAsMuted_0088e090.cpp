// CGameScriptInterface::SetAllSoundsAsMuted(bool muted) const -- CUserProfileManager::Get().SetSoundAsMuted(muted)
// Leaf: ?SetAllSoundsAsMuted@CGameScriptInterface@@UBEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088E090. Identity (NF431, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: ?Get@CUserProfileManager@@SIAAV1@XZ (retail 0x0040D2A0) then ?SetSoundAsMuted@CUserProfileManager@@QAEX_N@Z
//   (retail 0x00408340).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SETSOUNDASMUTED
#include "engine/CGameScriptInterface.h"
#include "engine/CUserProfileManager.h"

void CGameScriptInterface::SetAllSoundsAsMuted(bool muted) const
{
    CUserProfileManager::Get().SetSoundAsMuted(muted);
}
