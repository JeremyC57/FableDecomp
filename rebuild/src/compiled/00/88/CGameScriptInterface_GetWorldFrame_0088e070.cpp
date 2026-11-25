// CGameScriptInterface::GetWorldFrame() const -- the world frame counter: CWorld::GetFrame() (static __fastcall), a tail jump
// Leaf: ?GetWorldFrame@CGameScriptInterface@@UBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088E070. Identity (NF431, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body calls ?GetFrame@CWorld@@SIJXZ (retail 0x0049D870, the jmp target); VC7.1 turns the call into the tail jump.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"

long CGameScriptInterface::GetWorldFrame() const
{
    return CWorld::GetFrame();
}
