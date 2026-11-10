// CTCInventoryWeapons::RemoveItemFromInventory(long itemDefIndex, long count) -- removes the items through the base inventory, then for each live active weapon (melee, then ranged) of which none are left: takes it off the owner's carrier if carried (refreshing the sheathes), takes the weapons out of their sheathes and kills the weapon thing; finally refreshes the sheathed weapons and sets the active weapons up on the carrier again
// Leaf: ?RemoveItemFromInventory@CTCInventoryWeapons@@UAEXJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005CB128, 390 bytes, thiscall `ret 8`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Identity: slot 79 of the CTCInventoryWeapons vtable 0x01253FF4 (the NInventory::CTCInventoryBase slot RemoveItemFromInventory,
//   base 0x005BF3C6 called qualified first); Ego_r.pdb / FableWin.pdb ?RemoveItemFromInventory@CTCInventoryWeapons@@UAEXJJ@Z.
// Callees: CTCInventoryBase::RemoveItemFromInventory 0x005BF3C6, IS_THING_ALIVE 0x004CC340, GetPItem 0x00A01B50,
//   CTCInventoryBase::GetNumberOfItemsOfTypeInInventory 0x005BDF08 (const), bitset test 0x00410DE0 + LowerBound 0x0040F020
//   (GetTC<CTCCarrying>, interface 0x46), CTCCarrying::IsCarryingThing 0x006A1060 / RemoveThing 0x006A19B0,
//   RefreshSheathedWeapons 0x005C9962, RemoveWeaponsFromSheathes 0x005C552C, CThing::Kill 0x004C9B80,
//   SetupActiveWeaponsOnCarrier 0x005CA68E.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CTCCARRYING_REMOVETHING
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCCarrying.h"

template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);          // 0x004CC340

void CTCInventoryWeapons::RemoveItemFromInventory(long itemDefIndex, long count)
{
    NInventory::CTCInventoryBase::RemoveItemFromInventory(itemDefIndex, count);
    if (IS_THING_ALIVE(PActiveMeleeWeapon) && GetNumberOfItemsOfTypeInInventory(PActiveMeleeWeapon.Get()->GetDefGlobalIndex()) == 0) {
        CTCCarrying* carrying;
        if (ParentThing->GetTC(&carrying) && carrying->IsCarryingThing(*PActiveMeleeWeapon.Get())) {
            carrying->RemoveThing(*PActiveMeleeWeapon.Get());
            RefreshSheathedWeapons();
        }
        RemoveWeaponsFromSheathes();
        PActiveMeleeWeapon.Get()->Kill(false);
    }
    if (IS_THING_ALIVE(PActiveRangedWeapon) && GetNumberOfItemsOfTypeInInventory(PActiveRangedWeapon.Get()->GetDefGlobalIndex()) == 0) {
        CTCCarrying* carrying;
        if (ParentThing->GetTC(&carrying) && carrying->IsCarryingThing(*PActiveRangedWeapon.Get())) {
            carrying->RemoveThing(*PActiveRangedWeapon.Get());
            RefreshSheathedWeapons();
        }
        RemoveWeaponsFromSheathes();
        PActiveRangedWeapon.Get()->Kill(false);
    }
    RefreshSheathedWeapons();
    SetupActiveWeaponsOnCarrier();
}