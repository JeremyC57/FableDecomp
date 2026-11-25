// CGameScriptInterface::IsEntityWieldingMeleeWeapon(const CScriptThing& entity) const -- CTCCarrying::IsThingCarryingMeleeWeapon for a live thing, else false
// Leaf: ?IsEntityWieldingMeleeWeapon@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00890220. Identity (NF433, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: entity.GetPThing() (CScriptThing slot 11, +0x2c), ??$IS_THING_ALIVE@PBVCThing@@@@YI_NABQBVCThing@@@Z, then the static
//   ?IsThingCarryingMeleeWeapon@CTCCarrying@@SI_NABVCThing@@@Z (retail 0x006A15A0); else false.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCCARRYING_ISTHINGCARRYINGMELEEWEAPON
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CTCCarrying.h"

bool CGameScriptInterface::IsEntityWieldingMeleeWeapon(const CScriptThing& entity) const
{
    const CThing* thing = entity.GetPThing();
    if (IS_THING_ALIVE(thing))
        return CTCCarrying::IsThingCarryingMeleeWeapon(*thing);
    return false;
}
