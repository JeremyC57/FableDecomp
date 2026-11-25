// CGameScriptInterface::DontPopulateNextLoadedRegion() const -- World (+0x04)->SetDontPopulateNextLoadedRegion(), FableWin's inline CWorld accessor
// Leaf: ?DontPopulateNextLoadedRegion@CGameScriptInterface@@UBEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088E380. Identity (NF433, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: World (+0x04), then the inline ?SetDontPopulateNextLoadedRegion@CWorld@@QAEXXZ (DontPopulateNextLoadedRegion +0xD8 = true), expanded in retail.
// Round 2 of NF431's landed port (coordinator debt line "nf431 CWorld accessors"): the member is now reached through FableWin's accessor; same bytes.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_SCRIPTFLAGACCESSORS
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"

void CGameScriptInterface::DontPopulateNextLoadedRegion() const
{
    World->SetDontPopulateNextLoadedRegion();
}
