// CTCInventoryTrade::ConstructActualBuyInventoryPC() -- builds the PC buy trade screen: gets the shop's buyable items and drops hair and tattoo cards from them, creates the goods list under the BUY menu entry with its frame, description viewport and title, adds one sub list per menu branch (sized by the buyable and sold-out items in it) holding a buy entry per item, appends the per-branch and per-category stock counts to their labels, adds the sold-out entries, descriptions and subtitles, shows the hero's gold and, for an ordinary shop, refreshes the entry amounts
// Grade: DIFFER(3532v3532) -- one reload-order hunk at the subtitle join 0x005ACB58: retail `mov esi,[ebp+74h] ; mov edi,[ebp+4Ch]` (entries then pFrame), ours edi first. The loop-head and empty() hunks are closed by the escape facts (as Sell 0x005A4CDD, matched): same-TU InitialiseBuyableItems body + PUSHBACK_NOINLINE + new ERASE_NOINLINE.
// Next: c2prio --trace: pFrame #30 (prio 197, piece edi #63 from split pass 2) is coloured before entries #51 (177, esi #125); the twin Sell orders the same join pFrame-first in retail. Tried: k=1..7 unused locals (top / before xString / in the loop), 1..4 dead FABLE_RETAIL_ASSERT(pFrame), forceinline RemoveCards, pBranch local, decl orders, shared `type` local: no move or worse.
// Leaf: ?ConstructActualBuyInventoryPC@CTCInventoryTrade@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005AC10E, 3532 bytes, plain `ret` at 0x005ACED9.
// Same TU and shape as ConstructActualWantedInventoryPC 0x005A5B31 (matched); differences: the buyable list from
//   InitialiseBuyableItems 0x005A30A1 filtered with vector<pair<long,long> >::erase 0x00593139 and CTCInventoryItem::GetTattooCardDef
//   0x005D9AA0 / GetHairCardDef 0x005D9A10, the NotInStock (sold-out) list counted and added with a zero count, AddBuyItemToListPC
//   0x005A6CA1, the per-branch stock total in NumBuyableThings, the tree clears 0x0042ADE5 / 0x005B1C18 / 0x005B1C41 / 0x005B21A1.
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
    std::vector<std::pair<long, long> > buyableItems;
    CTCShop* pShop = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &pShop);
    InitialiseBuyableItems(buyableItems);
    for (std::vector<std::pair<long, long> >::iterator it = buyableItems.begin(); it != buyableItems.end(); ) {
        if (!CTCInventoryItem::GetTattooCardDef(it->first).IsNull() || !CTCInventoryItem::GetHairCardDef(it->first).IsNull())
            it = buyableItems.erase(it);
        else
            ++it;
    }
    ItemsToBuy.clear();
    BuyListsByCategory.clear();
    const CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    CManager* pUIManager = CManager::GetInstance();
    CComponent* pTopLevelList = PTop_Level_List;
    pTopLevelList->FindChild("PC_MENU_ENTRY_TRADE_BUY");
    PCurrentOpenTradeMenu = pUIManager->CreateComponent("PC_TRADING_GOODS_LIST", false);
    PCurrentOpenTradeMenu->Initialise();
    AttachToFrame(pTopLevelList->FindChild("PC_MENU_ENTRY_TRADE_BUY"));
    CComponent* pFrame = PCurrentOpenTradeMenu->FindChild("PC_UI_INACTIVE_FRAME");
    CViewport* pViewport = static_cast<CViewport*>(PCurrentOpenTradeMenu->FindChild("PC_DESCRIPTION_VIEWPORT"));
    pFrame->AddPositionChild(pViewport);
    CList* pGoodsList = static_cast<CList*>(PCurrentOpenTradeMenu);
    CCountedPointer<CComponent> pDescription;
    CCountedPointer<CComponent> pTitle(pUIManager->CreateComponent("PC_TRADE_BUY_TITLE", false));
    pGoodsList->AddNonScrollingChildDynamically(pTitle);
    pFrame->AddPositionChild(pTitle.Get());

    TotNumClothesToBuy = TotNumWeaponsToBuy = TotNumItemsToBuy = 0;
    if (!buyableItems.empty() || !NotInStock.empty()) {
        std::map<CComponent*, long> branchCounts;
        for (std::vector<std::pair<long, long> >::iterator it = buyableItems.begin(); it != buyableItems.end(); ++it) {
            EItem_Menu_Type type;
            CComponent* branch = GetMenuBranchFromDefIndexPC(it->first, PCurrentOpenTradeMenu, type);
            std::map<CComponent*, long>::iterator found = branchCounts.find(branch);
            if (found == branchCounts.end())
                branchCounts[branch] = 1;
            else
                ++found->second;
        }
        for (std::list<long>::iterator n = NotInStock.begin(); n != NotInStock.end(); ++n) {
            EItem_Menu_Type type;
            CComponent* branch = GetMenuBranchFromDefIndexPC(*n, PCurrentOpenTradeMenu, type);
            std::map<CComponent*, long>::iterator found = branchCounts.find(branch);
            if (found == branchCounts.end())
                branchCounts[branch] = 1;
            else
                ++found->second;
        }
        for (std::map<CComponent*, long>::iterator b = branchCounts.begin(); b != branchCounts.end(); ++b) {
            CCountedPointer<CComponent> pSubList(pUIManager->CreateComponent("PC_SUB_LIST", false));
            CList* pList = static_cast<CList*>(pSubList.Get());
            b->first->AddChildDynamically(pSubList);
            pList->InitialiseOffsets(b->second);
            BuyListsByCategory[b->first] = pList;
        }
        NumBuyableThings.clear();
        for (std::vector<std::pair<long, long> >::iterator it = buyableItems.begin(); it != buyableItems.end(); ++it) {
            EItem_Menu_Type type;
            CComponent* branch = GetMenuBranchFromDefIndexPC(it->first, PCurrentOpenTradeMenu, type);
            if (AddBuyItemToListPC(pFrame, it->first, it->second, BuyListsByCategory.find(branch)->second, pViewport, true)) {
                if (NumBuyableThings.find(branch) == NumBuyableThings.end())
                    NumBuyableThings[branch].second = it->second;
                else
                    NumBuyableThings[branch].second += it->second;
                switch (type) {
                case ITEM_MENU_TYPE_CLOTHES: TotNumClothesToBuy += it->second; break;
                case ITEM_MENU_TYPE_WEAPONS: TotNumWeaponsToBuy += it->second; break;
                case ITEM_MENU_TYPE_ITEMS:   TotNumItemsToBuy += it->second;   break;
                }
            }
        }
        std::map<CComponent*, std::pair<CWideString, long> >::iterator end = NumBuyableThings.end();
        for (std::map<CComponent*, std::pair<CWideString, long> >::iterator s = NumBuyableThings.begin(); s != end; ++s) {
            CText* pText = static_cast<CText*>(s->first->FindFirstChildOfType(UI_TYPE_TEXT));
            CWideString text(pText->GetValue());
            s->second.first = text;
            if (s->second.second > 0) {
                text += CTCInventoryStats::GetXString();
                text += GFIntToWideString(s->second.second);
            }
            pText->SetValue(text, true, true);
        }
        {
            CText* pText = static_cast<CText*>(PCurrentOpenTradeMenu->FindChild("PC_TEXT_CLOTHING"));
            CWideString text(pText->GetValue());
            text += CTCInventoryStats::GetXString();
            text += GFIntToWideString(TotNumClothesToBuy);
            pText->SetValue(text, true, true);
        }
        {
            CText* pText = static_cast<CText*>(PCurrentOpenTradeMenu->FindChild("PC_TEXT_WEAPONS"));
            CWideString text(pText->GetValue());
            text += CTCInventoryStats::GetXString();
            text += GFIntToWideString(TotNumWeaponsToBuy);
            pText->SetValue(text, true, true);
        }
        {
            CText* pText = static_cast<CText*>(PCurrentOpenTradeMenu->FindChild("PC_TEXT_ITEMS"));
            CWideString text(pText->GetValue());
            text += CTCInventoryStats::GetXString();
            text += GFIntToWideString(TotNumItemsToBuy);
            pText->SetValue(text, true, true);
        }
        for (std::list<long>::iterator n = NotInStock.begin(); n != NotInStock.end(); ++n) {
            EItem_Menu_Type type;
            CComponent* branch = GetMenuBranchFromDefIndexPC(*n, PCurrentOpenTradeMenu, type);
            AddBuyItemToListPC(pFrame, *n, 0, BuyListsByCategory.find(branch)->second, pViewport, true);
        }
    }

    CWideString xString = CTCInventoryStats::GetXString();
    CWideString buyText = textBank.GetTextBySymbol("TEXT_GUI_MENU_BUY") + xString;
    buyText += GFIntToWideString(TotNumItemsToBuy + TotNumClothesToBuy + TotNumWeaponsToBuy);
    CCountedPointer<CComponent> pSubtitle;
    CComponent* pMenu = PCurrentOpenTradeMenu;
    std::vector<CCountedPointer<CComponent> >& entries = pMenu->GetChildren();
    for (long i = 0; i < (long)entries.size(); ++i) {
        pDescription.Reset(pUIManager->CreateComponent("PC_TRADE_BUY_DESC", false));
        entries[i]->AddChildDynamically(pDescription);
        pFrame->AddPositionChild(pDescription.Get());
        pViewport->AddFakeChild(pDescription.Get());
        pSubtitle.Reset(pUIManager->CreateComponent("PC_SUBTITLE", false));
        CText* pSubtitleText = static_cast<CText*>(pSubtitle.Get());
        CWideString subtitleText;
        CCharString entryName = entries[i]->GetName();
        if (entryName == "PC_TRADING_MENU_CLOTHING_ENTRY") {
            subtitleText = textBank.GetTextBySymbol("TEXT_GUI_MENU_CLOTHING") + xString;
            subtitleText += GFIntToWideString(TotNumClothesToBuy);
            pSubtitle->SetName(CLOTHES_SUBTITLE);
        } else if (entryName == "PC_TRADING_MENU_WEAPONS_ENTRY") {
            subtitleText = textBank.GetTextBySymbol("TEXT_GUI_MENU_WEAPONS") + xString;
            subtitleText += GFIntToWideString(TotNumWeaponsToBuy);
            pSubtitle->SetName(WEAPONS_SUBTITLE);
        } else if (entryName == "PC_TRADING_MENU_ITEMS_ENTRY") {
            subtitleText = textBank.GetTextBySymbol("TEXT_GUI_MENU_ITEMS") + xString;
            subtitleText += GFIntToWideString(TotNumItemsToBuy);
            pSubtitle->SetName(ITEMS_SUBTITLE);
        }
        pSubtitleText->SetValue(subtitleText, true, true);
        entries[i]->AddChildDynamically(pSubtitle);
        pFrame->AddPositionChild(pSubtitle.Get());
    }
    {
        std::vector<CCountedPointer<CComponent> >& items = pMenu->FindChild("PC_TRADING_CLOTHING_LIST")->GetChildren();
        for (long i = 0; i < (long)items.size(); ++i) {
            pDescription.Reset(pUIManager->CreateComponent("PC_TRADE_BUY_DESC", false));
            items[i]->AddChildDynamically(pDescription);
            pFrame->AddPositionChild(pDescription.Get());
            pViewport->AddFakeChild(pDescription.Get());
        }
    }
    {
        std::vector<CCountedPointer<CComponent> >& items = pMenu->FindChild("PC_TRADING_WEAPONS_LIST")->GetChildren();
        for (long i = 0; i < (long)items.size(); ++i) {
            pDescription.Reset(pUIManager->CreateComponent("PC_TRADE_BUY_DESC", false));
            items[i]->AddChildDynamically(pDescription);
            pFrame->AddPositionChild(pDescription.Get());
            pViewport->AddFakeChild(pDescription.Get());
        }
    }
    {
        std::vector<CCountedPointer<CComponent> >& items = pMenu->FindChild("PC_TRADING_ITEMS_LIST")->GetChildren();
        for (long i = 0; i < (long)items.size(); ++i) {
            pDescription.Reset(pUIManager->CreateComponent("PC_TRADE_BUY_DESC", false));
            items[i]->AddChildDynamically(pDescription);
            pFrame->AddPositionChild(pDescription.Get());
            pViewport->AddFakeChild(pDescription.Get());
        }
    }
    ConstructSubtitlesPC(pMenu, NumBuyableThings, pFrame);

    CText* pGoldText = static_cast<CText*>(pGoodsList->FindChild("PC_GOLD_AMOUNT_TRADE_SCREEN"));
    if (pGoldText) {
        const CTCHeroStats* pHeroStats = 0;
        CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature()->PeekTC(TCI_HERO_STATS, &pHeroStats);
        pGoldText->SetValue(GFIntToWideString(pHeroStats->GetMoney()), true, true);
    }
    PCurrentOpenTradeMenu->ChangeState(5);
    Components.push_back(PCurrentOpenTradeMenu);
    if (!pShop->IsTitleShop() && !pShop->IsTattooShop() && !pShop->IsBarberShop())
        RefreshEntriesAmountsAndAvailabilitiesPC(PCurrentOpenTradeMenu, BuyListsByCategory);
}
