// CGameScriptInterface::SetExperienceSpendingAsEnabled(bool enabled) const -- World (+0x04)->ExperienceSpendingEnabled (+0xd6) = enabled
// Leaf: ?SetExperienceSpendingAsEnabled@CGameScriptInterface@@UBEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088F460, `ret 4`.
// Identity (NF431, 2026-11-25): the retail RTTI vftable placement of CGameScriptInterface (0x01260F0C) names this PDB member here;
//   Ego_r.pdb and FableWin.pdb list the symbol. FableWin's /Od body calls the inline FableWin.pdb ?SetExperienceSpendingEnabled@CWorld@@QAEX_N@Z
//   (one byte store); retail expands it, written here as the store to the shared header's CWorld::ExperienceSpendingEnabled.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"

void CGameScriptInterface::SetExperienceSpendingAsEnabled(bool enabled) const
{
    World->ExperienceSpendingEnabled = enabled;
}
