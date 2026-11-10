// CTCInventoryWeapons::GetFreeAugmentationSlotForWeapon(long weaponDefIndex) -- the first free augmentation slot of a weapon of that def: the live active melee or ranged weapon when it is of that def, otherwise a weapon object created from the inventory for the question (and returned to the inventory afterwards); -1 when it has no augmentations component
// Leaf: ?GetFreeAugmentationSlotForWeapon@CTCInventoryWeapons@@QAEJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C4671, 246 bytes, thiscall `ret 4`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Identity: Ego_r.pdb / FableWin.pdb ?GetFreeAugmentationSlotForWeapon@CTCInventoryWeapons@@QAEJJ@Z (Ego_r donor 0x00627D5F, after
//   AugmentWeapon 0x005C44FA whose first half is the same weapon search).
// Callees: IS_THING_ALIVE 0x004CC340, CBaseIntelligentPointer::GetPItem 0x00A01B50, CTCInventoryBase::CreateInventoryObject 0x005BE8F3
//   (protected, Ego_r IAE), std::bitset<274>::test 0x00410DE0, CVectorMap LowerBound const 0x004365B0 (PeekTC<const
//   CTCObjectAugmentations>, interface 0xBF) / non-const 0x0040F020 (GetTC<CTCInventoryItem>, interface 0x1A),
//   CTCObjectAugmentations::GetFreeSlot 0x00765D50, CTCInventoryItem::ReturnToInventory 0x005D8E50.
// Shape: the two weapon tests share one GetPItem tail (cross-jumped); the created flag is a frame byte and the result a frame word
//   preset to -1; the component lookups are the expanded PeekTC / GetTC (FABLE_RETAIL_CTHING_GETTC_EXPANDED).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CREATEOBJECT
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CTCINVENTORYITEM
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCObjectAugmentations.h"
#include "engine/CTCInventoryItem.h"

template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);          // 0x004CC340

long CTCInventoryWeapons::GetFreeAugmentationSlotForWeapon(long weaponDefIndex)
{
    long freeSlot = -1;
    bool created = false;
    CThing* weapon;
    if (IS_THING_ALIVE(PActiveMeleeWeapon) && PActiveMeleeWeapon.Get()->GetDefGlobalIndex() == weaponDefIndex)
        weapon = PActiveMeleeWeapon.Get();
    else if (IS_THING_ALIVE(PActiveRangedWeapon) && PActiveRangedWeapon.Get()->GetDefGlobalIndex() == weaponDefIndex)
        weapon = PActiveRangedWeapon.Get();
    else {
        weapon = CreateInventoryObject(weaponDefIndex);       // a temporary object of that def
        created = true;
    }
    const CTCObjectAugmentations* augmentations;
    if (weapon->PeekTC(&augmentations))
        freeSlot = augmentations->GetFreeSlot();
    if (created) {                                            // hand the temporary object back to the inventory
        CTCInventoryItem* item = 0;
        weapon->GetTC(&item);
        item->ReturnToInventory();
    }
    return freeSlot;
}