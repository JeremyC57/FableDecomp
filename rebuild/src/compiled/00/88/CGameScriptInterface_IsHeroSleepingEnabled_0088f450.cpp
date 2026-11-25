// CGameScriptInterface::IsHeroSleepingEnabled() const -- World (+0x04)->HeroSleepingEnabled (+0xd9)
// Leaf: ?IsHeroSleepingEnabled@CGameScriptInterface@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088F450. Identity (NF431, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body calls the inline ?IsHeroSleepingEnabled@CWorld@@QBE_NXZ (a byte read); retail expands it, written here as the read
//   of the shared header's CWorld::HeroSleepingEnabled.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"

bool CGameScriptInterface::IsHeroSleepingEnabled() const
{
    return World->HeroSleepingEnabled;
}
