// CTCInventoryTrade::ConstructActualBuyInventoryPC builds buy menus with goods, stock totals, subtitles and the hero’s cash.
// Grade: DIFFER(3532v3532) -- 2 instructions: reload order at 0x005ACB58 (retail mov esi,[ebp+74h] then mov edi,[ebp+4Ch]; ours reversed)
// Grade: DIFFER(3532v3532). Retail reloads the category children before the frame in the subtitle join. The rebuilt instructions are reversed.
// Leaf: ?ConstructActualBuyInventoryPC@CTCInventoryTrade@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005AC10E, 3532 bytes, plain `ret` at 0x005ACED9.

#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_PCINVENTORY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CTCSHOP_SHOPTYPE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CTCHEROSTATS_GETMONEY
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTATTOODEF
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS
#define FABLE_RETAIL_STLP_VECTOR_ERASE_NOINLINE
#define FABLE_RETAIL_CTCSHOP_GETSTOCKLIST
#define FABLE_RETAIL_CTCSHOP_STOCKQUERIES
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CHEROTITLEDEF
#define FABLE_RETAIL_CTCINVENTORYTRADE_ADDITEMOFTYPE
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CRESOURCELIST
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CThingBaseDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CHeroTitleDef.h"
#include "engine/CTattooDef.h"
#include "engine/CHairCardDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_enums.h"
#include "engine/CCountedPointer.h"
using namespace NUISystem;
using NGameText::CDataBank;


CWideString FABLE_FASTCALL GFIntToWideString(long value);                 // 0x00405F00

namespace { extern CCharString WEAPONS_SUBTITLE; }                                // "Weapons_Subtitle"
namespace { extern CCharString CLOTHES_SUBTITLE; }                                // "Clothes_Subtitle"
namespace { extern CCharString ITEMS_SUBTITLE; }                                // "Items_Subtitle"

#pragma auto_inline(off)
template <class T> long __fastcall CBasicString<T>::Compare(const T* a, const T* b)
{
    for (;;) {
        T ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        T cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
template <class T> bool CBasicString<T>::operator==(const T* text) const
{
    return Compare(PData, text) == 0;
}
#pragma auto_inline(on)

// Same-TU body of InitialiseBuyableItems (CTCInventoryTrade.cpp defines it above this row; retail 0x005A30A1, called out of line at
// the row's call site): the landed row's logic, written over the shared headers (landed local-model names -> Ego_r members:
// BuyableItems = SpecialItemsToDelete +0x1A4, TotalBuyable = ItemBuyCount +0x168, Mode = CategoryIdent +0x170, GetShopThing =
// GetBuildingThing). With it visible (and vector::push_back / erase visible but out of line) VC7.1 knows no callee keeps the
// address of the row's local buyable vector. Only the row is graded.
void CTCInventoryTrade::InitialiseBuyableItems(std::vector<std::pair<long, long> >& items)
{
    SpecialItemsToDelete.clear();
    long total = 0;
    CategoryIdent = (EInventoryCategoryIdentifier)0xb;
    CTCShop* pShop = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &pShop);
    std::map<long, long> stock = pShop->GetStockList();
    const NInventory::CTCInventoryBase* pInventory = 0;
    CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature()->PeekTC(TCI_INVENTORY, &pInventory);
    std::vector<std::pair<long, long> > heroItems;
    pInventory->GetAllItems(heroItems);
    for (unsigned int i = 0; i < heroItems.size(); ++i) {
        long defIndex = heroItems[i].first;
        long key = defIndex;
        CDefPointer<const CThingBaseDef> def;
        GetDefinitionManager().GetDef(defIndex, &def);
        CDefPointer<const CHeroTitleDef> heroTitle;
        bool special = !CTCInventoryItem::GetTattooCardDef(defIndex).IsNull() && pShop->IsTattooShop();
        if (!special)
            special = !CTCInventoryItem::GetHairCardDef(defIndex).IsNull() && pShop->IsBarberShop();
        if (special)
            stock[key] = 1;
        else if (def->GetSubDef(&heroTitle) && pShop->IsTitleShop())
            stock[key] = 1;
        else
            goto next;
        {
            std::map<long, long>::iterator it = SpecialItemsToDelete.find(key);
            if (it == SpecialItemsToDelete.end())
                SpecialItemsToDelete[key] = heroItems[i].second;
            else
                (*it).second += heroItems[i].second;
        }
    next:
        ;
    }
    NotInStock.clear();
    for (std::map<long, long>::iterator it = stock.begin(); it != stock.end(); ++it) {
        long count = (*it).second;
        long defIndex = (*it).first;
        long key = defIndex;
        if (count == 0) {
            bool plain = CTCInventoryItem::GetTattooCardDef(defIndex).IsNull() && CTCInventoryItem::GetHairCardDef(defIndex).IsNull();
            if (plain && pShop->GetMaxStock(defIndex))
                NotInStock.push_back(key);
        } else if (CTCInventoryItem::IsBuyable(defIndex)) {
            AddItemOfTypeToInventory(defIndex, count);
            total += count;
            items.push_back(std::pair<long, long>(defIndex, count));
        }
    }
    ItemBuyCount = total;
}

void CTCInventoryTrade::ConstructActualBuyInventoryPC()
{
    ItemBuyLists.clear();
    ResetTradeLists();
    std::vector<std::pair<long, long> > buyable;
    CTCShop* ptc_shop = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &ptc_shop);
    InitialiseBuyableItems(buyable);
    for (std::vector<std::pair<long, long> >::iterator it = buyable.begin(); it != buyable.end(); ) {
        if (!CTCInventoryItem::GetTattooCardDef(it->first).IsNull() || !CTCInventoryItem::GetHairCardDef(it->first).IsNull())
            it = buyable.erase(it);
        else
            ++it;
    }
    ItemsToBuy.clear();
    BuyListsByCategory.clear();
    const CDataBank& rTextBank = CMainGameComponent::Get()->GetTextBank();
    CManager* pmanager = CManager::GetInstance();
    CComponent* plist_component = PTop_Level_List;
    plist_component->FindChild("PC_MENU_ENTRY_TRADE_BUY");
    PCurrentOpenTradeMenu = pmanager->CreateComponent("PC_TRADING_GOODS_LIST", false);
    PCurrentOpenTradeMenu->Initialise();
    AttachToFrame(plist_component->FindChild("PC_MENU_ENTRY_TRADE_BUY"));
    CComponent* pframe = PCurrentOpenTradeMenu->FindChild("PC_UI_INACTIVE_FRAME");
    CComponent* pviewport_component = PCurrentOpenTradeMenu->FindChild("PC_DESCRIPTION_VIEWPORT");
    CViewport* pviewport = static_cast<CViewport*>(pviewport_component);
    pframe->AddPositionChild(pviewport);
    CList* plist_of_goods = static_cast<CList*>(PCurrentOpenTradeMenu);
    CCountedPointer<CComponent> pdesc;
    CCountedPointer<CComponent> pbuytitle(pmanager->CreateComponent("PC_TRADE_BUY_TITLE", false));
    plist_of_goods->AddNonScrollingChildDynamically(pbuytitle);
    pframe->AddPositionChild(pbuytitle.Get());

    TotNumClothesToBuy = TotNumWeaponsToBuy = TotNumItemsToBuy = 0;
    if (!buyable.empty() || !NotInStock.empty()) {
        std::map<CComponent*, long> same_categories;
        for (std::vector<std::pair<long, long> >::iterator it = buyable.begin(); it != buyable.end(); ++it) {
            EItem_Menu_Type type;
            CComponent* pentry = GetMenuBranchFromDefIndexPC(it->first, PCurrentOpenTradeMenu, type);
            std::map<CComponent*, long>::iterator found = same_categories.find(pentry);
            if (found == same_categories.end())
                same_categories[pentry] = 1;
            else
                ++found->second;
        }
        for (std::list<long>::iterator it = NotInStock.begin(); it != NotInStock.end(); ++it) {
            EItem_Menu_Type type;
            CComponent* pentry = GetMenuBranchFromDefIndexPC(*it, PCurrentOpenTradeMenu, type);
            std::map<CComponent*, long>::iterator found = same_categories.find(pentry);
            if (found == same_categories.end())
                same_categories[pentry] = 1;
            else
                ++found->second;
        }
        for (std::map<CComponent*, long>::iterator it = same_categories.begin(); it != same_categories.end(); ++it) {
            CCountedPointer<CComponent> pchild(pmanager->CreateComponent("PC_SUB_LIST", false));
            CList* pbuyable_list = static_cast<CList*>(pchild.Get());
            it->first->AddChildDynamically(pchild);
            pbuyable_list->InitialiseOffsets(it->second);
            BuyListsByCategory[it->first] = pbuyable_list;
        }
        NumBuyableThings.clear();
        for (std::vector<std::pair<long, long> >::iterator it = buyable.begin(); it != buyable.end(); ++it) {
            EItem_Menu_Type type;
            CComponent* pentry = GetMenuBranchFromDefIndexPC(it->first, PCurrentOpenTradeMenu, type);
            if (AddBuyItemToListPC(pframe, it->first, it->second, BuyListsByCategory.find(pentry)->second, pviewport, true)) {
                if (NumBuyableThings.find(pentry) == NumBuyableThings.end())
                    NumBuyableThings[pentry].second = it->second;
                else
                    NumBuyableThings[pentry].second += it->second;
                switch (type) {
                case ITEM_MENU_TYPE_CLOTHES: TotNumClothesToBuy += it->second; break;
                case ITEM_MENU_TYPE_WEAPONS: TotNumWeaponsToBuy += it->second; break;
                case ITEM_MENU_TYPE_ITEMS:   TotNumItemsToBuy += it->second;   break;
                }
            }
        }
        std::map<CComponent*, std::pair<CWideString, long> >::iterator endit = NumBuyableThings.end();
        for (std::map<CComponent*, std::pair<CWideString, long> >::iterator it = NumBuyableThings.begin(); it != endit; ++it) {
            CComponent* component = it->first->FindFirstChildOfType(UI_TYPE_TEXT);
            CText* p_text = static_cast<CText*>(component);
            CWideString v(p_text->GetValue());
            it->second.first = v;
            if (it->second.second > 0) {
                v += CTCInventoryStats::GetXString();
                v += GFIntToWideString(it->second.second);
            }
            p_text->SetValue(v, true, true);
        }
        {
            CText* p_text = static_cast<CText*>(PCurrentOpenTradeMenu->FindChild("PC_TEXT_CLOTHING"));
            CWideString v(p_text->GetValue());
            v += CTCInventoryStats::GetXString();
            v += GFIntToWideString(TotNumClothesToBuy);
            p_text->SetValue(v, true, true);
        }
        {
            CText* p_text = static_cast<CText*>(PCurrentOpenTradeMenu->FindChild("PC_TEXT_WEAPONS"));
            CWideString v(p_text->GetValue());
            v += CTCInventoryStats::GetXString();
            v += GFIntToWideString(TotNumWeaponsToBuy);
            p_text->SetValue(v, true, true);
        }
        {
            CText* p_text = static_cast<CText*>(PCurrentOpenTradeMenu->FindChild("PC_TEXT_ITEMS"));
            CWideString v(p_text->GetValue());
            v += CTCInventoryStats::GetXString();
            v += GFIntToWideString(TotNumItemsToBuy);
            p_text->SetValue(v, true, true);
        }
        for (std::list<long>::iterator it = NotInStock.begin(); it != NotInStock.end(); ++it) {
            EItem_Menu_Type type;
            CComponent* pentry = GetMenuBranchFromDefIndexPC(*it, PCurrentOpenTradeMenu, type);
            AddBuyItemToListPC(pframe, *it, 0, BuyListsByCategory.find(pentry)->second, pviewport, true);
        }
    }

    CWideString XString = CTCInventoryStats::GetXString();
    CWideString new_title = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_BUY") + XString;
    new_title += GFIntToWideString(TotNumItemsToBuy + TotNumClothesToBuy + TotNumWeaponsToBuy);
    CCountedPointer<CComponent> pbuy_subtitle;
    CText* pbuy_subtitle_real;
    CComponent* ui_trading_goods_list = PCurrentOpenTradeMenu;
    std::vector<CCountedPointer<CComponent> >& ui_trading_goods_list_children_buy = ui_trading_goods_list->GetChildren();
    for (long i = 0; i < (long)ui_trading_goods_list_children_buy.size(); ++i) {
        pdesc.Reset(pmanager->CreateComponent("PC_TRADE_BUY_DESC", false));
        ui_trading_goods_list_children_buy[i]->AddChildDynamically(pdesc);
        pframe->AddPositionChild(pdesc.Get());
        pviewport->AddFakeChild(pdesc.Get());
        pbuy_subtitle.Reset(pmanager->CreateComponent("PC_SUBTITLE", false));
        pbuy_subtitle_real = static_cast<CText*>(pbuy_subtitle.Get());
        CWideString v;
        CCharString n = ui_trading_goods_list_children_buy[i]->GetName();
        if (n == "PC_TRADING_MENU_CLOTHING_ENTRY") {
            v = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_CLOTHING") + XString;
            v += GFIntToWideString(TotNumClothesToBuy);
            pbuy_subtitle->SetName(CLOTHES_SUBTITLE);
        } else if (n == "PC_TRADING_MENU_WEAPONS_ENTRY") {
            v = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_WEAPONS") + XString;
            v += GFIntToWideString(TotNumWeaponsToBuy);
            pbuy_subtitle->SetName(WEAPONS_SUBTITLE);
        } else if (n == "PC_TRADING_MENU_ITEMS_ENTRY") {
            v = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_ITEMS") + XString;
            v += GFIntToWideString(TotNumItemsToBuy);
            pbuy_subtitle->SetName(ITEMS_SUBTITLE);
        }
        pbuy_subtitle_real->SetValue(v, true, true);
        ui_trading_goods_list_children_buy[i]->AddChildDynamically(pbuy_subtitle);
        pframe->AddPositionChild(pbuy_subtitle.Get());
    }
    {
        CComponent* ui_clothes_goods_list = ui_trading_goods_list->FindChild("PC_TRADING_CLOTHING_LIST");
        std::vector<CCountedPointer<CComponent> >& ui_clothes_goods_list_children_buy = ui_clothes_goods_list->GetChildren();
        for (long i = 0; i < (long)ui_clothes_goods_list_children_buy.size(); ++i) {
            pdesc.Reset(pmanager->CreateComponent("PC_TRADE_BUY_DESC", false));
            ui_clothes_goods_list_children_buy[i]->AddChildDynamically(pdesc);
            pframe->AddPositionChild(pdesc.Get());
            pviewport->AddFakeChild(pdesc.Get());
        }
    }
    {
        CComponent* ui_weapons_goods_list = ui_trading_goods_list->FindChild("PC_TRADING_WEAPONS_LIST");
        std::vector<CCountedPointer<CComponent> >& ui_weapons_goods_list_children_buy = ui_weapons_goods_list->GetChildren();
        for (long i = 0; i < (long)ui_weapons_goods_list_children_buy.size(); ++i) {
            pdesc.Reset(pmanager->CreateComponent("PC_TRADE_BUY_DESC", false));
            ui_weapons_goods_list_children_buy[i]->AddChildDynamically(pdesc);
            pframe->AddPositionChild(pdesc.Get());
            pviewport->AddFakeChild(pdesc.Get());
        }
    }
    {
        CComponent* ui_items_goods_list = ui_trading_goods_list->FindChild("PC_TRADING_ITEMS_LIST");
        std::vector<CCountedPointer<CComponent> >& ui_items_goods_list_children_buy = ui_items_goods_list->GetChildren();
        for (long i = 0; i < (long)ui_items_goods_list_children_buy.size(); ++i) {
            pdesc.Reset(pmanager->CreateComponent("PC_TRADE_BUY_DESC", false));
            ui_items_goods_list_children_buy[i]->AddChildDynamically(pdesc);
            pframe->AddPositionChild(pdesc.Get());
            pviewport->AddFakeChild(pdesc.Get());
        }
    }
    ConstructSubtitlesPC(ui_trading_goods_list, NumBuyableThings, pframe);

    CText* pgold_real = static_cast<CText*>(plist_of_goods->FindChild("PC_GOLD_AMOUNT_TRADE_SCREEN"));
    if (pgold_real) {
        const CTCHeroStats* pTCHeroStats = 0;
        CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature()->PeekTC(TCI_HERO_STATS, &pTCHeroStats);
        long cash = pTCHeroStats->GetMoney();
        pgold_real->SetValue(GFIntToWideString(cash), true, true);
    }
    PCurrentOpenTradeMenu->ChangeState(5);
    Components.push_back(PCurrentOpenTradeMenu);
    if (!ptc_shop->IsTitleShop() && !ptc_shop->IsTattooShop() && !ptc_shop->IsBarberShop())
        RefreshEntriesAmountsAndAvailabilitiesPC(PCurrentOpenTradeMenu, BuyListsByCategory);
}
