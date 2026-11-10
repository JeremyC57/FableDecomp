// CTCInventoryTrade::ConstructBuyActionsList(NUISystem::CComponent* parent, long itemDefIndex, long count, bool dynamically) -- console shop: builds the buy-actions sub-list for one shop item (the buyable quantity is the stock count capped by the room in the hero's inventory and stepped down until affordable; a "buy" entry plus "buy 10", "buy 50" and "buy all (n)" entries as that quantity allows, never in a barber's, tattooist's or title shop); when the item is affordable, fits (or is a special case) and is in stock, remembers the list in ItemBuyLists[parent], attaches it to the parent and returns true, else returns false
// Leaf: ?ConstructBuyActionsList@CTCInventoryTrade@@QAE_NPAVCComponent@NUISystem@@JJ_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A3E47, 1967 bytes, thiscall `ret 10h`, bool in al (bl). Frame-pointer size TU: optimize("y",off) + ("s",on).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_CONTAINABLE
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_ACTIONSLISTS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCSHOP
#define FABLE_RETAIL_CTCSHOP_SHOPTYPE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CACTIONPARAMULONG
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamULONG.h"
using namespace NUISystem;
using NGameText::CDataBank;

class CThingPlayerCreature : public CThing { };

CWideString FABLE_FASTCALL GFIntToWideString(long value);                 // 0x00405F00

bool CTCInventoryTrade::ConstructBuyActionsList(CComponent* parent, long itemDefIndex, long count, bool dynamically)
{
    CManager* manager = CManager::GetInstance();
    CCountedPointer<CComponent> actionsList(CManager::GetInstance()->CreateComponent("UI_LIST_BUY_ACTIONS", false));
    CList* list = static_cast<CList*>(actionsList.Get());
    const NInventory::CTCInventoryBase* inventory = 0;
    const CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    CTCShop* shop = 0;
    hero->PeekTC(CTCInventoryItem::GetInventoryType(itemDefIndex), &inventory);
    GetBuildingThing()->GetTC(TCI_SHOP, &shop);
    const CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    CWideString buyText = textBank.GetTextBySymbol("TEXT_GUI_MENU_BUY");
    CCountedPointer<CComponent> buyEntry(manager->CreateMenuEntryComponent(buyText, 25.0f, 0.66666f, "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW",
                                                                           0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    CCountedPointer<CAction> buyAction(new CAction);
    buyAction->ActionType = UI_ACTION_TYPE_BUY;
    buyEntry->AddAction(buyAction);
    long containable = inventory->GetAmountOfItemsContainable(itemDefIndex);
    long quantity = count;
    if (containable <= count)
        quantity = containable;
    for (; quantity >= 1; --quantity)
        if (IsItemAffordable(itemDefIndex, quantity))
            break;
    if (quantity > 50)
        list->InitialiseOffsets(4);
    else if (quantity > 10)
        list->InitialiseOffsets(3);
    else if (quantity > 1)
        list->InitialiseOffsets(2);
    else
        list->InitialiseOffsets(1);
    list->SetDefaultChild(0L);
    list->AddScrollingChildAndSetOffsets(buyEntry);
    if (quantity > 10 && !shop->IsBarberShop() && !shop->IsTattooShop() && !shop->IsTitleShop()) {
        CWideString label = textBank.GetTextBySymbol("TEXT_GUI_MENU_BUY") + L" " + GFIntToWideString(10);
        CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(label, 25.0f, 0.66666f, "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW",
                                                                            0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_BUY_BULK;
        action->UIActionParams.push_back(new CActionParamULONG(10));
        entry->AddAction(action);
        list->AddScrollingChildAndSetOffsets(entry);
    }
    if (quantity > 50 && !shop->IsBarberShop() && !shop->IsTattooShop() && !shop->IsTitleShop()) {
        CWideString label = textBank.GetTextBySymbol("TEXT_GUI_MENU_BUY") + L" " + GFIntToWideString(50);
        CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(label, 25.0f, 0.66666f, "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW",
                                                                            0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_BUY_BULK;
        action->UIActionParams.push_back(new CActionParamULONG(50));
        entry->AddAction(action);
        list->AddScrollingChildAndSetOffsets(entry);
    }
    if (quantity > 1 && !shop->IsBarberShop() && !shop->IsTattooShop() && !shop->IsTitleShop()) {
        CWideString label = textBank.GetTextBySymbol("TEXT_GUI_MENU_BUY_ALL") + L" (" + GFIntToWideString(quantity) + L")";
        CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(label, 25.0f, 0.66666f, "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW",
                                                                            0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_BUY_BULK;
        action->UIActionParams.push_back(new CActionParamULONG(quantity));
        entry->AddAction(action);
        list->AddScrollingChildAndSetOffsets(entry);
    }
    if (IsItemAffordable(itemDefIndex, 1) && (inventory->IsSpaceInInventoryForItem(itemDefIndex) || IsSpecialCase(itemDefIndex)) && count != 0) {
        ItemBuyLists[parent] = actionsList.Get();
        if (dynamically)
            parent->AddChildDynamically(actionsList);
        else
            parent->AddChild(actionsList);
        return true;
    }
    return false;
}