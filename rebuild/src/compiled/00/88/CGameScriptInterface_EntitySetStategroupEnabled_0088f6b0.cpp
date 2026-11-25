// CGameScriptInterface::EntitySetStategroupEnabled(const CScriptThing& entity, const CCharString& state_group, bool enabled) const -- an AI creature's SetStateGroupEnabled
// Leaf: ?EntitySetStategroupEnabled@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088F6B0. Identity (NF433, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: entity.GetPThing() (CScriptThing slot 11, +0x2c), non-null, the inline ?GetType@CClass@@QBEKXZ == 1, then
//   ??$GFDebugPDynamicCast@PAVCThing@@PAVCThingAICreature@@@@YIPAVCThingAICreature@@PAVCThing@@PAV0@@Z and
//   ?SetStateGroupEnabled@CThingAICreature@@QAEXABVCCharString@@_N@Z (retail 0x008302D0), arguments in order.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATEGROUPENABLED
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/GFDebugPDynamicCast.h"

void CGameScriptInterface::EntitySetStategroupEnabled(const CScriptThing& entity, const CCharString& state_group, bool enabled) const
{
    CThing* thing = entity.GetPThing();
    if (thing && thing->GetType() == 1)
        GFDebugPDynamicCast(thing, (CThingAICreature*)0)->SetStateGroupEnabled(state_group, enabled);
}
