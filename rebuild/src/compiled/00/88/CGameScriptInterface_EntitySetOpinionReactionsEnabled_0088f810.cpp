// CGameScriptInterface::EntitySetOpinionReactionsEnabled(const CScriptThing& entity, bool enabled) const -- the "SG_OPINION_REACTION_ROOT" state group through the virtual EntitySetStategroupEnabled
// Leaf: ?EntitySetOpinionReactionsEnabled@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088F810. Identity (NF433, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: ??0CCharString@@QAE@PBDJ@Z ("SG_OPINION_REACTION_ROOT", -1; retail 0x0099EBF0), the virtual
//   ?EntitySetStategroupEnabled@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@_N@Z (slot 590, +0x938) with the entity, the
//   named local and the flag, then ??1CCharString@@QAE@XZ (retail 0x0099EAE0).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"

void CGameScriptInterface::EntitySetOpinionReactionsEnabled(const CScriptThing& entity, bool enabled) const
{
    CCharString state_group("SG_OPINION_REACTION_ROOT");
    EntitySetStategroupEnabled(entity, state_group, enabled);
}
