// NInventory::CTCInventoryBase::CreateInventoryObject(long itemDefIndex) -- creates the world object of an owned item type at the owner's position (for the owner's player) and restores its per-item data: weapon kills / time wielded, the augmentation slots and their contents, the trophy's best witness count; then connects the object's inventory-item component to this inventory. Returns the thing (0 when no such item is owned; the unchecked thing when it is not alive).
// Leaf: ?CreateInventoryObject@CTCInventoryBase@NInventory@@IAEPAVCThing@@J@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BE8F3, 482 bytes, thiscall `ret 4`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Identity: Ego_r.pdb (donor 0x0062118F) / FableWin.pdb ?CreateInventoryObject@CTCInventoryBase@NInventory@@IAEPAVCThing@@J@Z (protected).
// Callees: GetNumberOfItemsOfTypeInInventory 0x005BDF08, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CThing::GetPos 0x004C73D0, CThingObject::Create 0x00703210 (static __fastcall), IS_THING_ALIVE<CThing*> 0x0040DCE0,
//   GetPerItemDataForItem 0x005BE8D3, std::bitset<274>::test 0x00410DE0, CVectorMap::LowerBound 0x0040F020,
//   CTCObjectAugmentations::GetMaximumNumberOfSlots 0x00765D90 / GetNumberOfSlots 0x00765D80 / AddNewSlot 0x00766D70 /
//   AttachAugmentationToSlot 0x00768120 / RemoveAugmentationFromSlot 0x007681D0, CTCInventoryItem::ConnectToInventory 0x005D8E40.
// Shape: `thing` starts as 0 and lives in ebx (its zero is also the two zero arguments of Create); the same-TU body of
//   IS_THING_ALIVE<CThing*> (out of line in retail, the landed 0x0040DCE0 body) lets the thing stay in the register and be passed
//   through a frame copy; every component fetch is the expanded out-parameter GetTC (the keys share the dead frame words); the
//   maximum slot count is a local evaluated BEFORE the vector size (`mov [ebp-4],eax` then size), and STLport's min(max, size)
//   picks by address; the inventory-item pointer is zeroed first and used unconditionally; the item block runs even without
//   per-item data. CTCTrophy has no shared header (local minimal Ego_r.pdb model below).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA
#define FABLE_RETAIL_CTCINVENTORYBASE_CREATEOBJECT
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_MEMBERS
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CONNECTTOINVENTORY
#define FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_SLOTS
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingObject.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCObjectAugmentations.h"

// CTCTrophy has no shared header: minimal model from Ego_r.pdb (CTCBase 0x0C, PDef +0x0C, NonMinigameDeedHandle +0x10,
// MinigameInProgress +0x14, BestWitnessesAheadToDate +0x18).
class CTCTrophy : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_TROPHY; }
    void* PDef;                                 // +0x0C CDefPointer<const CTrophyDef>
    long NonMinigameDeedHandle;                 // +0x10
    bool MinigameInProgress;                    // +0x14
    long BestWitnessesAheadToDate;              // +0x18
};

template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);          // 0x0040DCE0 (CThing*)
#pragma auto_inline(off)
template <> inline bool FABLE_FASTCALL IS_THING_ALIVE<CThing*>(CThing* const& thing)
{
    if (thing != 0 && thing->IsDead() == 0)
        return true;
    return false;
}
#pragma auto_inline(on)

namespace NInventory {
CThing* CTCInventoryBase::CreateInventoryObject(long itemDefIndex)
{
    CThing* thing = 0;
    if (GetNumberOfItemsOfTypeInInventory(itemDefIndex) > 0) {
        thing = CThingObject::Create(itemDefIndex, ParentThing->GetPos(), ParentThing->GetPlayerNumber(), 0, 0, "");
        if (IS_THING_ALIVE(thing)) {
            const CPerItemData* data = GetPerItemDataForItem(itemDefIndex);
            if (data) {
                CTCWeapon* weapon;
                if (thing->GetTC(&weapon)) {
                    weapon->NumThingsKilled = data->NumberOfKills;
                    weapon->TimeWielded = data->TimeWielded;
                }
                CTCObjectAugmentations* augmentations;
                if (thing->GetTC(&augmentations)) {
                    long max = augmentations->GetMaximumNumberOfSlots();
                    long count = std::min(max, (long)data->Augmentations.size());
                    while (augmentations->GetNumberOfSlots() < count)
                        augmentations->AddNewSlot();
                    for (long i = 0; i < count; ++i) {
                        if (data->Augmentations[i] > 0)
                            augmentations->AttachAugmentationToSlot(data->Augmentations[i], i);
                        else
                            augmentations->RemoveAugmentationFromSlot(i);
                    }
                }
                CTCTrophy* trophy;
                if (thing->GetTC(&trophy))
                    trophy->BestWitnessesAheadToDate = data->BestTrophyWitnessesAheadToDate;
            }
            CTCInventoryItem* item = 0;
            thing->GetTC(&item);
            item->ConnectToInventory(*this);
        }
    }
    return thing;
}
}