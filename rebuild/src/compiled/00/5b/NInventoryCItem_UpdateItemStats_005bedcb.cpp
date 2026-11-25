// NInventory::CItem::UpdateItemStats(const CThing& item) -- refresh the item's per-item data from the thing it was made from: a weapon
//   copies its kill count and wield time and its augmentations (else clears them), a trophy its best witnesses-ahead count when positive, a
//   fish takes CTCFish::MostRecentFishWeight when heavier (then zeroes it); while the quest-persistent ref count is unset (-1, or no data
//   yet), an inventory item that is not to persist until its quest completes marks the item quest-persistent unless the quest manager is
//   between the start and finish screens, and resets the ref count to 0
// Leaf: ?UpdateItemStats@CItem@NInventory@@QAEXABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BEDCB, 574 bytes; size-optimised frame-pointer TU (`push 48h ; pop esi`, `leave`).
// Identity (NF92, q11209): Ego_r.pdb 0x00621B5D + FableWin.pdb ?UpdateItemStats@CItem@NInventory@@QAEXABVCThing@@@Z (public); the landed
//   0x005BF556 (CItem::AddItem) and the ported 0x005BF16E (CCategory::UpdateItemStatsForThing) bind this address under it. Ego_d.pdb names
//   the parameter `item` and the locals ptc_weapon / ptc_trophy / ptc_fish, then ptc_augmentations and ptc_inventory_item in inner scopes.
// Callees: std::bitset<274>::test 0x00410DE0 and CVectorMap::LowerBound 0x004365B0 (the const PeekTC expansion), operator new,
//   CPerItemData ctor 0x005C14A6, CCountedPointer<CPerItemData>::Reset 0x005C1946, CTCObjectAugmentations::GetAugmentations 0x00766920,
//   vector<long>::erase 0x00440590 (clear), CTCInventoryItem::IsToNotPersistUntilQuestCompleted 0x005D9270 (Ego_r 0x0055A1C0 / FableWin
//   0x01E01220; was bound as the made-up CTCWeapon::IsAnalogueLoading_005d9270), CQuestManager::IsBetweenQuestStartAndFinishScreens
//   0x004AF290 on CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass; CTCFish::MostRecentFishWeight 0x013BAE80 (Ego_d.pdb PUB32).
// Port (NF92): was a local model of a dozen classes (global CItem with a CThing* parameter, CThing / CTCWeapon / CTCTrophy / CPerItemData /
//   CQuestManager stand-ins); now on the shared headers with the NF90 opt-in _CITEM_UPDATEITEMSTATS and the NF92 opt-ins
//   CTCINVENTORYITEM_ISTONOTPERSISTUNTILQUESTCOMPLETED / CTCFISH_MOSTRECENTFISHWEIGHT.
#include "fable_compiler.h"
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_MEMBERS
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_ISTONOTPERSISTUNTILQUESTCOMPLETED
#define FABLE_RETAIL_CTCFISH_MOSTRECENTFISHWEIGHT
#define FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_GETAUGMENTATIONS
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA_CTOR
#define FABLE_RETAIL_NINVENTORY_CITEM_UPDATEITEMSTATS
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCTrophy.h"
#include "engine/CTCFish.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCObjectAugmentations.h"
#include "engine/CQuestManager.h"
#include "engine/NInventory_CTCInventoryBase.h"

namespace NInventory {
void CItem::UpdateItemStats(const ::CThing& item)
{
    const CTCWeapon* ptc_weapon;
    const CTCTrophy* ptc_trophy;
    if (item.PeekTC(&ptc_weapon)) {
        if (PPerItemData.IsNull())
            PPerItemData.Reset(new CPerItemData);
        PPerItemData->NumberOfKills = ptc_weapon->NumThingsKilled;
        PPerItemData->TimeWielded = ptc_weapon->TimeWielded;
        const CTCObjectAugmentations* ptc_augmentations;
        if (item.PeekTC(&ptc_augmentations))
            ptc_augmentations->GetAugmentations(PPerItemData->Augmentations);
        else
            PPerItemData->Augmentations.clear();
    } else if (item.PeekTC(TCI_TROPHY, &ptc_trophy)) {
        if (ptc_trophy->BestWitnessesAheadToDate > 0) {
            if (PPerItemData.IsNull())
                PPerItemData.Reset(new CPerItemData);
            PPerItemData->BestTrophyWitnessesAheadToDate = ptc_trophy->BestWitnessesAheadToDate;
        }
    } else if (item.HasTC(CTCFish::StatGetInterfaceType())) {
        if (PPerItemData.IsNull())
            PPerItemData.Reset(new CPerItemData);
        if (CTCFish::MostRecentFishWeight > PPerItemData->BestFishWeight) {
            PPerItemData->BestFishWeight = CTCFish::MostRecentFishWeight;
            CTCFish::MostRecentFishWeight = 0.0f;
        }
    }
    if (PPerItemData.IsNull() || PPerItemData->QuestPersistentRefCount == -1) {
        const CTCInventoryItem* ptc_inventory_item;
        if (item.PeekTC(&ptc_inventory_item) && ptc_inventory_item && ptc_inventory_item->IsToNotPersistUntilQuestCompleted()) {
            if (PPerItemData.IsNull())
                PPerItemData.Reset(new CPerItemData);
            if (CQuestManager::Get().IsBetweenQuestStartAndFinishScreens())
                PPerItemData->QuestPersistent = false;
            else
                PPerItemData->QuestPersistent = true;
            PPerItemData->QuestPersistentRefCount = 0;
        }
    }
}
}
