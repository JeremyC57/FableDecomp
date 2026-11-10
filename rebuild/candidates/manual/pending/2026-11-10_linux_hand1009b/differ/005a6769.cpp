// CTCInventoryTrade::ConstructNormalShopListPC(const std::vector<std::pair<long,long> >& buyItems, const std::vector<std::pair<long,long> >& sellItems) -- PC trade screen: creates the top-level trade list and captions its sell / buy / wanted entries with the localised word plus the number of things in each (the sellable stock the shop deals in, the buyable items the hero can afford and carry, the shop's wanted list), keeping the per-category clothes / weapons / items totals; returns the list
// Grade: DIFFER(1336v1336) -- shared headers, same length; only the wanted-total sum's operand order: retail adds the three category totals as [this+1F0h],[this+1F4h],[this+1ECh], ours [1F4h],[1ECh],[1F0h] (VC7.1 ranks the commutative operands by context: deleting the buy section or the buy loop's PeekTC gives the retail order; 9 source spellings of the sum and reset order do not)
// Leaf: ?ConstructNormalShopListPC@CTCInventoryTrade@@QAEPAVCComponent@NUISystem@@ABV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A6769, 1336 bytes, thiscall `ret 8`. Frame-pointer size TU: optimize("y",off) + ("s",on).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_ACTIONSLISTS
#define FABLE_RETAIL_CTCINVENTORYTRADE_SHOPLISTS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCSHOP_VALIDSTOCK
#define FABLE_RETAIL_CTCSHOP_WANTED
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CText.h"
using namespace NUISystem;
using NGameText::CDataBank;

class CThingPlayerCreature : public CThing { };

CWideString FABLE_FASTCALL GFIntToWideString(long value);                 // 0x00405F00

CComponent* CTCInventoryTrade::ConstructNormalShopListPC(const std::vector<std::pair<long, long> >& buyItems,
                                                         const std::vector<std::pair<long, long> >& sellItems)
{
    const CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    CManager* manager = CManager::GetInstance();
    CComponent* list = manager->CreateComponent("PC_TOP_LEVEL_TRADE_LIST", false);
    CComponent* entry = list->FindChild("PC_MENU_ENTRY_TRADE_SELL");
    // what the hero can sell: the stock the shop deals in
    TotNumClothesToSell = TotNumWeaponsToSell = TotNumItemsToSell = 0;
    if (!sellItems.empty()) {
        CTCShop* shop = 0;
        GetBuildingThing()->GetTC(TCI_SHOP, &shop);
        for (std::vector<std::pair<long, long> >::const_iterator it = sellItems.begin(); it != sellItems.end(); ++it) {
            if (shop->IsValidStockItem(it->first)) {
                switch (GetTypeFromDefIndexPC(it->first)) {
                case ITEM_MENU_TYPE_CLOTHES: TotNumClothesToSell += it->second; break;
                case ITEM_MENU_TYPE_WEAPONS: TotNumWeaponsToSell += it->second; break;
                case ITEM_MENU_TYPE_ITEMS:   TotNumItemsToSell += it->second; break;
                }
            }
        }
    }
    CWideString xString = CTCInventoryStats::GetXString();
    CWideString caption = textBank.GetTextBySymbol("TEXT_GUI_MENU_SELL") + xString;
    caption += GFIntToWideString(TotNumWeaponsToSell + TotNumItemsToSell + TotNumClothesToSell);
    static_cast<CText*>(entry->FindChild("PC_TRADING_SELL_TEXT"))->SetValue(caption, true, true);
    // what the hero can buy: affordable, room in the hero's inventory (or a special not yet owned)
    entry = list->FindChild("PC_MENU_ENTRY_TRADE_BUY");
    TotNumClothesToBuy = TotNumWeaponsToBuy = TotNumItemsToBuy = 0;
    if (!buyItems.empty() || !NotInStock.empty()) {
        GetBuildingThing()->HasTC(TCI_SHOP);
        for (std::vector<std::pair<long, long> >::const_iterator it = buyItems.begin(); it != buyItems.end(); ++it) {
            long itemDefIndex = it->first;
            long count = it->second;
            const CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
            const NInventory::CTCInventoryBase* inventory = 0;
            ETCInterfaceType inventoryType = CTCInventoryItem::GetInventoryType(itemDefIndex);
            hero->PeekTC(inventoryType, &inventory);
            if (IsItemAffordable(itemDefIndex, 1) && (inventory->IsSpaceInInventoryForItem(itemDefIndex) || IsSpecialCase(itemDefIndex)) &&
                count > 0 && !AlreadyHaveSpecial(itemDefIndex)) {
                switch (GetTypeFromDefIndexPC(itemDefIndex)) {
                case ITEM_MENU_TYPE_CLOTHES: TotNumClothesToBuy += it->second; break;
                case ITEM_MENU_TYPE_WEAPONS: TotNumWeaponsToBuy += it->second; break;
                case ITEM_MENU_TYPE_ITEMS:   TotNumItemsToBuy += it->second; break;
                }
            }
        }
    }
    caption = textBank.GetTextBySymbol("TEXT_GUI_MENU_BUY") + xString;
    caption += GFIntToWideString(TotNumWeaponsToBuy + TotNumItemsToBuy + TotNumClothesToBuy);
    static_cast<CText*>(entry->FindChild("PC_TRADING_BUY_TEXT"))->SetValue(caption, true, true);
    // what the shop wants
    CTCShop* shop = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &shop);
    std::vector<long> wanted = shop->GetWantedItemsList();
    CComponent* wantedEntry = list->FindChild("PC_MENU_ENTRY_TRADE_WANTED");
    TotNumClothesWanted = TotNumWeaponsWanted = TotNumItemsWanted = 0;
    if (!wanted.empty()) {
        for (std::vector<long>::const_iterator w = wanted.begin(); w != wanted.end(); ++w) {
            switch (GetTypeFromDefIndexPC(*w)) {
            case ITEM_MENU_TYPE_CLOTHES: ++TotNumClothesWanted; break;
            case ITEM_MENU_TYPE_WEAPONS: ++TotNumWeaponsWanted; break;
            case ITEM_MENU_TYPE_ITEMS:   ++TotNumItemsWanted; break;
            }
        }
    }
    caption = textBank.GetTextBySymbol("TEXT_GUI_MENU_WANTED") + CTCInventoryStats::GetXString();
    caption += GFIntToWideString(TotNumWeaponsWanted + TotNumItemsWanted + TotNumClothesWanted);
    static_cast<CText*>(wantedEntry->FindChild("PC_TRADING_WANTED_TEXT"))->SetValue(caption, true, true);
    return list;
}
