// CTCInventoryTrade::ConstructNormalShopListPC -- build the PC trade list and its sell, buy, and wanted captions.
// Flags: none
// Leaf: ?ConstructNormalShopListPC@CTCInventoryTrade@@QAEPAVCComponent@NUISystem@@ABV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A6769, 1336 bytes, thiscall `ret 8`. Frame-pointer size TU: optimize("y",off) + ("s",on).
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
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
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#define FABLE_RETAIL_CTCSHOP_INTERFACETYPE
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
#include "engine/NUISystem_CList.h"
using namespace NUISystem;
using NGameText::CDataBank;

#include "engine/CThingPlayerCreature.h"

CWideString FABLE_FASTCALL GFIntToWideString(long value);                 // 0x00405F00

// The external original map copies are speed optimized with an omitted frame.
template<> CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::iterator CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType& key)
{
    iterator first=this->begin();
    long count=this->end()-first;
    while(count>0) {
        long half=count/2;
        iterator middle=first+half;
        if(middle->first<key) { first=middle+1; count-=half+1; }
        else count=half;
    }
    return first;
}
template<> CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::const_iterator CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType& key) const
{
    const_iterator first=this->begin();
    long count=this->end()-first;
    while(count>0) {
        long half=count/2;
        const_iterator middle=first+half;
        if(middle->first<key) { first=middle+1; count-=half+1; }
        else count=half;
    }
    return first;
}

#pragma optimize("y", off)
#pragma optimize("s", on)
CComponent* CTCInventoryTrade::ConstructNormalShopListPC(const std::vector<std::pair<long, long> >& buyable,
                                                         const std::vector<std::pair<long, long> >& sellable)
{
    const CDataBank& rTextBank = CMainGameComponent::Get()->GetTextBank();
    CManager* pmanager = CManager::GetInstance();
    CComponent* plist_component = pmanager->CreateComponent("PC_TOP_LEVEL_TRADE_LIST", false);
    CList* plist = GFDebugPDynamicCast(plist_component, (CList*)0);
    CComponent* pparent = plist_component->FindChild("PC_MENU_ENTRY_TRADE_SELL");
    // what the pplayer can sell: the stock the shop deals in
    TotNumClothesToSell = TotNumWeaponsToSell = TotNumItemsToSell = 0;
    if (!sellable.empty()) {
        CTCShop* shop = 0;
        GetBuildingThing()->GetTC(&shop);
        for (std::vector<std::pair<long, long> >::const_iterator it = sellable.begin(); it != sellable.end(); ++it) {
            if (shop->IsValidStockItem(it->first)) {
                switch (GetTypeFromDefIndexPC(it->first)) {
                case ITEM_MENU_TYPE_CLOTHES: TotNumClothesToSell += it->second; break;
                case ITEM_MENU_TYPE_WEAPONS: TotNumWeaponsToSell += it->second; break;
                case ITEM_MENU_TYPE_ITEMS:   TotNumItemsToSell += it->second; break;
                }
            }
        }
    }
    CWideString XString = CTCInventoryStats::GetXString();
    CWideString new_title = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_SELL") + XString;
    new_title += GFIntToWideString(TotNumWeaponsToSell + TotNumItemsToSell + TotNumClothesToSell);
    static_cast<CText*>(pparent->FindChild("PC_TRADING_SELL_TEXT"))->SetValue(new_title, true, true);
    // what the pplayer can buy: affordable, room in the pplayer's ptc_inventory (or a special not yet owned)
    pparent = plist_component->FindChild("PC_MENU_ENTRY_TRADE_BUY");
    TotNumClothesToBuy = TotNumWeaponsToBuy = TotNumItemsToBuy = 0;
    if (!buyable.empty() || !NotInStock.empty()) {
        CTCShop* pTCShop = 0;
        CThing* pbuilding_thing = GetBuildingThing();
        pbuilding_thing->GetTC(&pTCShop);
        for (std::vector<std::pair<long, long> >::const_iterator it = buyable.begin(); it != buyable.end(); ++it) {
            long item_def = it->first;
            long quantity = it->second;
            const CThingPlayerCreature* pplayer = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
            const NInventory::CTCInventoryBase* ptc_inventory = 0;
            ETCInterfaceType inventory_type = CTCInventoryItem::GetInventoryType(item_def);
            pplayer->PeekTC(inventory_type, &ptc_inventory);
            if (IsItemAffordable(item_def, 1) && (ptc_inventory->IsSpaceInInventoryForItem(item_def) || IsSpecialCase(item_def)) &&
                quantity > 0 && !AlreadyHaveSpecial(item_def)) {
                switch (GetTypeFromDefIndexPC(item_def)) {
                case ITEM_MENU_TYPE_CLOTHES: TotNumClothesToBuy += it->second; break;
                case ITEM_MENU_TYPE_WEAPONS: TotNumWeaponsToBuy += it->second; break;
                case ITEM_MENU_TYPE_ITEMS:   TotNumItemsToBuy += it->second; break;
                }
            }
        }
    }
    new_title = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_BUY") + XString;
    new_title += GFIntToWideString(TotNumWeaponsToBuy + TotNumItemsToBuy + TotNumClothesToBuy);
    static_cast<CText*>(pparent->FindChild("PC_TRADING_BUY_TEXT"))->SetValue(new_title, true, true);
    // Keep the wanted category totals tied to the inventory screen counters.
    long& clothes_wanted = TotNumClothesWanted;
    long& items_wanted = TotNumItemsWanted;
    // what the shop wants
    CTCShop* shop = 0;
    GetBuildingThing()->GetTC(&shop);
    std::vector<long> wanted_list = shop->GetWantedItemsList();
    pparent = plist_component->FindChild("PC_MENU_ENTRY_TRADE_WANTED");
    clothes_wanted = TotNumWeaponsWanted = items_wanted = 0;
    if (!wanted_list.empty()) {
        for (std::vector<long>::iterator it = wanted_list.begin(); it != wanted_list.end(); ++it) {
            switch (GetTypeFromDefIndexPC(*it)) {
            case ITEM_MENU_TYPE_CLOTHES: ++clothes_wanted; break;
            case ITEM_MENU_TYPE_WEAPONS: ++TotNumWeaponsWanted; break;
            case ITEM_MENU_TYPE_ITEMS:   ++items_wanted; break;
            }
        }
    }
    new_title = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_WANTED") + CTCInventoryStats::GetXString();
    new_title += GFIntToWideString(TotNumWeaponsWanted + items_wanted + clothes_wanted);
    static_cast<CText*>(pparent->FindChild("PC_TRADING_WANTED_TEXT"))->SetValue(new_title, true, true);
    return plist_component;
}