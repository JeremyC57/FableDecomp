// CTCInventoryTrade::ConstructSpecialShopList(const std::vector<std::pair<long,long> >& items, CCharString& listName) -- console special shop (barber / tattooist / title vendor): clears the trade lists, creates the shop's top-level list and its title (named after the shop when the text bank has the shop's name), fills a sub list under the special-shop buy entry with the items for sale (counting the buyable ones) and the out-of-stock ones, captions the buy entry with the buyable total, adds the shop's menu-guide description, shows the hero's gold and titles the trading screen "Trade"; returns the list
// Leaf: ?ConstructSpecialShopList@CTCInventoryTrade@@QAEPAVCComponent@NUISystem@@ABV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@AAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A29F4, 1709 bytes, thiscall `ret 8`. Frame-pointer size TU: optimize("y",off) + ("s",on).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_SHOPLISTS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCSHOP_SHOPTYPE
#define FABLE_RETAIL_CTCSHOP_NAME
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CTCHEROSTATS_GETMONEY
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CCountedPointer.h"
using namespace NUISystem;

class CThingPlayerCreature : public CThing { };

CWideString FABLE_FASTCALL GFIntToWideString(long value);                 // 0x00405F00

namespace { extern CCharString SPEC_SUBTITLE; }                                // "Spec_Subtitle"

// Same-TU body of the def lookup retail's TU holds (out of line in retail: GetDef<CUIMiscThingsDef> 0x0042B312), as the landed
// CTCInventoryWeapons::ConstructScreen 0x005C8459: VC7.1 sees it only writes through the out pointer.
#pragma auto_inline(off)
template <class T>
bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index > 0) {
        CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
        if (!def.IsNull()) {
            out->SetDynamicCast(def);
            return true;
        }
    }
    return false;
}
#pragma auto_inline(on)

CComponent* CTCInventoryTrade::ConstructSpecialShopList(const std::vector<std::pair<long, long> >& items, CCharString& listName)
{
    ItemsToBuy.clear();
    ItemsToSell.clear();
    SellListsByCategory.clear();
    BuyListsByCategory.clear();
    BuyListsByCategory.clear();
    CManager* manager = CManager::GetInstance();
    CComponent* list = manager->CreateComponent(listName, false);
    CTCShop* shop = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &shop);
    CCountedPointer<CComponent> title;
    if (shop->IsBarberShop())
        title.Reset(manager->CreateComponent("UI_TRADE_BARBER_TITLE", false));
    else if (shop->IsTattooShop())
        title.Reset(manager->CreateComponent("UI_TRADE_TATTOOIST_TITLE", false));
    else
        title.Reset(manager->CreateComponent("UI_TRADE_TITLE_VENDOR_TITLE", false));
    // the title shows the shop's own name when the text bank has it
    if (shop->GetName() > 0) {
        bool haveName = CMainGameComponent::Get()->GetTextBank().EntryExists(shop->GetName())
                     && CMainGameComponent::Get()->GetTextBank().GetTextInfo(shop->GetName()) != 0
                     && !CMainGameComponent::Get()->GetTextBank().GetPTextEntry(shop->GetName(), true).IsNull();
        if (haveName) {
            CText* titleText = static_cast<CText*>(title.Get());
            if (titleText)
                titleText->SetValue(CMainGameComponent::Get()->GetTextBank().GetPTextEntry(shop->GetName(), true)->GetText(), true, true);
        }
    }
    static_cast<CList*>(list)->AddNonScrollingChild(title);
    CViewport* viewport = static_cast<CViewport*>(list->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA"));
    CDefPointer<const CUIMiscThingsDef> miscDef;
    long miscIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(miscIndex, &miscDef);
    CCountedPointer<CComponent> subListComponent(manager->CreateComponent("UI_SUB_LIST", false));
    CList* subList = static_cast<CList*>(subListComponent.Get());
    CComponent* buyEntry = list->FindChild("UI_SPECIALSHOP_MENU_BUY_ENTRY");
    TotSpecialItemsToBuy = 0;
    if (!items.empty() || !NotInStock.empty()) {
        subList->InitialiseOffsets(items.size() + NotInStock.size());
        for (std::vector<std::pair<long, long> >::const_iterator it = items.begin(); it != items.end(); ++it) {
            if (AddBuyItemToList(it->first, it->second, subList, viewport, false))
                TotSpecialItemsToBuy += it->second;
        }
        // the buy entry's caption: its text plus " x <buyable total>", also as a one-off subtitle
        CText* buyText = static_cast<CText*>(buyEntry->FindChild("UI_SPECIALSHOP_BUY_TEXT"));
        SpecialItemString = buyText->GetValue();
        CWideString priceText(SpecialItemString);
        priceText += CTCInventoryStats::GetXString();
        priceText += GFIntToWideString(TotSpecialItemsToBuy);
        buyText->SetValue(priceText, true, true);
        CCountedPointer<CComponent> subtitle(manager->CreateComponent("UI_ONE_OFF_SUBTITLE", false));
        CText* subtitleText = static_cast<CText*>(subtitle.Get());
        subtitleText->SetName(SPEC_SUBTITLE);
        subtitleText->SetValue(priceText, true, true);
        buyEntry->AddChild(subtitle);
        CCharString guideSymbol;
        if (shop->IsBarberShop())
            guideSymbol = "TEXT_GUI_MENUGUIDE_TRADE_BARBERS_BUY";
        else if (shop->IsTattooShop())
            guideSymbol = "TEXT_GUI_MENUGUIDE_TRADE_TATTOOIST_BUY";
        else
            guideSymbol = "TEXT_GUI_MENUGUIDE_TRADE_TITLEVENDOR_BUY";
        CCountedPointer<CComponent> description(manager->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF", false));
        static_cast<CText*>(description.Get())->SetValue(CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(guideSymbol), true, true);
        buyEntry->AddChild(description);
        viewport->AddFakeChild(description.Get());
        for (std::list<long>::iterator n = NotInStock.begin(); n != NotInStock.end(); ++n)
            AddBuyItemToList(*n, 0, subList, viewport, false);
    }
    buyEntry->AddChild(subListComponent);
    // the hero's gold
    CComponent* gold = list->FindChild("UI_GOLD_AMOUNT_TRADE_SCREEN");
    if (gold) {
        const CTCHeroStats* heroStats = 0;
        CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature()->PeekTC(TCI_HERO_STATS, &heroStats);
        static_cast<CText*>(gold)->SetValue(GFIntToWideString(heroStats->GetMoney()), true, true);
    }
    // the trading screen's "Trade" title
    CComponent* screen = list->FindChild("UI_TRADING_MENU_SCREEN");
    if (screen) {
        CCountedPointer<CComponent> screenTitle(CManager::GetInstance()->CreateComponent("UI_TITLE_NEW", false));
        CCountedPointer<CText> screenTitleText;
        screenTitleText.SetStaticCast(screenTitle);
        {
            CCharString trade("Trade");
            screenTitleText->SetValue(trade, true, true);
        }
        screen->AddChildDynamically(screenTitle);
    }
    static_cast<CList*>(list)->SetDefaultChild(0L);
    return list;
}