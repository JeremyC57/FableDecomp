// CTCInventoryTrade::ConstructSellActionsList(NUISystem::CComponent* parent, long itemDefIndex, long count, bool dynamically) -- console shop: builds the sell-actions sub-list for one item: a "sell" entry plus "sell 10", "sell 50" and "sell all (n)" entries as the count allows; when the count is non-zero and the shop stocks the item, remembers the list in ItemSellLists[parent], attaches it to the parent and returns true, else returns false
// Leaf: ?ConstructSellActionsList@CTCInventoryTrade@@QAE_NPAVCComponent@NUISystem@@JJ_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A45F6, 1767 bytes, thiscall `ret 10h`, bool in al (bl). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Identity: Ego_r.pdb / FableWin.pdb ?ConstructSellActionsList@CTCInventoryTrade@@QAE_NPAVCComponent@NUISystem@@JJ_N@Z; the sell twin of the
//   landed ConstructBuyActionsList 0x005A3E47, ported from it.
// Callees: as 0x005A3E47, plus CTCShop::IsValidStockItem(long) const 0x007723D0; no IsItemAffordable / inventory-room clamp (the hero's
//   inventory component is looked up and dropped: only its UsingComponents test 0x00410DE0 survives, 0x005A46B6).
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
#define FABLE_RETAIL_CTCSHOP_VALIDSTOCK
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

bool CTCInventoryTrade::ConstructSellActionsList(CComponent* parent, long itemDefIndex, long count, bool dynamically)
{
    CManager* manager = CManager::GetInstance();
    CCountedPointer<CComponent> actionsList(CManager::GetInstance()->CreateComponent("UI_LIST_SELL_ACTIONS", false));
    CList* list = static_cast<CList*>(actionsList.Get());
    const NInventory::CTCInventoryBase* inventory = 0;
    const CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    CTCShop* shop = 0;
    const CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    GetBuildingThing()->GetTC(TCI_SHOP, &shop);
    hero->PeekTC(CTCInventoryItem::GetInventoryType(itemDefIndex), &inventory);
    CWideString sellText = textBank.GetTextBySymbol("TEXT_GUI_MENU_SELL");
    CCountedPointer<CComponent> sellEntry(manager->CreateMenuEntryComponent(sellText, 25.0f, 0.66666f, "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW",
                                                                            0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    CCountedPointer<CAction> sellAction(new CAction);
    sellAction->ActionType = UI_ACTION_TYPE_SELL;
    sellEntry->AddAction(sellAction);
    unsigned long quantity = count;
    if (quantity > 50)
        list->InitialiseOffsets(4);
    else if (quantity > 10)
        list->InitialiseOffsets(3);
    else if (quantity > 1)
        list->InitialiseOffsets(2);
    else
        list->InitialiseOffsets(1);
    list->SetDefaultChild(0L);
    list->AddScrollingChildAndSetOffsets(sellEntry);
    if (quantity > 10) {
        CWideString label = textBank.GetTextBySymbol("TEXT_GUI_MENU_SELL") + CWideString(L" ") + GFIntToWideString(10);
        CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(label, 25.0f, 0.66666f, "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW",
                                                                            0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_SELL_BULK;
        action->UIActionParams.push_back(new CActionParamULONG(10));
        entry->AddAction(action);
        list->AddScrollingChildAndSetOffsets(entry);
    }
    if (quantity > 50) {
        CWideString label = textBank.GetTextBySymbol("TEXT_GUI_MENU_SELL") + CWideString(L" ") + GFIntToWideString(50);
        CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(label, 25.0f, 0.66666f, "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW",
                                                                            0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_SELL_BULK;
        action->UIActionParams.push_back(new CActionParamULONG(50));
        entry->AddAction(action);
        list->AddScrollingChildAndSetOffsets(entry);
    }
    if (quantity > 1) {
        CWideString label = textBank.GetTextBySymbol("TEXT_GUI_MENU_SELL_ALL") + CWideString(L" (") + GFIntToWideString(quantity) + CWideString(L")");
        CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(label, 25.0f, 0.66666f, "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW",
                                                                            0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_SELL_BULK;
        action->UIActionParams.push_back(new CActionParamULONG(quantity));
        entry->AddAction(action);
        list->AddScrollingChildAndSetOffsets(entry);
    }
    if (quantity != 0 && shop->IsValidStockItem(itemDefIndex)) {
        ItemSellLists[parent] = actionsList.Get();
        if (dynamically)
            parent->AddChildDynamically(actionsList);
        else
            parent->AddChild(actionsList);
        return true;
    }
    return false;
}
