// CTCInventory::ConstructItemEntryPC(long defIndex, long count, NUISystem::CList* list, NUISystem::CComponent* owner, CCountedPointer<NUISystem::CComponent>& actionsList) -- builds one PC item menu entry: the plain or branch PC entry template (by the item's use-button action), recorded in ItemsMenuEntries; usable items get the expression-assignment icon (its sprite and faded twin showing the item's graphic, dropping it assigns the item); the mesh, description and name actions (shown in the owner's PC frame and description viewport); added to the list with the def index as its thing parameter; usable items also get the actions list and the list's observe / ignore mouse-movement actions for the icon
// Leaf: ?ConstructItemEntryPC@CTCInventory@@QAEXJJPAVCList@NUISystem@@PAVCComponent@3@AAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0058E208, 1943 bytes, thiscall `leave ; ret 14h`; frame-pointer size TU (optimize("y",off) +
//   ("s",on)), frame 0x70. Identity: Ego_r.pdb / FableWin.pdb ?ConstructItemEntryPC@CTCInventory@@QAEXJJPAVCList@NUISystem@@PAVCComponent@3@
//   AAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z (the PC twin of ConstructItemEntry 0x0058A0C7).
// Callees: as 0x0058A0C7, plus CTCInventoryItem::GetEngineGraphic(long) 0x005D96A0 (static __fastcall, by value), CSprite slot 145
//   SetGraphicIndex +0x244, CComponent slot 70 AddActionOnInactive +0x118 / 74 AddActionOnDropped +0x128, inline CActionParamULONG
//   (vtable 0x0122F844). Data: "PC_UI_FRAME" 0x0124EFA8, "PC_MENU_ENTRY_TEMPLATE" 0x01236F04, "PC_MENU_ENTRY_BRANCH_TEMPLATE" 0x01236EE4,
//   "PC_EXPRESSION_ITEM_ASSIGNMENT_ICON" 0x01251B10 (+ "_SPRITE" 0x012501F4, "_FADED" 0x01251AE4), L"PC_MODEL" 0x0125012C,
//   "PC_DESCRIPTION_VIEWPORT" 0x012370F4.
// Shape: the matched console twin 0x0058A0C7 and the landed OpenPCOptionsMenu 0x0058AF10 observe / ignore spellings.
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
#define FABLE_RETAIL_CACTIONPARAMULONG
#define FABLE_RETAIL_CTCINVENTORYITEM_STATICENGINEGRAPHIC
#define FABLE_RETAIL_CENGINEGRAPHIC_CTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInventoryItem.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_EActionType.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CActionParamString.h"
#include "engine/NUISystem_CActionParamULONG.h"

using NUISystem::CComponent;
using NUISystem::CAction;
using NUISystem::CActionParamSLONG;
using NUISystem::CActionParamULONG;

void CTCInventory::ConstructItemEntryPC(long defIndex, long count, NUISystem::CList* list, CComponent* owner,
                                        CCountedPointer<CComponent>& actionsList)
{
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CComponent* frame = owner->FindChild("PC_UI_FRAME");
    CCountedPointer<CComponent> selectMarker;
    CCountedPointer<CComponent> entry;
    if (CTCInventoryItem::GetUseButtonActionType(defIndex) == 0)
        entry.Reset(manager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE", 0.0f, "", 0.0f,
                                                      "PC_BUTTON_BASTARD"));
    else
        entry.Reset(manager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f, "", 0.0f,
                                                      "PC_BUTTON_BASTARD"));
    ItemsMenuEntries[defIndex] = entry.Get();
    NUISystem::CMenuEntry* menuEntry = static_cast<NUISystem::CMenuEntry*>(entry.Data);
    CCountedPointer<CComponent> assignIcon;
    // usable items carry the expression-assignment icon: dropping it assigns the item
    if (CTCInventoryItem::GetUseButtonActionType(defIndex)) {
        assignIcon.Reset(manager->CreateComponent("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON", false));
        NUISystem::CSprite* sprite = static_cast<NUISystem::CSprite*>(assignIcon->FindChild("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_SPRITE"));
        CCountedPointer<CComponent> faded(manager->CreateComponent("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_FADED", false));
        NUISystem::CSprite* fadedSprite = static_cast<NUISystem::CSprite*>(faded.Get());
        CCountedPointer<CAction> assign(new CAction);
        assign->ActionType = NUISystem::UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_PC;
        assign->UIActionParams.push_back(new CActionParamSLONG(defIndex));
        assignIcon->AddActionOnDropped(assign);
        if (defIndex) {
            sprite->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(defIndex).BankIndex);
            fadedSprite->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(defIndex).BankIndex);
        }
        entry->AddChild(assignIcon);
        entry->AddChild(faded);
    }
    // selected: show the item's mesh in the frame
    CCountedPointer<CAction> addMesh(new CAction);
    addMesh->ActionType = NUISystem::UI_ACTION_ADD_MESH_CHILD;
    addMesh->UIActionParams.push_back(new CActionParamSLONG(defIndex));
    addMesh->AddParam(entry.Data, addMesh);
    CWideString modelName(L"PC_MODEL");
    addMesh->UIActionParams.push_back(new NUISystem::CActionParamString(modelName));
    addMesh->AddParam(frame, addMesh);
    entry->AddActionOnSelected(addMesh);
    // unselected: remove it
    CCountedPointer<CAction> removeMesh(new CAction);
    removeMesh->ActionType = NUISystem::UI_ACTION_REMOVE_MESH_CHILD;
    removeMesh->AddParam(entry.Data, removeMesh);
    menuEntry->AddActionOnUnselected(removeMesh);
    // selected: the item's description in the owner's description viewport
    CCountedPointer<CAction> loadDescription(new CAction);
    loadDescription->ActionType = NUISystem::UI_ACTION_LOAD_ITEM_DESC;
    loadDescription->UIActionParams.push_back(new CActionParamSLONG(defIndex));
    loadDescription->AddParam(owner->FindChild("PC_DESCRIPTION_VIEWPORT"), loadDescription);
    loadDescription->AddParam(entry.Data, loadDescription);
    loadDescription->UIActionParams.push_back(new CActionParamSLONG(count));
    loadDescription->AddParam(frame, loadDescription);
    entry->AddActionOnSelected(loadDescription);
    CCountedPointer<CAction> unloadDescription(new CAction);
    unloadDescription->ActionType = NUISystem::UI_ACTION_UNLOAD_ITEM_DESC;
    unloadDescription->AddParam(entry.Data, unloadDescription);
    menuEntry->AddActionOnUnselected(unloadDescription);
    // visible: the entry's name and count
    CCountedPointer<CAction> loadName(new CAction);
    loadName->ActionType = NUISystem::UI_ACTION_LOAD_ITEM_MENU_ENTRY_NAME;
    loadName->UIActionParams.push_back(new CActionParamSLONG(defIndex));
    loadName->AddParam(entry.Data, loadName);
    loadName->UIActionParams.push_back(new CActionParamSLONG(count));
    menuEntry->AddActionOnVisible(loadName);
    menuEntry->AddThingActionParam(defIndex);
    list->AddScrollingChildAndSetOffsets(entry);
    // usable items: the actions list, and the list watches the mouse over the icon while selected
    if (CTCInventoryItem::GetUseButtonActionType(defIndex)) {
        entry->AddChild(actionsList);
        CCountedPointer<CAction> observe(new CAction);
        observe->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observe->AddParam(assignIcon.Data, observe);
        observe->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(observe);
        CCountedPointer<CAction> ignore(new CAction);
        ignore->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignore->AddParam(assignIcon.Data, ignore);
        ignore->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnInactive(ignore);
    }
}