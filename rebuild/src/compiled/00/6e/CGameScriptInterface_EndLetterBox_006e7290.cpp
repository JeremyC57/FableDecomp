// CGameScriptInterface::EndLetterBox() -- the display engine's EndLetterBoxMode, a tail jump
// Leaf: ?EndLetterBox@CGameScriptInterface@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E7290. Identity (NF433, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: DisplayEngine (+0x0c) in ecx, then ?EndLetterBoxMode@CDisplayEngine@@QAEXXZ (retail 0x004349D0, the jmp target).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CDISPLAYENGINE_ENDLETTERBOXMODE
#include "engine/CGameScriptInterface.h"
#include "engine/CDisplayEngine.h"

void CGameScriptInterface::EndLetterBox()
{
    DisplayEngine->EndLetterBoxMode();
}
