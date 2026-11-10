// CTCInventoryTrade::OpenInventory() -- open the console trade screen: count the open menu, fade the sound groups 0x10 / 0x40 / 0x80 out, stop 3-D scene rendering, hand the UI manager the hero, clear the item-list maps, switch the hero-information screens to the trade screen (remembering the previous one), open the base view and set its animation box from the inventory def, then reset the trade lists, collect the shop's sellable / buyable / owned items, refresh the highlighted item's owned count and build the trading menu (barber / tattooist / title-vendor menus of the hero's cards, or the buy / sell / wanted menu of a normal shop, whose entries are then refreshed) or close the screen when trading is disabled
// Grade: DIFFER(1488v1482) -- frame now retail's (0x40, every [ebp-x] slot as retail) and instruction-for-instruction retail except: (1) this=edi / menu=esi where retail has this=esi / menu=edi (c2prio: pMenu prio 74 colours before this 72; retail needs this first); (2) the first normal-shop erase loop (buyableItems) is entered with a duplicated pre-test (`cmp; je`) where retail jumps to the bottom test (the first loop of that else-branch gets it whichever vector it walks); (3) the const-0 ebx piece starts at the `highlighted > 0` compare (`xor ebx,ebx; cmp eax,ebx`) where retail tests eax and zeroes ebx in the next block.
// Leaf: ?OpenInventory@CTCInventoryTrade@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A94BD, 1482 bytes, plain `ret`. Frame-pointer size TU: optimize("y",off) + ("s",on).
//   Identity (chunk 1009 NOTES, two sources): Ego_r.pdb / FableWin.pdb ?OpenInventory@CTCInventoryTrade@@UAEXXZ and CTCInventoryTrade
//   vtable 0x01253034 slot 30 = 0x005A94BD (slot 31 CloseInventory 0x0059F304, slot 86 OpenInventoryPC 0x005AE86B).
// Callees: NUISystem::CManager::OnOpenMenus 0x0041D122 (static); PSoundManager 0x013B8394 CASoundManager::SetGainFader (slot 46);
//   CIEngine::Enable3DSceneRendering (slot 76) through CMainGameComponent::Get() (0x013B86A0) +0x28 / +0x2C; CManager::GetInstance
//   0x0041E5F2 + SetPlayerThing (slot 26) of CThing::GetPlayer 0x004C7A30 -> CPlayer::GetPControlledCreature 0x00487DC0; the
//   CComponent map clears 0x0042ADE5 (ItemSellLists +0x20C, ItemBuyLists +0x200); NHeroInformationScreens::CBase::IsInventoryScreen
//   (slot 3 of the +0x130 base), CPlayerGui::GetPHeroInfoScreensManager 0x00437CD0 (CTBaseSingletonNoDefaultAlloc<CPlayerGui>
//   0x013B8790), NHeroInformationScreens::CManager::GetCurrentScreen 0x0066E7BD (-> GetScreenType, slot 2) / SelectScreenOfType
//   0x0066E799 (INVENTORY_TRADE_SCREEN); CTCInventoryBase::OpenInventory 0x005BD4EA (qualified) and SetInventoryAnimRenderBox
//   0x005BC58F (C2DBoxF of the inventory def's MeshAreaTLPos / MeshAreaBRPos through two inline GetPDef copies); then as
//   ConstructActualInventoryPC 0x005ACEDA with the console list builders ConstructSpecialShopList 0x005A29F4 (menu names
//   UI_BARBER_MENU / UI_TATOOIST_MENU / UI_TITLESHOP_MENU) / ConstructNormalShopList 0x005A04F7, and for a normal shop
//   RefreshEntriesAmountsAndAvailabilities 0x0059DB82 of the BUY / SELL / WANTED entries (CComponent::FindChild slot 3, by value).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_HIGHLIGHTEDITEM
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYBASE_ANIMBOX
#define FABLE_RETAIL_CTCINVENTORYBASE_PDEF
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_OPENINVENTORY
#define FABLE_RETAIL_CTCINVENTORYTRADE_PCINVENTORY
#define FABLE_RETAIL_CTCINVENTORYTRADE_SHOPLISTS
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
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_C2DBOXF_CORNERS_FORCEINLINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CMANAGER_MENUCOUNT
#include "engine/C2DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CInventoryDef.h"
#include "engine/CTCShop.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTattooDef.h"
#include "engine/CHairCardDef.h"
#include "engine/CASoundManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerGui.h"
#include "engine/NHeroInformationScreens_CManager.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
// The controlled creature only goes to SetPlayerThing(CThing*): a complete stand-in derived from CThing is enough (as the landed
// 0x005A8C66; engine/CThingPlayerCreature.h needs the whole creature tree).
class CThingPlayerCreature : public CThing { };

#pragma auto_inline(off)
// Same-TU body of vector<NUISystem::CComponent*>::push_back (0x00486024, out of line in retail; the STLport 4.6 _vector.h body):
// with it visible c2 knows the pushed menu pointer is only read, so the menu stays in edi across the call (retail 0x005A99AA ..
// 0x005A9A45 `mov ecx,edi`). Only OpenInventory is graded.
template<> __declspec(noinline) void std::vector<NUISystem::CComponent*>::push_back(NUISystem::CComponent* const& x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish, x);
        ++this->_M_finish;
    } else
        _M_insert_overflow(this->_M_finish, x, _IsPODType(), 1, true);
}
#pragma auto_inline(on)

void CTCInventoryTrade::OpenInventory()
{
    NUISystem::CManager::OnOpenMenus();
    if (PSoundManager) {
        PSoundManager->SetGainFader(0x10, 0.0f, 500.0f);
        PSoundManager->SetGainFader(0x40, 0.0f, 500.0f);
        PSoundManager->SetGainFader(0x80, 0.0f, 500.0f);
    }
    CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().Enable3DSceneRendering(false);
    NUISystem::CManager::GetInstance()->SetPlayerThing(ParentThing->GetPlayer().GetPControlledCreature());
    ItemSellLists.clear();
    ItemBuyLists.clear();
    if (!IsInventoryScreen()) {
        PreviousScreenType = CPlayerGui::Get().GetPHeroInfoScreensManager()->GetCurrentScreen()->GetScreenType();
        CPlayerGui::Get().GetPHeroInfoScreensManager()->SelectScreenOfType(NHeroInformationScreens::INVENTORY_TRADE_SCREEN);
    }
    NInventory::CTCInventoryBase::OpenInventory();
    SetInventoryAnimRenderBox(C2DBoxF(GetPDef()->MeshAreaTLPos, GetPDef()->MeshAreaBRPos));

    ResetTradeLists();
    IsShoppingValid = GetInventoryShoppingValidity();
    std::vector<std::pair<long, long> > sellableItems;
    // Block scope (frame): retail 0x005A95E9 builds the SetInventoryAnimRenderBox C2DBoxF at [ebp-34h], over the slots of the
    // buy / all-items vectors ([ebp-30h] / [ebp-24h]) while the sell vector keeps [ebp-40h]: the later vectors and locals are
    // in a scope of their own (frame 0x40; with every vector at function scope the box gets its own 16 bytes, frame 0x44).
    {
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
    NUISystem::CComponent* pMenu = 0;
    if (GetIsEnabled()) {
        NUISystem::CManager::GetInstance()->ClearActionParams();
        NUISystem::CManager::GetInstance()->SetInventoryTrade(this);
        if (pShop->IsBarberShop()) {
            // a barber only deals in hair cards
            for (std::vector<std::pair<long, long> >::iterator it = allItems.begin(); it != allItems.end(); ) {
                CDefPointer<const CHairCardDef> pHairCard = CTCInventoryItem::GetHairCardDef(it->first);
                if (pHairCard.IsNull())
                    it = allItems.erase(it);
                else
                    ++it;
            }
            CCharString menuName("UI_BARBER_MENU");
            pMenu = ConstructSpecialShopList(allItems, menuName);
        } else if (pShop->IsTattooShop()) {
            // a tattooist only deals in tattoo cards
            for (std::vector<std::pair<long, long> >::iterator it = allItems.begin(); it != allItems.end(); ) {
                CDefPointer<const CTattooDef> pTattooCard = CTCInventoryItem::GetTattooCardDef(it->first);
                if (pTattooCard.IsNull())
                    it = allItems.erase(it);
                else
                    ++it;
            }
            CCharString menuName("UI_TATOOIST_MENU");
            pMenu = ConstructSpecialShopList(allItems, menuName);
        } else if (pShop->IsTitleShop()) {
            CCharString menuName("UI_TITLESHOP_MENU");
            pMenu = ConstructSpecialShopList(allItems, menuName);
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
            pMenu = ConstructNormalShopList(buyableItems, sellableItems);
        }
    } else {
        AddCloseInventoryEvent();
    }
    if (pMenu) {
        pMenu->Initialise();
        pMenu->ChangeState(5);
        Components.push_back(pMenu);
        if (!pShop->IsTitleShop() && !pShop->IsTattooShop() && !pShop->IsBarberShop()) {
            RefreshEntriesAmountsAndAvailabilities(pMenu->FindChild("UI_TRADING_MENU_BUY_ENTRY"), BuyListsByCategory);
            RefreshEntriesAmountsAndAvailabilities(pMenu->FindChild("UI_TRADING_MENU_SELL_ENTRY"), SellListsByCategory);
            RefreshEntriesAmountsAndAvailabilities(pMenu->FindChild("UI_TRADING_MENU_WANTED_ENTRY"), WantedListsByCategory);
        }
    }
    }
}
