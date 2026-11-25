// CGameScriptInterface::DontPopulateNextLoadedRegion() const -- World (+0x04)->DontPopulateNextLoadedRegion (+0xd8) = true
// Leaf: ?DontPopulateNextLoadedRegion@CGameScriptInterface@@UBEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088E380. Identity (NF431, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body calls the inline ?SetDontPopulateNextLoadedRegion@CWorld@@QAEXXZ (one byte store of 1); retail expands it, written
//   here as the store to the shared header's CWorld::DontPopulateNextLoadedRegion.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"

void CGameScriptInterface::DontPopulateNextLoadedRegion() const
{
    World->DontPopulateNextLoadedRegion = true;
}
