// CTCInventory::ConstructItemEntry(long defIndex, long count, NUISystem::CList* list, NUISystem::CComponent* owner, CCountedPointer<NUISystem::CComponent>& actionsList) -- builds one console item menu entry: the plain or branch entry template (by the item's use-button action), recorded in ItemsMenuEntries; a select marker on usable items; the assigned quick-access icon (left / right / up / down) recorded in AssignedIcons; the mesh, description and name actions; added to the list, the def index as its thing parameter, and the actions list attached to usable items
// Leaf: ?ConstructItemEntry@CTCInventory@@QAEXJJPAVCList@NUISystem@@PAVCComponent@3@AAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0058A0C7, 1629 bytes, thiscall `ret 14h`; frame-pointer size TU (optimize("y",off) + ("s",on)),
//   frame 0x48. Identity: Ego_r.pdb / FableWin.pdb ?ConstructItemEntry@CTCInventory@@QAEXJJPAVCList@NUISystem@@PAVCComponent@3@AAV?$
//   CCountedPointer@VCComponent@NUISystem@@@@@Z (the PC twin ConstructItemEntryPC 0x0058E208).
// Callees: NUISystem::CManager::GetInstance 0x0041E5F2 / CreateMenuEntryComponent 0x00428494 / CreateComponent 0x0041DB1D;
//   CTCInventoryItem::GetUseButtonActionType 0x005D9710 (static __fastcall; Ego_r.pdb / FableWin.pdb, the landed descriptive spelling
//   LookUpInventoryItemDef is wrong); CCountedPointer<CComponent>(T*) 0x00429C15 / Reset 0x00429C6F / ReleaseData 0x004291DE; the STLport
//   map<long, CComponent*>::operator[] 0x005949C3 (ItemsMenuEntries / AssignedIcons); CAction() 0x0042BE50, CCountedPointer<CAction>(T*)
//   0x0042AA29 / ReleaseData 0x00429160, operator new 0x00BFEA1A, list<CActionParam*>::push_back 0x0042B646, CAction::AddParam(
//   CActionParam*, CCountedPointer<CAction>&) 0x0053FDE0, CActionParamString(CWideString&) 0x0054DAE0, inline CActionParamSLONG (vtable
//   0x012366F4); CWideString 0x0099B6B0 / ~ 0x0099B510, CCharString 0x0099EBF0 / ~ 0x0099EAE0. Virtual slots: CComponent 3 FindChild,
//   59 AddChild +0xEC, 67 AddActionOnSelected +0x10C; CMenuEntry 150 AddThingActionParam +0x258, 152 AddActionOnUnselected +0x260,
//   153 AddActionOnVisible +0x264; CList 152 AddScrollingChildAndSetOffsets +0x260.
// Shape: the landed CTCInventoryWeapons::FinalConstructWeaponItemEntries 0x005C3E91 spellings; `map[k] = ptr.Get()` (the accessor read
//   before the visible STLport operator[] call, &defIndex passed directly); VC cross-jumps the four identical quick-access tails.
// Header: overlay FABLE_HDR_OVERLAY (FABLE_RETAIL_CTCINVENTORY_ITEMENTRY, FABLE_RETAIL_CTCINVENTORYITEM_USEBUTTONACTION).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORY_ITEMENTRY
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_USEBUTTONACTION
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#define FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
#define FABLE_RETAIL_CMENUENTRY_ACTIONLISTS
#define FABLE_RETAIL_CACTIONPARAMSLONG
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInventoryItem.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_EActionType.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CActionParamString.h"

using NUISystem::CComponent;
using NUISystem::CAction;

void CTCInventory::ConstructItemEntry(long defIndex, long count, NUISystem::CList* list, CComponent* owner,
                                      CCountedPointer<CComponent>& actionsList)
{
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CCountedPointer<CComponent> selectMarker;
    CCountedPointer<CComponent> entry;
    if (CTCInventoryItem::GetUseButtonActionType(defIndex) == 0)
        entry.Reset(manager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_TEMPLATE", 0.0f, "", 0.0f,
                                                      "PC_BUTTON_BASTARD"));
    else
        entry.Reset(manager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f, "",
                                                      0.0f, "PC_BUTTON_BASTARD"));
    ItemsMenuEntries[defIndex] = entry.Get();
    // usable items carry the select marker
    if (CTCInventoryItem::GetUseButtonActionType(defIndex)) {
        if (!selectMarker.Data)
            selectMarker.Reset(manager->CreateComponent("UI_SELECT_FOR_MENU_ENTRY", false));
        entry->AddChild(selectMarker);
    }
    // the quick-access slot the item is assigned to shows its icon
    if (QuickAccessItems[2] == defIndex) {
        CCountedPointer<CComponent> icon(manager->CreateComponent("UI_ASSIGNED_LEFT_ITEM_ICON", false));
        entry->AddChild(icon);
        AssignedIcons[2] = icon.Get();
    } else if (QuickAccessItems[3] == defIndex) {
        CCountedPointer<CComponent> icon(manager->CreateComponent("UI_ASSIGNED_RIGHT_ITEM_ICON", false));
        entry->AddChild(icon);
        AssignedIcons[3] = icon.Get();
    } else if (QuickAccessItems[0] == defIndex) {
        CCountedPointer<CComponent> icon(manager->CreateComponent("UI_ASSIGNED_UP_ITEM_ICON", false));
        entry->AddChild(icon);
        AssignedIcons[0] = icon.Get();
    } else if (QuickAccessItems[1] == defIndex) {
        CCountedPointer<CComponent> icon(manager->CreateComponent("UI_ASSIGNED_DOWN_ITEM_ICON", false));
        entry->AddChild(icon);
        AssignedIcons[1] = icon.Get();
    }
    NUISystem::CMenuEntry* menuEntry = static_cast<NUISystem::CMenuEntry*>(entry.Data);
    // selected: show the item's mesh
    CCountedPointer<CAction> addMesh(new CAction);
    addMesh->ActionType = NUISystem::UI_ACTION_ADD_MESH_CHILD;
    addMesh->UIActionParams.push_back(new NUISystem::CActionParamSLONG(defIndex));
    addMesh->AddParam(entry.Data, addMesh);
    CWideString modelName(L"UI_ITEM_MODEL_TEMPLATE_NEW");
    addMesh->UIActionParams.push_back(new NUISystem::CActionParamString(modelName));
    entry->AddActionOnSelected(addMesh);
    // unselected: remove it
    CCountedPointer<CAction> removeMesh(new CAction);
    removeMesh->ActionType = NUISystem::UI_ACTION_REMOVE_MESH_CHILD;
    removeMesh->AddParam(entry.Data, removeMesh);
    menuEntry->AddActionOnUnselected(removeMesh);
    // selected: the item's description in the owner's text area
    CCountedPointer<CAction> loadDescription(new CAction);
    loadDescription->ActionType = NUISystem::UI_ACTION_LOAD_ITEM_DESC;
    loadDescription->UIActionParams.push_back(new NUISystem::CActionParamSLONG(defIndex));
    loadDescription->AddParam(owner->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA"), loadDescription);
    loadDescription->AddParam(entry.Data, loadDescription);
    loadDescription->UIActionParams.push_back(new NUISystem::CActionParamSLONG(count));
    entry->AddActionOnSelected(loadDescription);
    CCountedPointer<CAction> unloadDescription(new CAction);
    unloadDescription->ActionType = NUISystem::UI_ACTION_UNLOAD_ITEM_DESC;
    unloadDescription->AddParam(entry.Data, unloadDescription);
    menuEntry->AddActionOnUnselected(unloadDescription);
    // visible: the entry's name and count
    CCountedPointer<CAction> loadName(new CAction);
    loadName->ActionType = NUISystem::UI_ACTION_LOAD_ITEM_MENU_ENTRY_NAME;
    loadName->UIActionParams.push_back(new NUISystem::CActionParamSLONG(defIndex));
    loadName->AddParam(entry.Data, loadName);
    loadName->UIActionParams.push_back(new NUISystem::CActionParamSLONG(count));
    menuEntry->AddActionOnVisible(loadName);
    menuEntry->AddThingActionParam(defIndex);
    list->AddScrollingChildAndSetOffsets(entry);
    if (CTCInventoryItem::GetUseButtonActionType(defIndex))
        entry->AddChild(actionsList);
}