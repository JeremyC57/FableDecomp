// CGameScriptInterface::ModifyThingHealth(const CScriptThing& entity, float amount, bool can_kill) const -- the live physical thing's ModifyHealth (a virtual)
// Leaf: ?ModifyThingHealth@CGameScriptInterface@@UBEXABVCScriptThing@@M_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00890430. Identity (NF433, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: entity.GetPThing() (CScriptThing slot 11, +0x2c), ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z, the inline
//   ?GetTypeFlags@CThing@@QBEKXZ (& 2), ??$GFDebugPDynamicCast@PAVCThing@@PAVCThingPhysical@@@@YIPAVCThingPhysical@@PAVCThing@@PAV0@@Z, then
//   the virtual ?ModifyHealth@CThingPhysical@@UAEXM_N@Z (retail slot +0xEC; FableWin's debug vtable +0xF8), arguments in order.
//   FableWin's CDebugManager::DoErrorMessage blocks are debug-only (absent in retail).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPhysical.h"
#include "engine/GFDebugPDynamicCast.h"

void CGameScriptInterface::ModifyThingHealth(const CScriptThing& entity, float amount, bool can_kill) const
{
    CThing* thing = entity.GetPThing();
    if (IS_THING_ALIVE(thing) && (thing->GetTypeFlags() & 2))
        GFDebugPDynamicCast(thing, (CThingPhysical*)0)->ModifyHealth(amount, can_kill);
}
