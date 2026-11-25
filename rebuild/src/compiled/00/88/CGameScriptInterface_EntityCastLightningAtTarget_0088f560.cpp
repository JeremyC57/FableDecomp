// CGameScriptInterface::EntityCastLightningAtTarget(const CScriptThing&, const CScriptThing&) const -- empty in retail (`ret 8`)
// Leaf: ?EntityCastLightningAtTarget@CGameScriptInterface@@UBEXABVCScriptThing@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088F560. Identity (NF431, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body is empty too (frame, `ret 8`).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EntityCastLightningAtTarget(const CScriptThing&, const CScriptThing&) const
{
}
