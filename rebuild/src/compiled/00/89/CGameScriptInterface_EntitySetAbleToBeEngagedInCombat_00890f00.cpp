// CGameScriptInterface::EntitySetAbleToBeEngagedInCombat(const CScriptThing& entity, bool able) const -- the live physical thing's AbleToBeEngagedInCombat flag
// Leaf: ?EntitySetAbleToBeEngagedInCombat@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00890F00. Identity (NF433, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: entity.GetPThing() (CScriptThing slot 11, +0x2c), ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z, the inline
//   ?GetTypeFlags@CThing@@QBEKXZ (& 2), ??$GFDebugPDynamicCast@PAVCThing@@PAVCThingPhysical@@@@YIPAVCThingPhysical@@PAVCThing@@PAV0@@Z, then
//   the inline ?SetAsAbleToBeEngagedInCombat@CThingPhysical@@QAEX_N@Z (bit 5 of +0xBC); retail expands all of them.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_SCRIPTHITSETTERS
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPhysical.h"
#include "engine/GFDebugPDynamicCast.h"

void CGameScriptInterface::EntitySetAbleToBeEngagedInCombat(const CScriptThing& entity, bool able) const
{
    CThing* thing = entity.GetPThing();
    if (IS_THING_ALIVE(thing) && (thing->GetTypeFlags() & 2))
        GFDebugPDynamicCast(thing, (CThingPhysical*)0)->SetAsAbleToBeEngagedInCombat(able);
}
