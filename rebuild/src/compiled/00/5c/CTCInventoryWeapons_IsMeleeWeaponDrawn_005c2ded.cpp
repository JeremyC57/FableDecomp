// CTCInventoryWeapons::IsMeleeWeaponDrawn() const -- whether the inventory's parent thing carries a melee weapon in hand
// Leaf: ?IsMeleeWeaponDrawn@CTCInventoryWeapons@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005c2ded, 8 bytes: `mov ecx, [ecx+4]` (CTCBase::ParentThing) ; `jmp 0x006A15A0`
//   (?IsThingCarryingMeleeWeapon@CTCCarrying@@SI_NABVCThing@@@Z, Ego_r.pdb 0x006C6040; public static fastcall).
// NF240 (q12673, NF239 L38): named from Ego_r.pdb 0x00624F7A / FableWin.pdb 0x0237C819 ?IsMeleeWeaponDrawn@CTCInventoryWeapons@@QBE_NXZ
//   (public const) and the landed binder NPlayerGui::CDrawAbilityButtons::UpdateWeaponAbilities 0x0064B1AC (the paired call at retail
//   0x0064B293 binds this spelling); was the stand-in forwarder ?Run@T@@QAEXXZ (catalog _global::Forward_IsThingCarryingMeleeWeapon_via0x4_005c2ded).
//   On engine/CTCInventoryWeapons.h (the binder's PASS27 opt-in) + engine/CTCCarrying.h; the parent accessor is CTCBase::GetParentThing.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CDRAWABILITYBUTTONS_WEAPONABILITIES_PASS27
#define FABLE_RETAIL_CTCCARRYING_ISTHINGCARRYINGMELEEWEAPON
#include "engine/ETCInterfaceType.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCCarrying.h"

bool CTCInventoryWeapons::IsMeleeWeaponDrawn() const
{
    return CTCCarrying::IsThingCarryingMeleeWeapon(GetParentThing());
}
