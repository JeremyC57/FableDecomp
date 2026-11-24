// CThingCreatureBase::UnsheatheMeleeWeapon() -- unless the creature is the player character or it already carries a melee weapon, sheathe everything and draw the creature definition's default melee weapons (primary, then secondary)
// Leaf: ?UnsheatheMeleeWeapon@CThingCreatureBase@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006649C0, length 79 bytes (+ int3 padding).
// Callees: the creature's virtual slot 77 (+0x134) IsPlayerCharacter (engine/CThingCreatureBase.h FABLE_RETAIL_CTHING_ORIGINAL_VTABLE; the
//   CThingAICreature.h / CThingPlayerCreature.h overrides at slot 77 are 0x00835940 / 0x0052ABE0; the landed sibling 0x00664A10 calls the same
//   slot by that name), ?IsThingCarryingMeleeWeapon@CTCCarrying@@SI_NABVCThing@@@Z @ 0x006A15A0 (Ego_r.pdb 0x006C6040 / FableWin.pdb 0x02214C70,
//   public static fastcall; the thing in ecx), ?SheatheAllWeapons@CThingCreatureBase@@QAEXXZ @ 0x006647F0,
//   ?UnsheatheWeapon@CThingCreatureBase@@IAEXJ_N@Z @ 0x006648D0 (protected IAE in all three PDBs).
// Name: ?UnsheatheMeleeWeapon@CThingCreatureBase@@QAEXXZ in Ego_r.pdb / FableWin.pdb / Ego_d.pdb (public; Ego_r donor order UnsheatheWeapon
//   0x00540710 / UnsheatheMeleeWeapon 0x00540810 = retail 0x006648D0 / 0x006649C0); HEAD's row emitted the same spelling from a row-local
//   CThingCreatureBase model (77 placeholder virtuals, `m_pDef` at +0xE0).
// Layout: engine/CThingCreatureBase.h retail branch (PDef = CDefPointer<const CThingCreatureDef> at +0xE0) with the NF253 opt-in
//   FABLE_RETAIL_CTHINGCREATUREBASE_UNSHEATHEMELEEWEAPON (this member), NF247's _UNSHEATHERANGEDWEAPON (UnsheatheWeapon, protected) and _BEDSLEEP
//   (SheatheAllWeapons); engine/CThingCreatureDef.h (PrimaryMeleeWeaponDef +0xD8 / SecondaryMeleeWeaponDef +0xDC, PDB +224 / +228);
//   engine/CTCCarrying.h FABLE_RETAIL_CTCCARRYING_ISTHINGCARRYINGMELEEWEAPON.
// NF253 (q12711): ported off the HEAD row-local CThingCreatureBase / CThingCreatureDef models onto the shared headers, the 0x00664A10
//   recipe (NF247); 0x006A15A0 now binds its PDB spelling (was the free stand-in ?IsThingCarryingMeleeWeapon@@YI_NABVCThingCreatureBase@@@Z).
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
#define FABLE_RETAIL_CTHINGCREATUREBASE_UNSHEATHEMELEEWEAPON
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCCARRYING_ISTHINGCARRYINGMELEEWEAPON
#define FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS
#include "engine/CThingCreatureBase.h"
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CTCBase.h"
#include "engine/CTCCarrying.h"

void CThingCreatureBase::UnsheatheMeleeWeapon()
{
    if (IsPlayerCharacter())
        return;
    if (CTCCarrying::IsThingCarryingMeleeWeapon(*this))
        return;
    SheatheAllWeapons();
    UnsheatheWeapon(PDef->PrimaryMeleeWeaponDef, true);
    UnsheatheWeapon(PDef->SecondaryMeleeWeaponDef, false);
}
