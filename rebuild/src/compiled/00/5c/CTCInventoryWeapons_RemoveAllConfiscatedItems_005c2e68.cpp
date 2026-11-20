#pragma optimize("s", on)
// CTCInventoryWeapons::RemoveAllConfiscatedItems() -- empties the base inventory's confiscated items, then forgets the confiscated active melee/ranged weapon defs
// Leaf: ?RemoveAllConfiscatedItems@CTCInventoryWeapons@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C2E68, length 24 bytes. Size TU (`and dword ptr [esi+160h],0` zero
// stores): `optimize("s",on)`.
// Label: the catalog name "CTCVillage::InitialiseSecuritySweeps" is propagated. 0x005C2E68 is slot 74 of the
// CTCInventoryWeapons vtable 0x01253FF4; CTCInventoryClothing's slot 74 is the base body 0x005BF969 called here
// (ConfiscatedItems.clear() on the vector at +0xAC, next to the landed slot-73 ReturnAllConfiscatedItems 0x005BF91A),
// and the PDB lists `?RemoveAllConfiscatedItems@CTCInventoryWeapons@@UAEXXZ`.
// Callees: NInventory::CTCInventoryBase::RemoveAllConfiscatedItems() @ 0x005BF969 (the base override called directly).
// Layout: engine/CTCInventoryWeapons.h (ConfiscatedActiveMeleeWeaponDefIndex / ConfiscatedActiveRangedWeaponDefIndex +0x160 / +0x164).
// Naming (NF158, 2026-11-20, q5158): the local CTCInventoryBase / CTCInventoryWeapons models replaced by the shared
//   engine/CTCInventoryWeapons.h (on engine/NInventory_CTCInventoryBase.h; opt-in FABLE_RETAIL_CTCINVENTORYWEAPONS_REMOVEALLCONFISCATEDITEMS
//   declares the slot-74 override), so the base call binds 0x005BF969 as ?RemoveAllConfiscatedItems@CTCInventoryBase@NInventory@@UAEXXZ
//   (the self row's Ego_r.pdb spelling) instead of the global ?RemoveAllConfiscatedItems@CTCInventoryBase@@UAEXXZ stand-in.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_REMOVEALLCONFISCATEDITEMS
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"

void CTCInventoryWeapons::RemoveAllConfiscatedItems()
{
    NInventory::CTCInventoryBase::RemoveAllConfiscatedItems();
    ConfiscatedActiveMeleeWeaponDefIndex = 0;
    ConfiscatedActiveRangedWeaponDefIndex = 0;
}
