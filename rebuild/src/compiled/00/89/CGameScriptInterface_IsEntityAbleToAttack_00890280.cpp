// CGameScriptInterface::IsEntityAbleToAttack(const CScriptThing& entity) const -- NCreatureMode::IsAbleToMove for a live thing, else false
// Leaf: ?IsEntityAbleToAttack@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00890280. Identity (NF433, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: entity.GetPThing() (CScriptThing slot 11, +0x2c), ??$IS_THING_ALIVE@PBVCThing@@@@YI_NABQBVCThing@@@Z, then
//   ?IsAbleToMove@NCreatureMode@@YI_NABVCThing@@@Z (retail 0x0085F3E0) tested (true / false); else false.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_NCREATUREMODE_ISABLETOMOVE
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/NCreatureMode.h"

bool CGameScriptInterface::IsEntityAbleToAttack(const CScriptThing& entity) const
{
    const CThing* thing = entity.GetPThing();
    if (IS_THING_ALIVE(thing))
    {
        if (NCreatureMode::IsAbleToMove(*thing))
            return true;
        return false;
    }
    return false;
}
