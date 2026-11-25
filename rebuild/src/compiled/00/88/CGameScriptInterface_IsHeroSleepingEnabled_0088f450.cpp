// CGameScriptInterface::IsHeroSleepingEnabled() const -- World (+0x04)->IsHeroSleepingEnabled(), FableWin's inline CWorld accessor
// Leaf: ?IsHeroSleepingEnabled@CGameScriptInterface@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088F450. Identity (NF433, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: World (+0x04), then the inline ?IsHeroSleepingEnabled@CWorld@@QBE_NXZ (HeroSleepingEnabled +0xD9; the existing FABLE_RETAIL_BED_USE_PASS28 accessor), expanded in retail.
// Round 2 of NF431's landed port (coordinator debt line "nf431 CWorld accessors"): the member is now reached through FableWin's accessor; same bytes.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_BED_USE_PASS28
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"

bool CGameScriptInterface::IsHeroSleepingEnabled() const
{
    return World->IsHeroSleepingEnabled();
}
