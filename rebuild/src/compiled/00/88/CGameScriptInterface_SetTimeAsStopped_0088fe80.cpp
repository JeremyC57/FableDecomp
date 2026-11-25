// CGameScriptInterface::SetTimeAsStopped(bool stopped, long& time) const -- World->GetGameTimeManager().SetTimeAsStopped, a tail jump
// Leaf: ?SetTimeAsStopped@CGameScriptInterface@@UBEX_NAAJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088FE80. Identity (NF431, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: ?GetGameTimeManager@CWorld@@QAEAAVCGameTimeManager@@XZ (inline, World +0x1c) then
//   ?SetTimeAsStopped@CGameTimeManager@@QAEX_NAAJ@Z (retail 0x006BBE30, the jmp target).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMETIMEMANAGER_FULL
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include "engine/CGameTimeManager.h"

void CGameScriptInterface::SetTimeAsStopped(bool stopped, long& time) const
{
    World->GetGameTimeManager().SetTimeAsStopped(stopped, time);
}
