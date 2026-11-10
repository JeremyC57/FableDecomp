// CTCInventoryTrade::AddBuyItemToList(long itemDefIndex, long count, NUISystem::CList* list, NUISystem::CViewport* viewport, bool dynamically) -- console shop: builds the buy-menu entry for one shop item (a branch entry when the hero can afford it, has room for it or it is a special case, the shop has some and the hero does not already own the special; else a plain entry made unavailable), records it in ItemsToBuy, wires its seven actions (add / remove the item model, load / unload the buy description and the buy-actions menu, load the entry's name) and adds it to the list; returns whether it is a branch entry
// Grade: DIFFER(1552v1556) -- shared headers; register allocation of `this` vs the inventory pointer: retail keeps the inventory (0-initialised: `xor ebx,ebx` also zeroes the entry handle) in ebx and splits `this` (memory home [ebp-0Ch], then esi), ours gives `this` ebx (c2prio 17 v inventory 13) and keeps alreadyHave in the dead parameter byte; frame 60h v 64h, every later slot shifts 4
// Leaf: ?AddBuyItemToList@CTCInventoryTrade@@QAE_NJJPAVCList@NUISystem@@PAVCViewport@3@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059FEE3, 1556 bytes, thiscall `ret 14h`, bool in al. Frame-pointer size TU: optimize("y",off) + ("s",on).
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
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_ACTIONSLISTS
#define FABLE_RETAIL_CTCINVENTORYTRADE_SHOPLISTS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
#define FABLE_RETAIL_CMENUENTRY_UNSELECTED_VISIBLE
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CActionParamString.h"
using namespace NUISystem;

class CThingPlayerCreature : public CThing { };

bool CTCInventoryTrade::AddBuyItemToList(long itemDefIndex, long count, CList* list, CViewport* viewport, bool dynamically)
{
    bool isBranch = true;
    CManager* manager = CManager::GetInstance();
    CCountedPointer<CComponent> entry;
    const CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    const NInventory::CTCInventoryBase* inventory = 0;
    ETCInterfaceType inventoryType = CTCInventoryItem::GetInventoryType(itemDefIndex);
    if (hero->HasTC(inventoryType))
        inventory = static_cast<const NInventory::CTCInventoryBase*>(&hero->GetTC(inventoryType));
    bool alreadyHave = AlreadyHaveSpecial(itemDefIndex);
    if (IsItemAffordable(itemDefIndex, 1) && (inventory->IsSpaceInInventoryForItem(itemDefIndex) || IsSpecialCase(itemDefIndex)) &&
        count > 0 && !alreadyHave) {
        entry.Reset(manager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",
                                                      0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    } else {
        isBranch = false;
        entry.Reset(manager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW",
                                                      0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        entry->MakeUnavailable();
    }
    ItemsToBuy.insert(std::map<CComponent*, std::pair<long, long> >::value_type(entry.Get(), std::pair<long, long>(itemDefIndex, count)));
    CMenuEntry* menuEntry = static_cast<CMenuEntry*>(entry.Get());
    CCountedPointer<CAction> addModel(new CAction);
    addModel->ActionType = UI_ACTION_ADD_MESH_CHILD;
    addModel->UIActionParams.push_back(new CActionParamSLONG(itemDefIndex));
    addModel->AddParam(entry.Get(), addModel);
    CWideString model(L"UI_ITEM_MODEL_TEMPLATE_NEW");
    addModel->UIActionParams.push_back(new CActionParamString(model));
    entry->AddActionOnSelected(addModel);
    CCountedPointer<CAction> removeModel(new CAction);
    removeModel->ActionType = UI_ACTION_REMOVE_MESH_CHILD;
    removeModel->AddParam(entry.Get(), removeModel);
    menuEntry->AddActionOnUnselected(removeModel);
    CCountedPointer<CAction> loadDesc(new CAction);
    loadDesc->ActionType = UI_ACTION_LOAD_BUY_ITEM_DESC;
    loadDesc->UIActionParams.push_back(new CActionParamSLONG(itemDefIndex));
    loadDesc->AddParam(viewport, loadDesc);
    loadDesc->AddParam(entry.Get(), loadDesc);
    entry->AddActionOnSelected(loadDesc);
    CCountedPointer<CAction> unloadDesc(new CAction);
    unloadDesc->ActionType = UI_ACTION_UNLOAD_BUY_ITEM_DESC;
    unloadDesc->AddParam(entry.Get(), unloadDesc);
    menuEntry->AddActionOnUnselected(unloadDesc);
    CCountedPointer<CAction> loadActions(new CAction);
    loadActions->ActionType = UI_ACTION_LOAD_BUY_ACTION_MENU;
    loadActions->UIActionParams.push_back(new CActionParamSLONG(itemDefIndex));
    loadActions->AddParam(entry.Get(), loadActions);
    entry->AddPreAction(loadActions);
    CCountedPointer<CAction> unloadActions(new CAction);
    unloadActions->ActionType = UI_ACTION_UNLOAD_BUY_ACTION_MENU;
    unloadActions->AddParam(entry.Get(), unloadActions);
    menuEntry->AddActionOnUnselected(unloadActions);
    CCountedPointer<CAction> loadName(new CAction);
    loadName->ActionType = UI_ACTION_LOAD_ITEM_MENU_ENTRY_NAME;
    loadName->UIActionParams.push_back(new CActionParamSLONG(itemDefIndex));
    loadName->AddParam(entry.Get(), loadName);
    loadName->UIActionParams.push_back(new CActionParamSLONG(count));
    menuEntry->AddActionOnVisible(loadName);
    menuEntry->AddUIActionParam(entry.Get());
    if (dynamically)
        list->AddScrollingChildDynamicallyAndSetOffsets(entry);
    else
        list->AddScrollingChildAndSetOffsets(entry);
    return isBranch;
}
