// CTCInventoryTrade::ConstructActualSpecialInventoryPC() -- builds the PC special-shop screen (barber / tattooist / title vendor): resets the trade lists, collects the hero's items the shop deals in (hair cards for a barber, valid tattoo cards for a tattooist), then creates the frame, position dummy, title, description viewport, parent list, description text, gold amount and coin sprite, fills the list with one buy entry per item (plus the sold-out specials), makes the parent list the open trade menu and registers its scroll / back / close clickables with the UI manager
// Leaf: ?ConstructActualSpecialInventoryPC@CTCInventoryTrade@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A8C66, 2135 bytes, plain `ret`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: map<CComponent*,CComponent*>::clear 0x0042ADE5 (ItemBuyLists), ResetTradeLists 0x0059D9AD, GetBuildingThing 0x005BC4C4,
//   bitset::test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020 / const 0x004365B0 (inline GetTC / PeekTC), InitialiseBuyableItems
//   0x005A30A1, CTCInventoryBase::GetAllItems 0x005BF482, CTCShop::IsBarberShop 0x00772490 / IsTattooShop 0x00772510 / IsTitleShop
//   0x00772590 / GetName 0x00772610, CTCInventoryItem::GetHairCardDef 0x005D9A10 / GetTattooCardDef 0x005D9AA0 (static `__fastcall`,
//   CDefPointer by hidden pointer), CTattooDef::IsInvalidCustomTattoo 0x00581765, vector<pair<long,long> >::erase 0x00593139,
//   map clear 0x005B1C18 (ItemsToBuy) / 0x005B1C41 (BuyListsByCategory), CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D,
//   CCountedPointer<CComponent>(T*) 0x00429C15 / Reset 0x00429C6F / ~ 0x004291DE, CCharString(const char*, long) 0x0099EBF0 / () 0x0099E4B0 /
//   = 0x0099EFE0 / ~ 0x0099EAE0, CBankFile::EntryExists 0x009CC190, CDataBank::GetTextInfo 0x009C7D00 / GetPTextEntry 0x009C9070 /
//   GetTextBySymbol 0x009C95E0 (text bank: CMainGameComponent::Get() +0x14), CTextEntry::GetText 0x009C7A40, CWideString copy 0x0099B720 /
//   += 0x0099B8D0 / ~ 0x0099B510, CTCInventoryStats::GetXString 0x005CC772, GFIntToWideString 0x00405F00, AddBuyItemToListPC 0x005A6CA1,
//   CPlayerManager::GetMainPlayer 0x00449960 (+0x1C) / CPlayer::PeekPControlledCreature 0x00487DD0, Components push_back 0x00486024, free 0x00BFEA14.
//   Virtual: CComponent::FindChild 3 / Initialise 43 / ChangeState(ulong) 48 / AddPositionChild 124, CText::SetValue 144 (+0x240),
//   CList::AddNonScrollingChild 145 (+0x244) / InitialiseOffsets 156 (+0x270), CObservable::AddObserver 2, CObserver::ObserveEvent 3.
// Shape: the hero's filtered item list is `allItems` (GetAllItems), the buyable list only feeds InitialiseBuyableItems; the title text
//   test is one `&&` chain whose last term is a CIVCountedPointer temporary (VC homes its "constructed" flag in [ebp-0Ch]); the wide
//   price string is built and never used (dead in the PC variant, as retail); the six clickables are looked up by name and observed
//   for mouse movement through the UI manager (CObserver subobject at +4).
// Name fix (agentNF 2026-11-10): 0x004291DE is CCountedPointer<NUISystem::CComponent>::ReleaseData (the destructor is the 5-byte
//   `jmp` COMDAT 0x00429C53; Ego_r.pdb ??1 is 5 bytes): FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (engine/CCountedPointer.h).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_CTCSHOP_SHOPTYPE
#define FABLE_RETAIL_CTCSHOP_NAME
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
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CText.h"
#include "engine/CCountedPointer.h"
// The controlled creature is only PeekTC'd here: a complete stand-in derived from CThing is enough (the landed
// NPlayerGui rows, e.g. 0x0064DB2B, model it the same way; engine/CThingPlayerCreature.h needs the whole creature tree).
class CThingPlayerCreature : public CThing { };

CWideString FABLE_FASTCALL GFIntToWideString(long value);          // 0x00405F00 ?GFIntToWideString@@YI?AVCWideString@@J@Z

using namespace NUISystem;

void CTCInventoryTrade::ConstructActualSpecialInventoryPC()
{
    ItemBuyLists.clear();
    ResetTradeLists();
    std::vector<std::pair<long, long> > buyableItems;
    std::vector<std::pair<long, long> > allItems;
    CTCShop* pShop = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &pShop);
    InitialiseBuyableItems(buyableItems);
    GetAllItems(allItems);
    if (pShop->IsBarberShop()) {
        // a barber only deals in hair cards: drop every item without a hair-card def
        for (std::vector<std::pair<long, long> >::iterator it = allItems.begin(); it != allItems.end(); ) {
            CDefPointer<const CHairCardDef> pHairCard = CTCInventoryItem::GetHairCardDef(it->first);
            if (pHairCard.IsNull())
                it = allItems.erase(it);
            else
                ++it;
        }
    } else if (pShop->IsTattooShop()) {
        // a tattooist only deals in valid tattoo cards
        for (std::vector<std::pair<long, long> >::iterator it = allItems.begin(); it != allItems.end(); ) {
            CDefPointer<const CTattooDef> pTattooCard = CTCInventoryItem::GetTattooCardDef(it->first);
            if (!pTattooCard.IsNull() && !pTattooCard->IsInvalidCustomTattoo())
                ++it;
            else
                it = allItems.erase(it);
        }
    } else {
        pShop->IsTitleShop();   // the title vendor sells everything it stocks (the test's result is unused in retail)
    }
    ItemsToBuy.clear();
    BuyListsByCategory.clear();

    CManager* pUIManager = CManager::GetInstance();
    CCountedPointer<CComponent> pFrame(pUIManager->CreateComponent("PC_UI_INACTIVE_FRAME", false));
    CCountedPointer<CComponent> pPositionDummy(pUIManager->CreateComponent("PC_LIST_POSITION_DUMMY", false));
    pPositionDummy->AddPositionChild(pFrame.Get());

    CCountedPointer<CComponent> pTitle;
    if (pShop->IsBarberShop())
        pTitle.Reset(pUIManager->CreateComponent("PC_TRADE_BARBER_TITLE", false));
    else if (pShop->IsTattooShop())
        pTitle.Reset(pUIManager->CreateComponent("PC_TRADE_TATTOOIST_TITLE", false));
    else
        pTitle.Reset(pUIManager->CreateComponent("PC_TRADE_TITLE_VENDOR_TITLE", false));
    if (pShop->GetName() > 0) {
        bool haveName = CMainGameComponent::Get()->GetTextBank().EntryExists(pShop->GetName())
                     && CMainGameComponent::Get()->GetTextBank().GetTextInfo(pShop->GetName()) != 0
                     && !CMainGameComponent::Get()->GetTextBank().GetPTextEntry(pShop->GetName(), true).IsNull();
        if (haveName) {
            CText* pTitleText = static_cast<CText*>(pTitle.Get());
            if (pTitleText)
                pTitleText->SetValue(CMainGameComponent::Get()->GetTextBank().GetPTextEntry(pShop->GetName(), true)->GetText(), true, true);
        }
    }

    CCountedPointer<CComponent> pDescriptionViewport(pUIManager->CreateComponent("PC_DESCRIPTION_VIEWPORT", false));
    CViewport* pViewport = static_cast<CViewport*>(pDescriptionViewport.Get());
    pFrame->AddPositionChild(pViewport);
    PTop_Level_List->FindChild("PC_SPECIALSHOP_MENU_BUY_ENTRY");
    CList* pParentList = static_cast<CList*>(pUIManager->CreateComponent("PC_PARENT_LIST", false));
    pParentList->AddNonScrollingChild(pPositionDummy);
    pParentList->AddNonScrollingChild(pFrame);
    pParentList->AddNonScrollingChild(pDescriptionViewport);
    pParentList->AddNonScrollingChild(pTitle);
    pFrame->AddPositionChild(pTitle.Get());

    TotSpecialItemsToBuy = 0;
    if (!allItems.empty() || !NotInStock.empty()) {
        pParentList->InitialiseOffsets(allItems.size() + NotInStock.size());
        for (std::vector<std::pair<long, long> >::iterator it = allItems.begin(); it != allItems.end(); ++it) {
            if (AddBuyItemToListPC(pFrame.Get(), it->first, it->second, pParentList, pViewport, false))
                TotSpecialItemsToBuy += it->second;
        }
        CWideString priceText(SpecialItemString);
        priceText += CTCInventoryStats::GetXString();
        priceText += GFIntToWideString(TotSpecialItemsToBuy);
        CCharString guideSymbol;
        if (pShop->IsBarberShop())
            guideSymbol = "TEXT_GUI_MENUGUIDE_TRADE_BARBERS_BUY";
        else if (pShop->IsTattooShop())
            guideSymbol = "TEXT_GUI_MENUGUIDE_TRADE_TATTOOIST_BUY";
        else
            guideSymbol = "TEXT_GUI_MENUGUIDE_TRADE_TITLEVENDOR_BUY";
        CCountedPointer<CComponent> pDescription(pUIManager->CreateComponent("PC_DESCRIPTION", false));
        static_cast<CText*>(pDescription.Get())->SetValue(CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(guideSymbol), true, true);
        pFrame->AddPositionChild(pDescription.Get());
        for (std::list<long>::iterator it = NotInStock.begin(); it != NotInStock.end(); ++it)
            AddBuyItemToListPC(pFrame.Get(), *it, 0, pParentList, pViewport, false);
    }

    CCountedPointer<CComponent> pGoldAmount(pUIManager->CreateComponent("PC_GOLD_AMOUNT_TRADE_SCREEN", false));
    CText* pGoldText = static_cast<CText*>(pGoldAmount.Get());
    pParentList->AddNonScrollingChild(pGoldAmount);
    pFrame->AddPositionChild(pGoldAmount.Get());
    CCountedPointer<CComponent> pGoldCoin(pUIManager->CreateComponent("PC_GOLD_COIN_SPRITE", false));
    pParentList->AddNonScrollingChild(pGoldCoin);
    pFrame->AddPositionChild(pGoldCoin.Get());
    const CTCHeroStats* pHeroStats = 0;
    CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature()->PeekTC(TCI_HERO_STATS, &pHeroStats);
    pGoldText->SetValue(GFIntToWideString(pHeroStats->GetMoney()), true, true);

    PCurrentOpenTradeMenu = pParentList;
    pParentList->Initialise();
    PCurrentOpenTradeMenu->ChangeState(5);
    Components.push_back(PCurrentOpenTradeMenu);

    CComponent* pClickable = pParentList->FindChild("SCROLL_LIST_UP");
    if (pClickable) {
        CManager::GetInstance()->AddObserver(pClickable);
        pClickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    pClickable = pParentList->FindChild("SCROLL_DESCRIPTION_UP");
    if (pClickable) {
        CManager::GetInstance()->AddObserver(pClickable);
        pClickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    pClickable = pParentList->FindChild("SCROLL_LIST_DOWN");
    if (pClickable) {
        CManager::GetInstance()->AddObserver(pClickable);
        pClickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    pClickable = pParentList->FindChild("SCROLL_DESCRIPTION_DOWN");
    if (pClickable) {
        CManager::GetInstance()->AddObserver(pClickable);
        pClickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    pClickable = pParentList->FindChild("PC_BACK_CLICKABLE");
    if (pClickable) {
        CManager::GetInstance()->AddObserver(pClickable);
        pClickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    pClickable = pParentList->FindChild("PC_CLOSE_CLICKABLE");
    if (pClickable) {
        CManager::GetInstance()->AddObserver(pClickable);
        pClickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
}