// CGameScriptInterface::IsSleepingTime(const CScriptThing& village) const -- the live village thing's night-time test
// Leaf: ?IsSleepingTime@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00896210. Identity (NF435, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: village.GetPThing() (CScriptThing slot 11, +0x2c), ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z, then
//   ??$PeekTC@$$CBVCTCVillage@@@CThing@@QBE_NPAPBVCTCVillage@@@Z (inline in retail: bit 0x22 TCI_VILLAGE, const LowerBound 0x004365B0) and
//   ?IsNightTime@CTCVillage@@QBE_NXZ (retail 0x00658D07, the call target); else false. The DoErrorMessage branches are debug-only.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_TCI
#define FABLE_RETAIL_CTCVILLAGE_ISNIGHTTIME
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CTCVillage.h"

bool CGameScriptInterface::IsSleepingTime(const CScriptThing& village) const
{
    CThing* thing = village.GetPThing();
    if (IS_THING_ALIVE(thing)) {
        const CTCVillage* village_tc;
        if (thing->PeekTC(&village_tc))
            return village_tc->IsNightTime();
    }
    return false;
}
