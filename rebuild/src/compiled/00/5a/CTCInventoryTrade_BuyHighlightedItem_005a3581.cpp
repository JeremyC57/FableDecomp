// CTCInventoryTrade::BuyHighlightedItem() -- buy the highlighted item of the selected category: when the hero can pay the shop's price and the item's inventory can take it, the shop sells it, the hero pays, the item is added and the trade lists are rebuilt; the owned count is refreshed
// Leaf: ?BuyHighlightedItem@CTCInventoryTrade@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A3581, 435 bytes, plain `ret`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Identity: Ego_r.pdb + FableWin.pdb ?BuyHighlightedItem@CTCInventoryTrade@@QAEXXZ.
// Callees: GetSelectedCategory 0x005BCACE, CCategory::GetItemInSlotWithIndex 0x005BC9B0 (the inline GetHighlightedItemIndex reads +0x24),
//   GetBuildingThing 0x005BC4C4, bitset::test 0x00410DE0 + LowerBound 0x0040F020 (the inline GetTC), CTCShop::GetCurrentSellPrice
//   0x007743B0 / SellItem 0x00774730, GetMainPlayer 0x00449960, CPlayer::GetPControlledCreature 0x00487DC0,
//   CTCInventoryItem::GetInventoryType 0x005D99D0, IsSpaceInInventoryForItem 0x005BF2ED, CTCHeroStats::AddMoney 0x0057B327, the virtual
//   AddItemToInventory(long, long, bool, bool, long, bool), ResetTradeLists 0x0059D9AD, InitialiseBuyableItems 0x005A30A1,
//   InitialiseSellableItems 0x0059D820, GetHighlightedItem 0x005BCAE3, ResetInventoryAnim 0x005BCD2D, SetNumberOfSelectedItemOwned
//   0x005BE284, the CRT free thunk 0x00BFEA14 (the item vector's storage).
// Name fix (agentNF89, q11164): moved off the row-local CThing / CPlayer / CPlayerManager / CMainGameComponent / CTCShop / CTCHeroStats
//   (renaming-macro donor) / CTCInventoryBase / CTCInventoryTrade / NInventory::CCategory / pair-vector models onto the shared headers, so
//   0x007743B0 binds ?GetCurrentSellPrice@CTCShop@@QBEJJJ@Z (Ego_r.pdb + FableWin.pdb; was the GetCurrentBuyPrice_007743b0 stand-in) and
//   the inventory callees bind their NInventory::CTCInventoryBase spellings. Overlay opt-ins CTCINVENTORYTRADE_BUYHIGHLIGHTEDITEM,
//   CTCINVENTORYBASE_RESETINVENTORYANIM, CCATEGORY_ITEMINSLOT.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMINSLOT
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYBASE_HIGHLIGHTEDITEM
#define FABLE_RETAIL_CTCINVENTORYBASE_RESETINVENTORYANIM
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_PCINVENTORY
#define FABLE_RETAIL_CTCINVENTORYTRADE_BUYHIGHLIGHTEDITEM
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
#define FABLE_RETAIL_CTCSHOP_SELLPRICE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CTCHEROSTATS_GETMONEY
#define FABLE_RETAIL_CTCHEROSTATS_ADDMONEY
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"

void CTCInventoryTrade::BuyHighlightedItem()
{
    NInventory::CCategory* category = GetSelectedCategory();
    if (category) {
        long item = category->GetItemInSlotWithIndex(category->GetHighlightedItemIndex());
        if (item > 0) {
            CThing* owner = GetBuildingThing();
            CTCShop* shop = 0;
            owner->GetTC(TCI_SHOP, &shop);
            long price = shop->GetCurrentSellPrice(item, 1);
            CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCHeroStats* stats = 0;
            hero->GetTC(TCI_HERO_STATS, &stats);
            if (stats->GetMoney() >= price) {
                NInventory::CTCInventoryBase* inventory = 0;
                hero->GetTC(CTCInventoryItem::GetInventoryType(item), &inventory);
                if (inventory->IsSpaceInInventoryForItem(item)) {
                    shop->SellItem(item);
                    stats->AddMoney(-price, true);
                    inventory->AddItemToInventory(item, 1, false, true, price, false);
                    ResetTradeLists();
                    std::vector<std::pair<long, long> > buyable;
                    InitialiseBuyableItems(buyable);
                    InitialiseSellableItems();
                    ResetInventoryAnim(GetHighlightedItem());
                }
                SetNumberOfSelectedItemOwned(GetHighlightedItem());
            }
        }
    }
}
