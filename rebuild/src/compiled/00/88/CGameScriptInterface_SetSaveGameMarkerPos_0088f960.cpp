// CGameScriptInterface::SetSaveGameMarkerPos(const C3DVector& pos) const -- World (+0x04)->SetSaveGameMarkerPos(pos), a tail jump
// Leaf: ?SetSaveGameMarkerPos@CGameScriptInterface@@UBEXABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088F960. Identity (NF431, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body calls ?SetSaveGameMarkerPos@CWorld@@QAEXABVC3DVector@@@Z (retail 0x0049E590, the jmp target) on World.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_UPDATEREGIONLOAD_CALLEES
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"

void CGameScriptInterface::SetSaveGameMarkerPos(const C3DVector& pos) const
{
    World->SetSaveGameMarkerPos(pos);
}
