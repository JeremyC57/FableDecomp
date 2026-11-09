// CTCInventoryTrade::ConstructActualSellInventoryPC() -- builds the PC sell trade screen: collects the hero's sellable items (none for a barber, tattooist or title vendor), drops hair and tattoo cards from them and from the shop's buyable list, records the buyable list in ItemsToBuy keyed by its index, creates the goods list under the SELL menu entry with its frame, description viewport and title, adds one named sub list per menu branch holding a sell entry per item, appends the per-branch and per-category item counts to their labels, adds descriptions and subtitles to every menu entry, shows the hero's gold and, for an ordinary shop, refreshes the entry amounts
// Grade: DIFFER(3664v3668) -- the second card-filter loop (buyableItems) only: retail 0x005A4E49 `mov esi,[ebp+4] ; cmp esi,[ebp+8] ; je 0x005A4F19` (head test duplicated, its empty-vector exit threaded past the ItemsToBuy insert loop); ours `mov esi,[ebp+4] ; jmp <bottom test>` (cross-jumped into the shared test). Probe: c2prio puts the begin() temporary at lines 107/114 (both loop heads) in one split candidate -> esi, so the allocator agrees; the difference is the head-test duplication / threading, which no plain spelling of the two loops moved (30 compile-only probes + grades s5..s18).
// Next: the only spelling that reproduces retail's control flow is `if (!buyableItems.empty()) { filter loop; insert loop }` (probe/s7.cpp, s13, s15): residue then one `mov esi,eax` (c2prio: the begin() temporary #9 prio 14, key 130, colours eax before `it` #37 prio 10; `it` would need +5 = one more reference in the K=5 body blocks, or the head block's K to drop to 2). Lane gap: loop-head duplication / jump threading.
// Leaf: ?ConstructActualSellInventoryPC@CTCInventoryTrade@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A4CDD, 3668 bytes, plain `ret` at 0x005A5B30 (0x005A5B31 Wanted follows with no padding).
// Same TU and shape as ConstructActualWantedInventoryPC 0x005A5B31 (matched); differences: two item vectors, the sell sub lists are
//   named "Category_Child_Sell_List" (g_CCharString_013b8b94) before they are added, AddSellItemToListPC 0x0059F51E, the ItemsToBuy
//   map insert 0x005B2872 keyed by the running index, vector<pair<long,long> >::erase 0x00593139, GetAllItems 0x005BF482,
//   InitialiseSellableItems 0x0059D820 / InitialiseBuyableItems 0x005A30A1, the tree clears 0x0042ADE5 / 0x005B1C18 / 0x005B1C41 /
//   0x005B21A1. Shape: the branch key is copied to a named local (`pBranch`, retail keeps it at [ebp+6Ch] for operator[]); the insert
//   value is std::make_pair (key/value stored after the pair is built, `mov edx,edi ; inc edi`).
#pragma optimize("s", on)
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
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
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
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCInventoryItem.h"
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

// CPlayer::PeekPControlledCreature returns a CThingPlayerCreature; only its CThing base (PeekTC) is used here. As in the landed
// sibling 0x005A8C66, engine/CThingPlayerCreature.h is not usable in this TU (it needs the whole retail creature tree:
// FABLE_RETAIL_CTHINGCREATUREBASE fails on CHitParameters / the base size asserts), so the type is completed over CThing.
class CThingPlayerCreature : public CThing { };

CWideString FABLE_FASTCALL GFIntToWideString(long value);                 // 0x00405F00

extern CCharString g_CCharString_013b8b70;                                // "Weapons_Subtitle"
extern CCharString g_CCharString_013b8b74;                                // "Clothes_Subtitle"
extern CCharString g_CCharString_013b8b78;                                // "Items_Subtitle"
extern CCharString g_CCharString_013b8b94;                                // "Category_Child_Sell_List"

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

void CTCInventoryTrade::ConstructActualSellInventoryPC()
{
    ItemSellLists.clear();
    ResetTradeLists();
    std::vector<std::pair<long, long> > allItems;
    std::vector<std::pair<long, long> > buyableItems;
    CTCShop* pShop = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &pShop);
    if (!pShop->IsBarberShop() && !pShop->IsTattooShop() && !pShop->IsTitleShop()) {
        InitialiseSellableItems();
        GetAllItems(allItems);
    }
    for (std::vector<std::pair<long, long> >::iterator it = allItems.begin(); it != allItems.end(); ) {
        if (!CTCInventoryItem::GetTattooCardDef(it->first).IsNull() || !CTCInventoryItem::GetHairCardDef(it->first).IsNull())
            it = allItems.erase(it);
        else
            ++it;
    }
    ItemsToBuy.clear();
    InitialiseBuyableItems(buyableItems);
    for (std::vector<std::pair<long, long> >::iterator it = buyableItems.begin(); it != buyableItems.end(); ) {
        if (!CTCInventoryItem::GetTattooCardDef(it->first).IsNull() || !CTCInventoryItem::GetHairCardDef(it->first).IsNull())
            it = buyableItems.erase(it);
        else
            ++it;
    }
    long index = 0;
    for (std::vector<std::pair<long, long> >::iterator it = buyableItems.begin(); it != buyableItems.end(); ++it)
        ItemsToBuy.insert(std::make_pair(reinterpret_cast<CComponent*>(index++), *it));
    ItemsToSell.clear();
    SellListsByCategory.clear();
    const CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    CManager* pUIManager = CManager::GetInstance();
    CComponent* pTopLevelList = PTop_Level_List;
    pTopLevelList->FindChild("PC_MENU_ENTRY_TRADE_SELL");
    PCurrentOpenTradeMenu = pUIManager->CreateComponent("PC_TRADING_GOODS_LIST", false);
    PCurrentOpenTradeMenu->Initialise();
    AttachToFrame(pTopLevelList->FindChild("PC_MENU_ENTRY_TRADE_SELL"));
    CComponent* pFrame = PCurrentOpenTradeMenu->FindChild("PC_UI_INACTIVE_FRAME");
    CViewport* pViewport = static_cast<CViewport*>(PCurrentOpenTradeMenu->FindChild("PC_DESCRIPTION_VIEWPORT"));
    pFrame->AddPositionChild(pViewport);
    CList* pGoodsList = static_cast<CList*>(PCurrentOpenTradeMenu);
    CCountedPointer<CComponent> pDescription;
    CCountedPointer<CComponent> pTitle(pUIManager->CreateComponent("PC_TRADE_SELL_TITLE", false));
    pGoodsList->AddNonScrollingChildDynamically(pTitle);
    pFrame->AddPositionChild(pTitle.Get());

    TotNumClothesToSell = TotNumWeaponsToSell = TotNumItemsToSell = 0;
    if (allItems.begin() != allItems.end()) {
        std::map<CComponent*, long> branchCounts;
        for (std::vector<std::pair<long, long> >::iterator it = allItems.begin(); it != allItems.end(); ++it) {
            EItem_Menu_Type type;
            CComponent* branch = GetMenuBranchFromDefIndexPC(it->first, PCurrentOpenTradeMenu, type);
            std::map<CComponent*, long>::iterator found = branchCounts.find(branch);
            if (found == branchCounts.end())
                branchCounts[branch] = 1;
            else
                ++found->second;
        }
        for (std::map<CComponent*, long>::iterator b = branchCounts.begin(); b != branchCounts.end(); ++b) {
            CCountedPointer<CComponent> pSubList(pUIManager->CreateComponent("PC_SUB_LIST", false));
            CList* pList = static_cast<CList*>(pSubList.Get());
            pList->SetName(g_CCharString_013b8b94);
            CComponent* pBranch = b->first;
            pBranch->AddChildDynamically(pSubList);
            pList->InitialiseOffsets(b->second);
            SellListsByCategory[pBranch] = pList;
        }
        NumSellableThings.clear();
        for (std::vector<std::pair<long, long> >::iterator it = allItems.begin(); it != allItems.end(); ++it) {
            EItem_Menu_Type type;
            CComponent* branch = GetMenuBranchFromDefIndexPC(it->first, PCurrentOpenTradeMenu, type);
            if (AddSellItemToListPC(pFrame, it->first, it->second, SellListsByCategory.find(branch)->second, pViewport, true)) {
                if (NumSellableThings.find(branch) == NumSellableThings.end())
                    NumSellableThings[branch].second = it->second;
                else
                    NumSellableThings[branch].second += it->second;
                switch (type) {
                case ITEM_MENU_TYPE_CLOTHES: TotNumClothesToSell += it->second; break;
                case ITEM_MENU_TYPE_WEAPONS: TotNumWeaponsToSell += it->second; break;
                case ITEM_MENU_TYPE_ITEMS:   TotNumItemsToSell += it->second;   break;
                }
            }
        }
        std::map<CComponent*, std::pair<CWideString, long> >::iterator end = NumSellableThings.end();
        for (std::map<CComponent*, std::pair<CWideString, long> >::iterator s = NumSellableThings.begin(); s != end; ++s) {
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
            text += GFIntToWideString(TotNumClothesToSell);
            pText->SetValue(text, true, true);
        }
        {
            CText* pText = static_cast<CText*>(PCurrentOpenTradeMenu->FindChild("PC_TEXT_WEAPONS"));
            CWideString text(pText->GetValue());
            text += CTCInventoryStats::GetXString();
            text += GFIntToWideString(TotNumWeaponsToSell);
            pText->SetValue(text, true, true);
        }
        {
            CText* pText = static_cast<CText*>(PCurrentOpenTradeMenu->FindChild("PC_TEXT_ITEMS"));
            CWideString text(pText->GetValue());
            text += CTCInventoryStats::GetXString();
            text += GFIntToWideString(TotNumItemsToSell);
            pText->SetValue(text, true, true);
        }
    }

    CWideString xString = CTCInventoryStats::GetXString();
    CWideString sellText = textBank.GetTextBySymbol("TEXT_GUI_MENU_SELL") + xString;
    sellText += GFIntToWideString(TotNumItemsToSell + TotNumClothesToSell + TotNumWeaponsToSell);
    CCountedPointer<CComponent> pSubtitle;
    CComponent* pMenu = PCurrentOpenTradeMenu;
    std::vector<CCountedPointer<CComponent> >& entries = pMenu->GetChildren();
    for (long i = 0; i < (long)entries.size(); ++i) {
        pDescription.Reset(pUIManager->CreateComponent("PC_TRADE_SELL_DESC", false));
        entries[i]->AddChildDynamically(pDescription);
        pFrame->AddPositionChild(pDescription.Get());
        pViewport->AddFakeChild(pDescription.Get());
        pSubtitle.Reset(pUIManager->CreateComponent("PC_SUBTITLE", false));
        CText* pSubtitleText = static_cast<CText*>(pSubtitle.Get());
        CWideString subtitleText;
        CCharString entryName = entries[i]->GetName();
        if (entryName == "PC_TRADING_MENU_CLOTHING_ENTRY") {
            subtitleText = textBank.GetTextBySymbol("TEXT_GUI_MENU_CLOTHING") + xString;
            subtitleText += GFIntToWideString(TotNumClothesToSell);
            pSubtitle->SetName(g_CCharString_013b8b74);
        } else if (entryName == "PC_TRADING_MENU_WEAPONS_ENTRY") {
            subtitleText = textBank.GetTextBySymbol("TEXT_GUI_MENU_WEAPONS") + xString;
            subtitleText += GFIntToWideString(TotNumWeaponsToSell);
            pSubtitle->SetName(g_CCharString_013b8b70);
        } else if (entryName == "PC_TRADING_MENU_ITEMS_ENTRY") {
            subtitleText = textBank.GetTextBySymbol("TEXT_GUI_MENU_ITEMS") + xString;
            subtitleText += GFIntToWideString(TotNumItemsToSell);
            pSubtitle->SetName(g_CCharString_013b8b78);
        }
        pSubtitleText->SetValue(subtitleText, true, true);
        entries[i]->AddChildDynamically(pSubtitle);
        pFrame->AddPositionChild(pSubtitle.Get());
    }
    {
        std::vector<CCountedPointer<CComponent> >& items = pMenu->FindChild("PC_TRADING_CLOTHING_LIST")->GetChildren();
        for (long i = 0; i < (long)items.size(); ++i) {
            pDescription.Reset(pUIManager->CreateComponent("PC_TRADE_SELL_DESC", false));
            items[i]->AddChildDynamically(pDescription);
            pFrame->AddPositionChild(pDescription.Get());
            pViewport->AddFakeChild(pDescription.Get());
        }
    }
    {
        std::vector<CCountedPointer<CComponent> >& items = pMenu->FindChild("PC_TRADING_WEAPONS_LIST")->GetChildren();
        for (long i = 0; i < (long)items.size(); ++i) {
            pDescription.Reset(pUIManager->CreateComponent("PC_TRADE_SELL_DESC", false));
            items[i]->AddChildDynamically(pDescription);
            pFrame->AddPositionChild(pDescription.Get());
            pViewport->AddFakeChild(pDescription.Get());
        }
    }
    {
        std::vector<CCountedPointer<CComponent> >& items = pMenu->FindChild("PC_TRADING_ITEMS_LIST")->GetChildren();
        for (long i = 0; i < (long)items.size(); ++i) {
            pDescription.Reset(pUIManager->CreateComponent("PC_TRADE_SELL_DESC", false));
            items[i]->AddChildDynamically(pDescription);
            pFrame->AddPositionChild(pDescription.Get());
            pViewport->AddFakeChild(pDescription.Get());
        }
    }
    ConstructSubtitlesPC(pMenu, NumSellableThings, pFrame);

    CText* pGoldText = static_cast<CText*>(pGoodsList->FindChild("PC_GOLD_AMOUNT_TRADE_SCREEN"));
    if (pGoldText) {
        const CTCHeroStats* pHeroStats = 0;
        CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature()->PeekTC(TCI_HERO_STATS, &pHeroStats);
        pGoldText->SetValue(GFIntToWideString(pHeroStats->GetMoney()), true, true);
    }
    PCurrentOpenTradeMenu->ChangeState(5);
    Components.push_back(PCurrentOpenTradeMenu);
    if (!pShop->IsTitleShop() && !pShop->IsTattooShop() && !pShop->IsBarberShop())
        RefreshEntriesAmountsAndAvailabilitiesPC(PCurrentOpenTradeMenu, SellListsByCategory);
}
