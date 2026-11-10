// CTCInventoryTrade::AddWantedItemToListPC(NUISystem::CComponent* frame, long itemDefIndex, NUISystem::CList* list, NUISystem::CViewport* viewport) -- PC wanted-items screen: builds a PC menu entry for one wanted item, wires its five actions (add the item model on select, remove it on unselect, load the wanted-item description into the viewport on select, unload it on unselect, load the entry's name when it becomes visible; the frame is an extra target of the select actions) and adds the entry to the list
// Leaf: ?AddWantedItemToListPC@CTCInventoryTrade@@QAEXPAVCComponent@NUISystem@@JPAVCList@3@PAVCViewport@3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059D270, 1002 bytes, thiscall `ret 10h`. Frame-pointer size TU: optimize("y",off) + ("s",on).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_PCINVENTORY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
#define FABLE_RETAIL_CMENUENTRY_UNSELECTED_VISIBLE
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
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

void CTCInventoryTrade::AddWantedItemToListPC(CComponent* frame, long itemDefIndex, CList* list, CViewport* viewport)
{
    CManager* manager = CManager::GetInstance();
    CCountedPointer<CComponent> entry;
    CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
    entry.Reset(manager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                  0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    CMenuEntry* menuEntry = static_cast<CMenuEntry*>(entry.Get());
    CCountedPointer<CAction> addModel(new CAction);
    addModel->ActionType = UI_ACTION_ADD_MESH_CHILD;
    addModel->UIActionParams.push_back(new CActionParamSLONG(itemDefIndex));
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
    loadDesc->ActionType = UI_ACTION_LOAD_WANTED_ITEM_DESC;
    loadDesc->UIActionParams.push_back(new CActionParamSLONG(itemDefIndex));
    loadDesc->AddParam(viewport, loadDesc);
    loadDesc->AddParam(entry.Get(), loadDesc);
    loadDesc->AddParam(frame, loadDesc);
    entry->AddActionOnSelected(loadDesc);
    CCountedPointer<CAction> unloadDesc(new CAction);
    unloadDesc->ActionType = UI_ACTION_UNLOAD_WANTED_ITEM_DESC;
    unloadDesc->AddParam(entry.Get(), unloadDesc);
    menuEntry->AddActionOnUnselected(unloadDesc);
    CCountedPointer<CAction> loadName(new CAction);
    loadName->ActionType = UI_ACTION_LOAD_ITEM_MENU_ENTRY_NAME;
    loadName->UIActionParams.push_back(new CActionParamSLONG(itemDefIndex));
    loadName->AddParam(entry.Get(), loadName);
    loadName->UIActionParams.push_back(new CActionParamSLONG(1));
    menuEntry->AddActionOnVisible(loadName);
    list->AddScrollingChildDynamicallyAndSetOffsets(entry);
}