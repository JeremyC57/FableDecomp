// CGameScriptInterface::EntitySetThingAsAllyOfThing(const CScriptThing& entity, const CScriptThing& ally) const -- ImpSetEntityAsAllyOfThing(entity, ally, true)
// Leaf: ?EntitySetThingAsAllyOfThing@CGameScriptInterface@@UBEXABVCScriptThing@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A7EC0. Identity (NF431, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body asserts the entity is valid (debug-only error report, absent from retail) and calls the private
//   ?ImpSetEntityAsAllyOfThing@CGameScriptInterface@@ABEXABVCScriptThing@@0_N@Z (retail 0x0089C100) with true (`push 1`, 0x008A7EC8).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CGAMESCRIPTINTERFACE_IMPSETENTITYASALLY
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EntitySetThingAsAllyOfThing(const CScriptThing& entity, const CScriptThing& ally) const
{
    ImpSetEntityAsAllyOfThing(entity, ally, true);
}
