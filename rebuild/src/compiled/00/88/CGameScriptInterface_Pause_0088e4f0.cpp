// CGameScriptInterface::Pause(float seconds) const -- forwards to the protected Rest(float) (0x006E71F0), a tail jump
// Leaf: ?Pause@CGameScriptInterface@@UBEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088E4F0. Identity (NF431, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body calls ?Rest@CGameScriptInterface@@IBEXM@Z with the same argument (Rest is declared by the header's
//   FABLE_RETAIL_CGAMESCRIPTINTERFACE_ENDCUTFADE_CALLEES opt-in, landed self 0x006E71F0).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CGAMESCRIPTINTERFACE_ENDCUTFADE_CALLEES
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::Pause(float seconds) const
{
    Rest(seconds);
}
