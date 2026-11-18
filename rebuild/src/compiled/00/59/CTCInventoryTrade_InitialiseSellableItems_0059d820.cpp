// CTCInventoryTrade::InitialiseSellableItems() -- select the sell category, gather every item of the hero's three inventory components, add each sellable one (positive def index) to the trade inventory and total their counts as the sell count, then refresh the unique count of the selected category and the category count
// Leaf: ?InitialiseSellableItems@CTCInventoryTrade@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059D820, 397 bytes, plain `ret`. Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)` (`push 11h ; pop ebx`).
// Identity: Ego_r.pdb + FableWin.pdb ?InitialiseSellableItems@CTCInventoryTrade@@QAEXXZ (engine/CTCInventoryTrade.h PCINVENTORY).
// Callees: CPlayerManager::GetMainPlayer() const @ 0x00449960, CPlayer::PeekPControlledCreature @ 0x00487DD0, std::bitset<274>::test @
//   0x00410DE0 and the const CVectorMap LowerBound @ 0x004365B0 (the inline PeekTC), CTCInventoryBase::GetAllItems(vector<pair<long, long> >&)
//   const @ 0x005BF482, CTCInventoryBase::GetBuildingThing @ 0x005BC4C4 (Ego_r + FableWin), CTCInventoryItem::IsSellable @ 0x005D9B30
//   (static `__fastcall`), CTCInventoryTrade::AddItemOfTypeToInventory @ 0x0059C6B0 (private), CTCInventoryBase::GetSelectedCategory @
//   0x005BCACE / GetCategoryCount @ 0x005BCE2F, NInventory::CCategory::GetUniqueItemCountInCategory @ 0x005BC8AC, the CRT free thunk @
//   0x00BFEA14 (the item vector's storage).
// Shape: the three lookups are PeekTC with the result ignored into one pointer initialised to 0 once (an absent component keeps the
//   previous pointer); `total` is declared before the hero lookup; the building's HasTC(TCI_SHOP) is evaluated and discarded; the item
//   vector escapes (GetAllItems is not a TU-mate here), so its start and size are re-read every pass.
// NF87: moved off the row-local CThing / CPlayer / CPlayerManager / CMainGameComponent / CTCInventoryBase / CTCInventoryTrade / CCategory /
//   CTCInventoryItem / item-vector models onto the shared headers (overlay opt-ins NINVENTORY_CCATEGORY_ACCESSORS,
//   CTCINVENTORYBASE_CATEGORYCOUNT), so 0x005BC4C4 binds GetBuildingThing.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYBASE_CATEGORYCOUNT
#define FABLE_RETAIL_NINVENTORY_CCATEGORY_ACCESSORS
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_PCINVENTORY
#define FABLE_RETAIL_CTCINVENTORYTRADE_ADDITEMOFTYPE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CTCINVENTORYITEM_ISSELLABLE
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"

void CTCInventoryTrade::InitialiseSellableItems()
{
    CategoryIdent = (EInventoryCategoryIdentifier)12;
    long total = 0;
    const CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    std::vector<std::pair<long, long> > items;
    const NInventory::CTCInventoryBase* inventory = 0;
    hero->PeekTC(TCI_INVENTORY, &inventory);
    inventory->GetAllItems(items);
    hero->PeekTC(TCI_INVENTORY_WEAPONS, &inventory);
    inventory->GetAllItems(items);
    hero->PeekTC(TCI_INVENTORY_CLOTHING, &inventory);
    inventory->GetAllItems(items);
    GetBuildingThing()->HasTC(TCI_SHOP);
    for (unsigned long i = 0; i < items.size(); ++i) {
        long defIndex = items[i].first;
        if (defIndex > 0 && CTCInventoryItem::IsSellable(defIndex)) {
            AddItemOfTypeToInventory(defIndex, items[i].second);
            total += items[i].second;
        }
    }
    ItemSellCount = total;
    NInventory::CCategory* category = GetSelectedCategory();
    if (category)
        ItemSellUniqueCount = category->GetUniqueItemCountInCategory();
    CategoryCount = GetCategoryCount();
}
