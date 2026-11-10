// CTCInventoryTrade::ConstructActualWantedInventoryPC() -- builds the PC "wanted" trade screen: asks the shop for the items it wants, creates the goods list under the WANTED menu entry with its frame, description viewport and title, adds one sub list per menu branch (sized by the wanted items in it) holding an entry per wanted item, appends the per-branch and per-category (clothing / weapons / items) counts to their labels, adds descriptions and subtitles to every menu entry, shows the hero's gold and, for an ordinary shop, refreshes the entry amounts
// Leaf: ?ConstructActualWantedInventoryPC@CTCInventoryTrade@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A5B31, 3128 bytes, plain `ret` at 0x005A6768. Size TU: optimize("s",on) only (frame
//   `push ebp ; lea ebp,[esp-78h]`; optimize("y",off) as in the sibling 0x005A8C66 gives `mov ebp,esp`).
// Callees: map<CComponent*,CList*>::clear 0x005B1C41 (WantedListsByCategory), ResetTradeLists 0x0059D9AD, GetBuildingThing
//   0x005BC4C4 + inline GetTC (bitset::test 0x00410DE0, CVectorMap::LowerBound 0x0040F020), CTCShop::GetWantedItemsList 0x00774100
//   (vector<long> by value), CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D, AttachToFrame 0x0059BB1B,
//   GetMenuBranchFromDefIndexPC 0x0059C0C6, AddWantedItemToListPC 0x0059D270, ConstructSubtitlesPC 0x0059E4D7,
//   RefreshEntriesAmountsAndAvailabilitiesPC 0x0059FAFE, CTCInventoryStats::GetXString 0x005CC772, GFIntToWideString 0x00405F00,
//   CDataBank::GetTextBySymbol 0x009C95E0, CBasicString<char>::operator==(const char*) 0x004115A0, the map<CComponent*,long>
//   ctor 0x004DF81D / dtor 0x004DD1C4, CCountedPointer<CComponent> ctor 0x00429C15 / Reset 0x00429C6F / dtor 0x004291DE.
// Shape: `entryName == "literal"` expands CCharString::operator== in place (FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ: the
//   PStringData null test + `repe cmpsb` against the empty literal, else the out-of-line 0x004115A0); the same-TU bodies of
//   CBasicString<char>::Compare (0x00411570) and operator==(const char*) under auto_inline(off) tell the mod/ref summary they only
//   read, so retail's single PStringData load per test survives (without them every test reloads it). The first FindChild of the
//   WANTED menu entry is retail's own discarded call (0x005A5BD2).
// Name fix (agentNF 2026-11-10): 0x004291DE is CCountedPointer<NUISystem::CComponent>::ReleaseData (the destructor is the 5-byte
//   `jmp` COMDAT 0x00429C53; Ego_r.pdb ??1 is 5 bytes): FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (engine/CCountedPointer.h).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
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
#define FABLE_RETAIL_CTCSHOP_WANTED
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CTCHEROSTATS_GETMONEY
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
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

void CTCInventoryTrade::ConstructActualWantedInventoryPC()
{
    WantedListsByCategory.clear();
    ResetTradeLists();
    CTCShop* pShop = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &pShop);
    std::vector<long> wantedItems = pShop->GetWantedItemsList();
    const CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    CManager* pUIManager = CManager::GetInstance();
    CComponent* pTopLevelList = PTop_Level_List;
    pTopLevelList->FindChild("PC_MENU_ENTRY_TRADE_WANTED");
    PCurrentOpenTradeMenu = pUIManager->CreateComponent("PC_TRADING_GOODS_LIST", false);
    PCurrentOpenTradeMenu->Initialise();
    AttachToFrame(pTopLevelList->FindChild("PC_MENU_ENTRY_TRADE_WANTED"));
    CComponent* pFrame = PCurrentOpenTradeMenu->FindChild("PC_UI_INACTIVE_FRAME");
    CViewport* pViewport = static_cast<CViewport*>(PCurrentOpenTradeMenu->FindChild("PC_DESCRIPTION_VIEWPORT"));
    pFrame->AddPositionChild(pViewport);
    CList* pGoodsList = static_cast<CList*>(PCurrentOpenTradeMenu);
    CCountedPointer<CComponent> pDescription;
    CCountedPointer<CComponent> pTitle(pUIManager->CreateComponent("PC_TRADE_WANTED_TITLE", false));
    pGoodsList->AddNonScrollingChildDynamically(pTitle);
    pFrame->AddPositionChild(pTitle.Get());

    TotNumClothesWanted = TotNumWeaponsWanted = TotNumItemsWanted = 0;
    if (wantedItems.begin() != wantedItems.end()) {
        std::map<CComponent*, long> branchCounts;
        for (std::vector<long>::iterator it = wantedItems.begin(); it != wantedItems.end(); ++it) {
            EItem_Menu_Type type;
            CComponent* branch = GetMenuBranchFromDefIndexPC(*it, PCurrentOpenTradeMenu, type);
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
            WantedListsByCategory[b->first] = pList;
        }
        NumWantedThings.clear();
        for (std::vector<long>::iterator it = wantedItems.begin(); it != wantedItems.end(); ++it) {
            EItem_Menu_Type type;
            CComponent* branch = GetMenuBranchFromDefIndexPC(*it, PCurrentOpenTradeMenu, type);
            AddWantedItemToListPC(pFrame, *it, WantedListsByCategory.find(branch)->second, pViewport);
            if (NumWantedThings.find(branch) == NumWantedThings.end())
                NumWantedThings[branch].second = 1;
            else
                ++NumWantedThings[branch].second;
            switch (type) {
            case ITEM_MENU_TYPE_CLOTHES: ++TotNumClothesWanted; break;
            case ITEM_MENU_TYPE_WEAPONS: ++TotNumWeaponsWanted; break;
            case ITEM_MENU_TYPE_ITEMS:   ++TotNumItemsWanted;   break;
            }
        }
        std::map<CComponent*, std::pair<CWideString, long> >::iterator end = NumWantedThings.end();
        for (std::map<CComponent*, std::pair<CWideString, long> >::iterator s = NumWantedThings.begin(); s != end; ++s) {
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
            text += GFIntToWideString(TotNumClothesWanted);
            pText->SetValue(text, true, true);
        }
        {
            CText* pText = static_cast<CText*>(PCurrentOpenTradeMenu->FindChild("PC_TEXT_WEAPONS"));
            CWideString text(pText->GetValue());
            text += CTCInventoryStats::GetXString();
            text += GFIntToWideString(TotNumWeaponsWanted);
            pText->SetValue(text, true, true);
        }
        {
            CText* pText = static_cast<CText*>(PCurrentOpenTradeMenu->FindChild("PC_TEXT_ITEMS"));
            CWideString text(pText->GetValue());
            text += CTCInventoryStats::GetXString();
            text += GFIntToWideString(TotNumItemsWanted);
            pText->SetValue(text, true, true);
        }
    }

    CWideString xString = CTCInventoryStats::GetXString();
    CWideString wantedText = textBank.GetTextBySymbol("TEXT_GUI_MENU_WANTED") + xString;
    wantedText += GFIntToWideString(TotNumItemsWanted + TotNumClothesWanted + TotNumWeaponsWanted);
    CCountedPointer<CComponent> pSubtitle;
    CComponent* pMenu = PCurrentOpenTradeMenu;
    std::vector<CCountedPointer<CComponent> >& entries = pMenu->GetChildren();
    for (long i = 0; i < (long)entries.size(); ++i) {
        pDescription.Reset(pUIManager->CreateComponent("PC_TRADE_WANTED_DESC", false));
        entries[i]->AddChildDynamically(pDescription);
        pFrame->AddPositionChild(pDescription.Get());
        pViewport->AddFakeChild(pDescription.Get());
        pSubtitle.Reset(pUIManager->CreateComponent("PC_SUBTITLE", false));
        CText* pSubtitleText = static_cast<CText*>(pSubtitle.Get());
        CWideString subtitleText;
        CCharString entryName = entries[i]->GetName();
        if (entryName == "PC_TRADING_MENU_CLOTHING_ENTRY") {
            subtitleText = textBank.GetTextBySymbol("TEXT_GUI_MENU_CLOTHING") + xString;
            subtitleText += GFIntToWideString(TotNumClothesWanted);
            pSubtitle->SetName(g_CCharString_013b8b74);
        } else if (entryName == "PC_TRADING_MENU_WEAPONS_ENTRY") {
            subtitleText = textBank.GetTextBySymbol("TEXT_GUI_MENU_WEAPONS") + xString;
            subtitleText += GFIntToWideString(TotNumWeaponsWanted);
            pSubtitle->SetName(g_CCharString_013b8b70);
        } else if (entryName == "PC_TRADING_MENU_ITEMS_ENTRY") {
            subtitleText = textBank.GetTextBySymbol("TEXT_GUI_MENU_ITEMS") + xString;
            subtitleText += GFIntToWideString(TotNumItemsWanted);
            pSubtitle->SetName(g_CCharString_013b8b78);
        }
        pSubtitleText->SetValue(subtitleText, true, true);
        entries[i]->AddChildDynamically(pSubtitle);
        pFrame->AddPositionChild(pSubtitle.Get());
    }
    {
        std::vector<CCountedPointer<CComponent> >& items = pMenu->FindChild("PC_TRADING_CLOTHING_LIST")->GetChildren();
        for (long i = 0; i < (long)items.size(); ++i) {
            pDescription.Reset(pUIManager->CreateComponent("PC_TRADE_WANTED_DESC", false));
            items[i]->AddChildDynamically(pDescription);
            pFrame->AddPositionChild(pDescription.Get());
            pViewport->AddFakeChild(pDescription.Get());
        }
    }
    {
        std::vector<CCountedPointer<CComponent> >& items = pMenu->FindChild("PC_TRADING_WEAPONS_LIST")->GetChildren();
        for (long i = 0; i < (long)items.size(); ++i) {
            pDescription.Reset(pUIManager->CreateComponent("PC_TRADE_WANTED_DESC", false));
            items[i]->AddChildDynamically(pDescription);
            pFrame->AddPositionChild(pDescription.Get());
            pViewport->AddFakeChild(pDescription.Get());
        }
    }
    {
        std::vector<CCountedPointer<CComponent> >& items = pMenu->FindChild("PC_TRADING_ITEMS_LIST")->GetChildren();
        for (long i = 0; i < (long)items.size(); ++i) {
            pDescription.Reset(pUIManager->CreateComponent("PC_TRADE_WANTED_DESC", false));
            items[i]->AddChildDynamically(pDescription);
            pFrame->AddPositionChild(pDescription.Get());
            pViewport->AddFakeChild(pDescription.Get());
        }
    }
    ConstructSubtitlesPC(pMenu, NumWantedThings, pFrame);

    CText* pGoldText = static_cast<CText*>(pGoodsList->FindChild("PC_GOLD_AMOUNT_TRADE_SCREEN"));
    if (pGoldText) {
        const CTCHeroStats* pHeroStats = 0;
        CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature()->PeekTC(TCI_HERO_STATS, &pHeroStats);
        pGoldText->SetValue(GFIntToWideString(pHeroStats->GetMoney()), true, true);
    }
    PCurrentOpenTradeMenu->ChangeState(5);
    Components.push_back(PCurrentOpenTradeMenu);
    if (!pShop->IsTitleShop() && !pShop->IsTattooShop() && !pShop->IsBarberShop())
        RefreshEntriesAmountsAndAvailabilitiesPC(PCurrentOpenTradeMenu, WantedListsByCategory);
}