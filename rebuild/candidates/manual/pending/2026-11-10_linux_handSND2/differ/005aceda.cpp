// CTCInventoryTrade::ConstructActualInventoryPC() -- build the PC trade screen: fade the sound groups 0x10 / 0x40 / 0x80 out, reset the trade lists, cache the shopping-valid flag, collect the shop's sellable / buyable / owned item lists, refresh the highlighted item's owned count, then build the top-level list (a barber or tattooist gets the barber list of the hero's hair / valid tattoo cards, a title vendor the same list of everything, a normal shop the buy and sell lists without the card items) or close the screen when trading is disabled, and finally initialise the list and register it as a component
// Grade: DIFFER(1113v1067) -- on shared headers (overlay hdr/), instruction-for-instruction retail's selection; left, all c2 allocation/split decisions: (1) retail homes `this` at entry (`mov [ebp-8],ecx`) and keeps no register over the SetGainFader block, so its ebx/esi/edi pushes are shrink-wrapped below it and the entry test is `test ecx,ecx`; ours starts the `this` esi piece and the const-0 ebx piece at entry (`push ebx ; xor ebx,ebx ; push esi ; mov esi,ecx`, `cmp ecx,ebx`); (2) retail's closing block reloads `this` into ebx (`mov ebx,[ebp-8] ; lea esi,[ebx+220h]`), ours reloads it into esi at the end of every shop arm, which stops the three special arms' tails cross-jumping; (3) the first normal-shop erase loop gets a duplicated pre-test (`cmp ; je`) where retail jumps to the bottom test (the same residue as OpenInventory 0x005A94BD).
// Leaf: ?ConstructActualInventoryPC@CTCInventoryTrade@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005ACEDA, 1067 bytes, plain `ret`. Frame-pointer size TU: optimize("y",off) + ("s",on).
//   Identity (chunk 1009 NOTES, two sources): FableWin.pdb ?ConstructActualInventoryPC@CTCInventoryTrade@@AAEXXZ (private) and its
//   only caller OpenInventoryPC 0x005AE86B (the catalog's "OpenInventory" is vtable slot 30 = 0x005A94BD; the landed OpenInventoryPC
//   spells this callee ConstructTradeScreenPC: NAME_FIXES).
// Callees: PSoundManager 0x013B8394 CASoundManager::SetGainFader (slot 46, +0xB8: groups 0x10 / 0x40 / 0x80 to gain 0 over 500 ms),
//   ResetTradeLists 0x0059D9AD, GetInventoryShoppingValidity (slot 48, +0xC0), GetBuildingThing 0x005BC4C4 + inline GetTC (bitset::test
//   0x00410DE0, CVectorMap::LowerBound 0x0040F020), GetIsEnabled (slot 75, +0x12C), CTCShop::IsBarberShop 0x00772490 / IsTattooShop
//   0x00772510 / IsTitleShop 0x00772590, InitialiseSellableItems 0x0059D820, GetAllItems 0x005BF482, InitialiseBuyableItems 0x005A30A1,
//   GetHighlightedItem 0x005BCAE3, SetNumberOfSelectedItemOwned 0x005BE284, NUISystem::CManager::GetInstance 0x0041E5F2 +
//   ClearActionParams (slot 29, +0x74) / SetInventoryTrade (slot 23, +0x5C), CTCInventoryItem::GetHairCardDef 0x005D9A10 /
//   GetTattooCardDef 0x005D9AA0 (static __fastcall, CDefPointer by hidden pointer), CTattooDef::IsInvalidCustomTattoo 0x00581765,
//   vector<pair<long,long> >::erase 0x00593139, CCharString 0x0099EBF0 / ~ 0x0099EAE0 ("PC_TOP_LEVEL_BARBER_LIST"),
//   ConstructSpecialShopListPC 0x005A8A5D, ConstructNormalShopListPC 0x005A6769, AddCloseInventoryEvent (slot 32, +0x80),
//   CComponent::Initialise (slot 43) / ChangeState (slot 48), Components push_back 0x00486024, free 0x00BFEA14 (~vector x3).
// Shape: the normal-shop filters are one `||` of two card-def temporaries (VC homes their "constructed" bits in [ebp-4]); the three
//   special-shop arms each build their own list-name string (retail cross-jumps the tails).
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
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_HIGHLIGHTEDITEM
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_PCINVENTORY
#define FABLE_RETAIL_CTCINVENTORYTRADE_SHOPLISTS
#define FABLE_RETAIL_CTCINVENTORYTRADE_ACTUALINVENTORYPC
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_CTCSHOP_SHOPTYPE
#define FABLE_RETAIL_CTATTOODEF
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTattooDef.h"
#include "engine/CHairCardDef.h"
#include "engine/CASoundManager.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"

void CTCInventoryTrade::ConstructActualInventoryPC()
{
    if (PSoundManager) {
        PSoundManager->SetGainFader(0x10, 0.0f, 500.0f);
        PSoundManager->SetGainFader(0x40, 0.0f, 500.0f);
        PSoundManager->SetGainFader(0x80, 0.0f, 500.0f);
    }
    ResetTradeLists();
    IsShoppingValid = GetInventoryShoppingValidity();
    std::vector<std::pair<long, long> > sellableItems;
    std::vector<std::pair<long, long> > buyableItems;
    std::vector<std::pair<long, long> > allItems;
    CTCShop* pShop = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &pShop);
    if (GetIsEnabled()) {
        if (!pShop->IsBarberShop() && !pShop->IsTattooShop() && !pShop->IsTitleShop()) {
            InitialiseSellableItems();
            GetAllItems(sellableItems);
        }
        InitialiseBuyableItems(buyableItems);
        GetAllItems(allItems);
    }
    long highlighted = GetHighlightedItem();
    if (highlighted > 0)
        SetNumberOfSelectedItemOwned(highlighted);
    if (GetIsEnabled()) {
        NUISystem::CManager::GetInstance()->ClearActionParams();
        NUISystem::CManager::GetInstance()->SetInventoryTrade(this);
        if (pShop->IsBarberShop()) {
            // a barber only deals in hair cards: drop every item without a hair-card def
            for (std::vector<std::pair<long, long> >::iterator it = allItems.begin(); it != allItems.end(); ) {
                CDefPointer<const CHairCardDef> pHairCard = CTCInventoryItem::GetHairCardDef(it->first);
                if (pHairCard.IsNull())
                    it = allItems.erase(it);
                else
                    ++it;
            }
            CCharString listName("PC_TOP_LEVEL_BARBER_LIST");
            PTop_Level_List = ConstructSpecialShopListPC(allItems, listName);
        } else if (pShop->IsTattooShop()) {
            // a tattooist only deals in valid tattoo cards
            for (std::vector<std::pair<long, long> >::iterator it = allItems.begin(); it != allItems.end(); ) {
                CDefPointer<const CTattooDef> pTattooCard = CTCInventoryItem::GetTattooCardDef(it->first);
                if (!pTattooCard.IsNull() && !pTattooCard->IsInvalidCustomTattoo())
                    ++it;
                else
                    it = allItems.erase(it);
            }
            CCharString listName("PC_TOP_LEVEL_BARBER_LIST");
            PTop_Level_List = ConstructSpecialShopListPC(allItems, listName);
        } else if (pShop->IsTitleShop()) {
            CCharString listName("PC_TOP_LEVEL_BARBER_LIST");
            PTop_Level_List = ConstructSpecialShopListPC(allItems, listName);
        } else {
            // a normal shop leaves the hair and tattoo cards to the special shops
            for (std::vector<std::pair<long, long> >::iterator it = buyableItems.begin(); it != buyableItems.end(); ) {
                if (!CTCInventoryItem::GetTattooCardDef(it->first).IsNull() || !CTCInventoryItem::GetHairCardDef(it->first).IsNull())
                    it = buyableItems.erase(it);
                else
                    ++it;
            }
            for (std::vector<std::pair<long, long> >::iterator it = sellableItems.begin(); it != sellableItems.end(); ) {
                if (!CTCInventoryItem::GetTattooCardDef(it->first).IsNull() || !CTCInventoryItem::GetHairCardDef(it->first).IsNull())
                    it = sellableItems.erase(it);
                else
                    ++it;
            }
            PTop_Level_List = ConstructNormalShopListPC(buyableItems, sellableItems);
        }
    } else {
        AddCloseInventoryEvent();
    }
    if (PTop_Level_List) {
        PTop_Level_List->Initialise();
        PTop_Level_List->ChangeState(5);
        Components.push_back(PTop_Level_List);
    }
}
