// CTCInventoryTrade::SellHighlightedItem() -- sell the highlighted item to the shop: the shop buys it at its current buy price, the hero is paid, the item leaves the inventory; then the trade lists are rebuilt
// Leaf: ?SellHighlightedItem@CTCInventoryTrade@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005A3436, 331 bytes; size TU with a frame pointer.
// Callees: NInventory::CTCInventoryBase::GetSelectedCategory 0x005BCACE, NInventory::CCategory::GetItemInSlotWithIndex 0x005BC9B0 (the inline
//   GetHighlightedItemIndex reads +0x24), GetBuildingThing 0x005BC4C4, std::bitset<274>::test 0x00410DE0 / the interface map LowerBound
//   0x0040F020 (the inline GetTC), CTCShop::GetCurrentBuyPrice 0x007742C0 / BuyItem 0x00774D20, CPlayer::GetPControlledCreature 0x00487DC0,
//   CTCHeroStats::AddMoney 0x0057B327, the virtual RemoveItemFromInventory (slot 79), ResetTradeLists 0x0059D9AD, InitialiseBuyableItems
//   0x005A30A1, InitialiseSellableItems 0x0059D820, GetHighlightedItem 0x005BCAE3, ResetInventoryAnim 0x005BCD2D,
//   SetNumberOfSelectedItemOwned 0x005BE284.
// Name fix (agentNF91, q11199): the self name was already Ego_r.pdb + FableWin.pdb ?SellHighlightedItem@CTCInventoryTrade@@QAEXXZ; the row moved
//   off its row-local CTCInventoryTrade / CTCInventoryBase / CCategory / CTCShop / CPlayer / vector models onto the shared headers (the
//   0x005A3581 BuyHighlightedItem recipe), so it binds 0x005A30A1 InitialiseBuyableItems with the PDB vector spelling (was AAUCPairVector) and
//   the NInventory::CTCInventoryBase members (GetSelectedCategory, GetBuildingThing (was GetOwner), GetHighlightedItem, ResetInventoryAnim,
//   SetNumberOfSelectedItemOwned) with their PDB spellings.
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
#define FABLE_RETAIL_CTCINVENTORYTRADE_PROCESSTRADEACTION
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
#define FABLE_RETAIL_CTCSHOP_BUYITEM
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

void CTCInventoryTrade::SellHighlightedItem()
{
    CategoryIdent = (EInventoryCategoryIdentifier)0xc;
    NInventory::CCategory* category = GetSelectedCategory();
    if (category) {
        long item = category->GetItemInSlotWithIndex(category->GetHighlightedItemIndex());
        if (item > 0) {
            CTCShop* shop = 0;
            GetBuildingThing()->GetTC(TCI_SHOP, &shop);
            long price = shop->GetCurrentBuyPrice(item, 1);
            shop->BuyItem(item);
            CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCHeroStats* stats = 0;
            hero->GetTC(TCI_HERO_STATS, &stats);
            stats->AddMoney(price, true);
            RemoveItemFromInventory(item, 1);
        }
    }
    ResetTradeLists();
    std::vector<std::pair<long, long> > buyable;
    InitialiseBuyableItems(buyable);
    InitialiseSellableItems();
    ResetInventoryAnim(GetHighlightedItem());
    SetNumberOfSelectedItemOwned(GetHighlightedItem());
}
