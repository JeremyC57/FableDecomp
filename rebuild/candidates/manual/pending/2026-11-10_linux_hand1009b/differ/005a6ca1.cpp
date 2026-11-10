// CTCInventoryTrade::AddBuyItemToListPC(NUISystem::CComponent* frame, long defIndex, long count, NUISystem::CList* list, NUISystem::CViewport* viewport, bool dynamically) -- PC shop: builds the buy-menu entry for one shop item (a branch entry when the hero can buy it, else an unavailable entry that closes the trade menu on Back for a special), records it in ItemsToBuy and wires its seven actions (model and description on select, with the PC frame as an extra target; unload on unselect; the buy-actions menu as a pre-action; the entry name when visible)
// Grade: DIFFER(1669v1662) -- shared headers; the 0x0059FEE3 residue (retail: the 0-initialised inventory in ebx, `this` split memory [ebp-0Ch] -> edi; ours: this ebx, inventory esi with a memory fallback) plus, from it, shifted temp slots and the two CreateMenuEntryComponent arms not head-merged (retail shares the `push -1` before the arm test, 0x005A6D5C)
// Leaf: ?AddBuyItemToListPC@CTCInventoryTrade@@QAE_NPAVCComponent@NUISystem@@JJPAVCList@3@PAVCViewport@3@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A6CA1, 1662 bytes, thiscall `ret 18h`, bool in al. Frame-pointer size TU: optimize("y",off) + ("s",on). PC twin of 0x0059FEE3.
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

bool CTCInventoryTrade::AddBuyItemToListPC(CComponent* frame, long defIndex, long count, CList* list, CViewport* viewport, bool dynamically)
{
    bool isBranch = true;
    CManager* manager = CManager::GetInstance();
    CCountedPointer<CComponent> entry;
    const CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    const NInventory::CTCInventoryBase* inventory = 0;
    ETCInterfaceType inventoryType = CTCInventoryItem::GetInventoryType(defIndex);
    if (hero->HasTC(inventoryType))
        inventory = static_cast<const NInventory::CTCInventoryBase*>(&hero->GetTC(inventoryType));
    bool alreadyHave = AlreadyHaveSpecial(defIndex);
    if (IsItemAffordable(defIndex, 1) && (inventory->IsSpaceInInventoryForItem(defIndex) || IsSpecialCase(defIndex)) &&
        count > 0 && !alreadyHave) {
        entry.Reset(manager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE",
                                                      0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    } else {
        isBranch = false;
        entry.Reset(manager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                      0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        entry->MakeUnavailable();
        if (IsSpecialCase(defIndex)) {
            CCountedPointer<CAction> close(new CAction);
            close->ActionType = UI_ACTION_TYPE_CLOSE_ACTIVE_TRADE_MENU;
            entry->AddActionOnBack(close);
        }
    }
    ItemsToBuy.insert(std::map<CComponent*, std::pair<long, long> >::value_type(entry.Get(), std::pair<long, long>(defIndex, count)));
    CMenuEntry* menuEntry = static_cast<CMenuEntry*>(entry.Get());
    CCountedPointer<CAction> addModel(new CAction);
    addModel->ActionType = UI_ACTION_ADD_MESH_CHILD;
    addModel->UIActionParams.push_back(new CActionParamSLONG(defIndex));
    addModel->AddParam(entry.Get(), addModel);
    CWideString model(L"PC_MODEL");
    addModel->UIActionParams.push_back(new CActionParamString(model));
    addModel->AddParam(frame, addModel);
    entry->AddActionOnSelected(addModel);
    CCountedPointer<CAction> removeModel(new CAction);
    removeModel->ActionType = UI_ACTION_REMOVE_MESH_CHILD;
    removeModel->AddParam(entry.Get(), removeModel);
    menuEntry->AddActionOnUnselected(removeModel);
    CCountedPointer<CAction> loadDesc(new CAction);
    loadDesc->ActionType = UI_ACTION_LOAD_BUY_ITEM_DESC;
    loadDesc->UIActionParams.push_back(new CActionParamSLONG(defIndex));
    loadDesc->AddParam(viewport, loadDesc);
    loadDesc->AddParam(entry.Get(), loadDesc);
    loadDesc->AddParam(frame, loadDesc);
    entry->AddActionOnSelected(loadDesc);
    CCountedPointer<CAction> unloadDesc(new CAction);
    unloadDesc->ActionType = UI_ACTION_UNLOAD_BUY_ITEM_DESC;
    unloadDesc->AddParam(entry.Get(), unloadDesc);
    menuEntry->AddActionOnUnselected(unloadDesc);
    CCountedPointer<CAction> loadActions(new CAction);
    loadActions->ActionType = UI_ACTION_LOAD_BUY_ACTION_MENU;
    loadActions->UIActionParams.push_back(new CActionParamSLONG(defIndex));
    loadActions->AddParam(entry.Get(), loadActions);
    entry->AddPreAction(loadActions);
    CCountedPointer<CAction> unloadActions(new CAction);
    unloadActions->ActionType = UI_ACTION_UNLOAD_BUY_ACTION_MENU;
    unloadActions->AddParam(entry.Get(), unloadActions);
    menuEntry->AddActionOnUnselected(unloadActions);
    CCountedPointer<CAction> loadName(new CAction);
    loadName->ActionType = UI_ACTION_LOAD_ITEM_MENU_ENTRY_NAME;
    loadName->UIActionParams.push_back(new CActionParamSLONG(defIndex));
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
