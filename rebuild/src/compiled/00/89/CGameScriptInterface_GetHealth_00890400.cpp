// CGameScriptInterface::GetHealth(const CScriptThing& entity) const -- the live physical thing's health, else 0
// Leaf: ?GetHealth@CGameScriptInterface@@UBEMABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00890400. Identity (NF433, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: entity.GetPThing() (CScriptThing slot 11, +0x2c), ??$IS_THING_ALIVE@PBVCThing@@@@YI_NABQBVCThing@@@Z, the inline
//   ?GetTypeFlags@CThing@@QBEKXZ (& 2), ??$GFDebugPDynamicCast@PBVCThing@@PBVCThingPhysical@@@@YIPBVCThingPhysical@@PBVCThing@@PBV0@@Z, then
//   the inline ?GetHealth@CThingPhysical@@QBEMXZ (+0xB4); else 0. FableWin's CDebugManager::DoErrorMessage blocks are debug-only (absent in retail).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPhysical.h"
#include "engine/GFDebugPDynamicCast.h"

float CGameScriptInterface::GetHealth(const CScriptThing& entity) const
{
    const CThing* thing = entity.GetPThing();
    if (IS_THING_ALIVE(thing) && (thing->GetTypeFlags() & 2))
        return GFDebugPDynamicCast(thing, (const CThingPhysical*)0)->GetHealth();
    return 0.0f;
}
