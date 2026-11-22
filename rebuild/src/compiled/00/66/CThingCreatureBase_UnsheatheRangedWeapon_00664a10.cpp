// CThingCreatureBase::UnsheatheRangedWeapon() -- unless the creature is the player character or it already carries a ranged weapon, sheathe everything and draw the creature definition's ranged weapon
// Leaf: ?UnsheatheRangedWeapon@CThingCreatureBase@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00664A10, length 57 bytes (+ int3 padding).
// Callees: the creature's virtual slot 77 (+0x134) IsPlayerCharacter (engine/CThingCreatureBase.h FABLE_RETAIL_CTHING_ORIGINAL_VTABLE; the
//   CThingAICreature.h / CThingPlayerCreature.h overrides at slot 77 are 0x00835940 / 0x0052ABE0; landed 0x00847DF0 calls the same slot by
//   that name), ?IsThingCarryingRangedWeapon@CTCCarrying@@SI_NABVCThing@@@Z @ 0x006A1620 (Ego_r.pdb 0x006C60F0 / FableWin.pdb 0x02214CE0; the
//   thing in ecx), ?SheatheAllWeapons@CThingCreatureBase@@QAEXXZ @ 0x006647F0, ?UnsheatheWeapon@CThingCreatureBase@@IAEXJ_N@Z @ 0x006648D0
//   (protected IAE in Ego_r.pdb / FableWin.pdb / Ego_d.pdb; bound QAE here as by its self row and every landed binder: NF247 NAME_FIXES).
// Leaf (NF247, q12679): ?UnsheatheRangedWeapon@CThingCreatureBase@@QAEXXZ in Ego_r.pdb / FableWin.pdb / Ego_d.pdb (public); HEAD's row emitted
//   the same spelling from a row-local CThingCreatureBase model (77 placeholder virtuals, `m_pDef` at +0xE0).
// Layout (NF247, q12679 header port): engine/CThingCreatureBase.h retail branch (PDef = CDefPointer<const CThingCreatureDef> at +0xE0) with the
//   NF247 opt-in FABLE_RETAIL_CTHINGCREATUREBASE_UNSHEATHERANGEDWEAPON (this member and UnsheatheWeapon) and the existing _BEDSLEEP
//   (SheatheAllWeapons); engine/CThingCreatureDef.h (RangedWeaponDef +0xE0, PDB +232); engine/CTCCarrying.h FABLE_RETAIL_CTCCARRYING_PLAYERHIT_PASS26.
// Shape: two early returns on the bool tests (`jne` to the shared `pop esi ; ret`), the definition pointer re-read from
// `this` for each UnsheatheWeapon call (a call may change it).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_BEDSLEEP
#define FABLE_RETAIL_CTHINGCREATUREBASE_UNSHEATHERANGEDWEAPON
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCCARRYING_PLAYERHIT_PASS26
#define FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CTCBase.h"
#include "engine/CTCCarrying.h"

void CThingCreatureBase::UnsheatheRangedWeapon()
{
    if (IsPlayerCharacter())
        return;
    if (CTCCarrying::IsThingCarryingRangedWeapon(*this))
        return;
    SheatheAllWeapons();
    UnsheatheWeapon(PDef->RangedWeaponDef, true);
}
