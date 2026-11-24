// CTCInventoryWeapons::IsRangedWeaponDrawn() const -- whether the inventory's parent thing carries a ranged weapon in hand
// Leaf: ?IsRangedWeaponDrawn@CTCInventoryWeapons@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005c2df5, 8 bytes: `mov ecx, [ecx+4]` (CTCBase::ParentThing) ; `jmp 0x006A1620`
//   (?IsThingCarryingRangedWeapon@CTCCarrying@@SI_NABVCThing@@@Z, Ego_r.pdb 0x006C60F0 / FableWin.pdb 0x02214CE0; public static fastcall).
// NF240 (q12673, NF239 L39): named from Ego_r.pdb 0x00624F82 / FableWin.pdb 0x0237C837 ?IsRangedWeaponDrawn@CTCInventoryWeapons@@QBE_NXZ
//   (public const) and the landed binder NPlayerGui::CDrawAbilityButtons::UpdateWeaponAbilities 0x0064B1AC (the paired call at retail
//   0x0064B2B7 binds this spelling); was the stand-in forwarder ?Run@T@@QAEXXZ (catalog _global::Forward_IsThingCarryingMeleeWeapon_via0x4_005c2df5).
//   On engine/CTCInventoryWeapons.h (the binder's PASS27 opt-in) + engine/CTCCarrying.h; the parent accessor is CTCBase::GetParentThing.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CDRAWABILITYBUTTONS_WEAPONABILITIES_PASS27
#define FABLE_RETAIL_CTCCARRYING_PLAYERHIT_PASS26
#include "engine/ETCInterfaceType.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCCarrying.h"

bool CTCInventoryWeapons::IsRangedWeaponDrawn() const
{
    return CTCCarrying::IsThingCarryingRangedWeapon(GetParentThing());
}
