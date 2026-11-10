// CTCInventoryTrade::BuyItemPC(NUISystem::CComponent* entry, long count) -- the PC trade screen buys `count` of the item behind a buy-list entry: the entry's stock drops (specials never run out), its name / title refresh, the entry goes unavailable when the hero can hold no more; the hero pays, then the item takes effect by kind -- a tattoo card is applied, a hair card applied, a title def sets the hero's title, anything else is sold `count` times by the shop into the hero's category inventory (a weapon the hero cannot use raises the warning) and, if it can be sold back, adds to the sell totals and the sell entry's "Sell x N" text. Specials make the other affordable, not-yet-owned specials available again; the buy descriptions, buy actions, gold readouts and (for a normal shop) the buy entry amounts refresh, and the UI steps back.
// Leaf: ?BuyItemPC@CTCInventoryTrade@@QAEXPAVCComponent@NUISystem@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B0AAC, 1518 bytes (0x5EE; the catalog's 1,793 also covers the 15 small COMDAT stubs from
//   0x005B109A on: SPLITS.tsv), thiscall `ret 8`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Identity: FableWin.pdb ?BuyItemPC@CTCInventoryTrade@@QAEXPAVCComponent@NUISystem@@J@Z (the catalog's doubled
//   "CTCInventoryTrade::CTCInventoryTrade::BuyItemPC" scope is a label typo): the PC twin of the landed BuyItem 0x005AF64E -- the same
//   purchase body with the PC callees (RefreshNameAndTitlePC 0x0059CA95, RefreshBuyPossibilityPC 0x005A887D, GetTypeFromDefIndexPC
//   0x0059C447, RefreshBuyDescriptionsPC 0x005AE9DE, RefreshEntriesAmountsAndAvailabilitiesPC 0x0059FAFE) and PC strings
//   ("PC_MENU_ENTRY_TRADE_SELL", "PC_TRADING_SELL_TEXT", "PC_LIST_BUY_ACTIONS", "PC_UI_INACTIVE_FRAME", "PC_GOLD_AMOUNT_TRADE_SCREEN");
//   its only caller is 0x0041F5E5.
// Shape: the landed BuyItem 0x005AF64E source (same TU, same opt-ins and same-TU GetDef / GetSubDef<CHeroTitleDef> bodies); the gold
//   readouts are every PC_GOLD_AMOUNT_TRADE_SCREEN child of the open trade menu (FindAllChildren into a local list<CComponent*>).
// Callees: as 0x005AF64E, plus the PC ones named above, CComponent::FindAllChildren (slot 5, +0x14), list<CComponent*> ctor
//   0x0042ABAF / dtor 0x0042ABCA.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_BUYITEMPC
#define FABLE_RETAIL_CTCINVENTORYTRADE_BUYPOSSIBILITYPC
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_CTCSHOP_SELLPRICE
#define FABLE_RETAIL_CTCSHOP_SHOPTYPE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CTCHEROSTATS_GETMONEY
#define FABLE_RETAIL_CTCHEROSTATS_ADDMONEY
#define FABLE_RETAIL_CTCHERO_TITLE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER   // non-const GetMainPlayer 0x00449970, as retail calls
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CHEROTITLEDEF
#define FABLE_RETAIL_CHAIRCARDDEF
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHero.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CThingBaseDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CHeroTitleDef.h"
#include "engine/CTattooDef.h"
#include "engine/CHairCardDef.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_enums.h"
#include "engine/CCountedPointer.h"
using namespace NUISystem;
using NGameText::CDataBank;

// The controlled creature is only GetTC'd here (as the landed NPlayerGui rows model it; engine/CThingPlayerCreature.h needs the
// whole creature tree).
class CThingPlayerCreature : public CThing { };

CWideString FABLE_FASTCALL GFIntToWideString(long value);                 // 0x00405F00

CWideString FABLE_FASTCALL GFIntToWideString(long value);                 // 0x00405F00

#pragma auto_inline(off)
template <class T> bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
    if (def.IsNull())
        return false;
    out->SetDynamicCast(def);
    return true;
}
template <> bool CDefClassBase::GetSubDef<CHeroTitleDef>(CDefPointer<const CHeroTitleDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CCharString("CHeroTitleDef"));
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#pragma auto_inline(on)

void CTCInventoryTrade::BuyItemPC(CComponent* entry, long count)
{
    const CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    CWideString xString = CTCInventoryStats::GetXString();
    std::pair<long, long>& buyRecord = ItemsToBuy[entry];
    if (!IsSpecialCase(buyRecord.first))
        buyRecord.second -= count;                                   // a special item never runs out
    RefreshNameAndTitlePC(entry, buyRecord.first, buyRecord.second);

    // the entry goes unavailable when the shop has none left, or the hero can hold no more of a normal item
    CThingPlayerCreature* hero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    NInventory::CTCInventoryBase* inventory = 0;
    ETCInterfaceType inventoryType = CTCInventoryItem::GetInventoryType(buyRecord.first);
    hero->GetTC(inventoryType, &inventory);
    if (buyRecord.second == 0 || (!inventory->IsSpaceInInventoryForItem(buyRecord.first) && !IsSpecialCase(buyRecord.first)))
        entry->MakeUnavailableDynamically();

    // the purchase: the hero pays for a card, a title or a normal item, and the item takes effect by kind
    CDefPointer<const CTattooDef> tattooCardDef = CTCInventoryItem::GetTattooCardDef(buyRecord.first);
    CDefPointer<const CHairCardDef> hairCardDef = CTCInventoryItem::GetHairCardDef(buyRecord.first);
    long itemDefIndex = buyRecord.first;
    CTCShop* shop = 0;
    CTCHeroStats* heroStats = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &shop);
    long price = shop->GetCurrentSellPrice(itemDefIndex, count);
    hero->GetTC(TCI_HERO_STATS, &heroStats);
    CDefPointer<const CThingBaseDef> itemDef;
    GetDefinitionManager().GetDef(itemDefIndex, &itemDef);
    CDefPointer<const CHeroTitleDef> titleDef;
    if (!tattooCardDef.IsNull() || !hairCardDef.IsNull() || itemDef->GetSubDef(&titleDef))
        heroStats->AddMoney(-price, true);
    if (!tattooCardDef.IsNull()) {
        ApplyTattooCard(itemDefIndex);
        RefreshBuyPossibilityPC();
    } else if (!hairCardDef.IsNull()) {
        CWideString description = CTCInventoryItem::GetDescription(itemDefIndex);
        ApplyHairCard(*hairCardDef);
        RefreshBuyPossibilityPC();
    } else if (itemDef->GetSubDef(&titleDef)) {
        CTCHero* heroComponent;
        if (ParentThing->GetTC(TCI_HERO, &heroComponent)) {
            heroComponent->SetTitle(itemDefIndex);
            RefreshBuyPossibilityPC();
        }
    } else {
        // a normal item: the shop sells `count`, the hero pays, and the items go into the hero's category inventory
        for (long i = 0; i < count; ++i)
            shop->SellItem(itemDefIndex);
        heroStats->AddMoney(-price, true);
        if (CTCInventoryItem::IsWeapon(itemDefIndex)) {
            CTCHero* heroComponent;
            if (hero->GetTC(TCI_HERO, &heroComponent)) {
                if (!heroComponent->WeaponsUsable)
                    WarningNeeded = true;                            // the hero cannot wield weapons yet
            }
        }
        inventory->AddItemToInventory(itemDefIndex, count, false, true, price / count, false);
        RefreshBuyPossibilityPC();
        if (CTCInventoryItem::IsBuyable(itemDefIndex)) {
            // the bought item joins the sell side: the per-type total and the "Sell x N" text of the sell entry
            CComponent* sellEntry = Components[0]->FindChild("PC_MENU_ENTRY_TRADE_SELL");
            EItem_Menu_Type type = GetTypeFromDefIndexPC(itemDefIndex);
            CWideString totalText;
            switch (type) {
            case ITEM_MENU_TYPE_CLOTHES:
                TotNumClothesToSell += count;
                break;
            case ITEM_MENU_TYPE_WEAPONS:
                TotNumWeaponsToSell += count;
                break;
            case ITEM_MENU_TYPE_ITEMS:
                TotNumItemsToSell += count;
                break;
            }
            totalText = textBank.GetTextBySymbol("TEXT_GUI_MENU_SELL") + xString;
            totalText += GFIntToWideString(TotNumItemsToSell + TotNumWeaponsToSell + TotNumClothesToSell);
            static_cast<CText*>(sellEntry->FindChild("PC_TRADING_SELL_TEXT"))->SetValue(totalText, true, true);
        }
    }

    // a special item: the other affordable, not-yet-owned specials become available again
    if (IsSpecialCase(itemDefIndex)) {
        for (std::map<CComponent*, std::pair<long, long> >::iterator it = ItemsToBuy.begin(); it != ItemsToBuy.end(); ++it) {
            if (it->second.second > 0
                && it->first->FindChild("PC_LIST_BUY_ACTIONS") == 0
                && shop->GetCurrentSellPrice(it->second.first, 1) <= heroStats->GetMoney()
                && !AlreadyHaveSpecial(it->second.first))
                it->first->MakeAvailableDynamically();
        }
    }

    RefreshBuyDescriptionsPC(PCurrentOpenTradeMenu->FindChild("PC_UI_INACTIVE_FRAME"));
    UpdateBuyActionsList();
    std::list<CComponent*> goldTexts;
    PCurrentOpenTradeMenu->FindAllChildren("PC_GOLD_AMOUNT_TRADE_SCREEN", goldTexts);
    std::list<CComponent*>::iterator end = goldTexts.end();
    for (std::list<CComponent*>::iterator it = goldTexts.begin(); it != end; ++it) {
        CComponent* goldText = *it;
        if (goldText)
            static_cast<CText*>(goldText)->SetValue(GFIntToWideString(heroStats->GetMoney()), true, true);
    }
    if (!shop->IsTitleShop() && !shop->IsTattooShop() && !shop->IsBarberShop())
        RefreshEntriesAmountsAndAvailabilitiesPC(PCurrentOpenTradeMenu, BuyListsByCategory);
    CManager::GetInstance()->SetEvent(UI_EVENT_BACK);
}