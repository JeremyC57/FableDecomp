// CTCInventory::ConstructStyleEntryPC(CComponent& list, const vector<pair<long, long> >& styles, CComponent* owner, const CCharString& headerDefinition, CComponent* viewport) -- PC twin of ConstructStyleEntry 0x00582ACE: the owner (when given) is told the header and is added as a parameter to the select and view actions; no per-entry user value. Fill the style-cards list: an empty style list just clears it; otherwise a sub-list with the header component and one menu entry per style (description text, select/focus/view/unfocus actions carrying the style index and count), then the list takes the sub-list, its name and a type-0x15 action pointing back at itself
// Leaf: ?ConstructStyleEntryPC@CTCInventory@@QAEXPAVCComponent@NUISystem@@AAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@0ABVCCharString@@0@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0058315B, 1399 bytes, `ret 14h` (`this` unused). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CManager::GetInstance 0x0041E5F2, CManager::CreateComponent 0x0041DB1D / CreateMenuEntryComponent 0x00428494,
//   CCountedPointer<CComponent>(T*) 0x00429C15 / Reset 0x00429C6F / dtor 0x004291DE, GetDescription(long) 0x005D95C0 (__fastcall),
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0, CWideString 0x0099B6B0 / ~ 0x0099B510, operator new 0x00BFEA1A, CAction 0x0042BE50,
//   CCountedPointer<CAction> 0x0042AA29 / ~ 0x00429160, param-list push_back 0x0042B646 (same-TU noinline stand-in),
//   CAction::AddParam 0x0053FDE0, CActionParamString 0x0054DAE0 (landed AddWantedItemToList 0x0059CB90 model). Slot names are descriptive.
// Layout: CAction 0x10; CActionParamLong 0x0C (vtable 0x012366F4, type 3); a component's CActionParam subobject at +0x18.
// Name fix (agentNF 2026-11-10): 0x004291DE is CCountedPointer<NUISystem::CComponent>::ReleaseData (the destructor is the 5-byte
//   `jmp` COMDAT 0x00429C53; Ego_r.pdb ??1 is 5 bytes): FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (engine/CCountedPointer.h).
// NF51 (round 7): ported onto the shared headers with the same-TU ConstructItemEntry 0x0058A0C7 (landed) and its console twin ConstructStyleEntry 0x00582ACE (this hand-back) as the models (were local
// CCharString / CWideString / NUISystem::CActionParam / CActionParamLong / CActionParamString / CAction / CComponent / CList / CManager /
// vector<pair<long,long> > models and a same-TU push_back stand-in body). The parameters are NUISystem::CActionParamSLONG (vtable
// 0x012366F4, ??_7CActionParamSLONG@NUISystem@@6B@; was a local CActionParamLong); GetDescription 0x005D95C0 is
// ?GetDescription@CTCInventoryItem@@SI?AVCWideString@@J@Z; the component slots are the header's: 3 FindChild, 60 AddChildDynamically, 67
// AddActionOnSelected, 70 AddActionOnInactive, 83 SetName, 92 MakeUnavailable, CList 145 AddNonScrollingChild / 152
// AddScrollingChildAndSetOffsets / 156 InitialiseOffsets, 124 AddPositionChild (was SetHeaderComponent), CMenuEntry 150 AddThingActionParam / 152 AddActionOnUnselected / 153
// AddActionOnVisible (were descriptive GetChild / AddChild / AddSelectedAction / AddAction / Clear / SetHeader / AddEntry / SetItemCount /
// SetUserValue / AddFocusAction / AddUnfocusAction). The function is Ego_r.pdb
// ?ConstructStyleEntryPC@CTCInventory@@QAEXPAVCComponent@NUISystem@@AAV?$vector@..@std@@0ABVCCharString@@0@Z (list by pointer, the styles by
// non-const reference; overlay opt-in FABLE_RETAIL_CTCINVENTORY_STYLEENTRY of engine/CTCInventory.h).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY_YOFF_TU   // NF364: Reset<CComponent> 0x00429C6F pick-any copy in this y-off / s-on TU
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_STYLEENTRY
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#define FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
#define FABLE_RETAIL_CMENUENTRY_UNSELECTED_VISIBLE
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
using namespace NUISystem;

void CTCInventory::ConstructStyleEntryPC(CComponent* list, std::vector<std::pair<long, long> >& styles, CComponent* owner,
                                         const CCharString& headerDefinition, CComponent* viewport)
{
    CManager* fe = CManager::GetInstance();
    if (styles.empty()) {
        list->MakeUnavailable();
        return;
    }
    CCountedPointer<CComponent> subList(fe->CreateComponent("PC_SUB_LIST", false));
    CList* sub = static_cast<CList*>(subList.Get());
    CCountedPointer<CComponent> header(fe->CreateComponent(headerDefinition, false));
    sub->AddNonScrollingChild(header);
    if (owner)
        owner->AddPositionChild(header.Get());
    sub->InitialiseOffsets(styles.size());
    for (std::pair<long, long>* it = styles.begin(), *last = styles.end(); it != last; ++it) {
        long style = it->first;
        long count = it->second;
        CCountedPointer<CComponent> entry;
        entry.Reset(fe->CreateMenuEntryComponent(CTCInventoryItem::GetDescription(style), 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        CMenuEntry* menuEntry = static_cast<CMenuEntry*>(entry.Get());
        CCountedPointer<CAction> addMesh(new CAction);
        addMesh->ActionType = UI_ACTION_ADD_MESH_CHILD;
        addMesh->UIActionParams.push_back(new CActionParamSLONG(style));
        addMesh->AddParam(entry.Get(), addMesh);
        CWideString model(L"PC_MODEL");
        addMesh->UIActionParams.push_back(new CActionParamString(model));
        addMesh->AddParam(owner, addMesh);
        entry->AddActionOnSelected(addMesh);
        CCountedPointer<CAction> removeMesh(new CAction);
        removeMesh->ActionType = UI_ACTION_REMOVE_MESH_CHILD;
        removeMesh->AddParam(entry.Get(), removeMesh);
        menuEntry->AddActionOnUnselected(removeMesh);
        CCountedPointer<CAction> loadDescription(new CAction);
        loadDescription->ActionType = UI_ACTION_LOAD_ITEM_DESC;
        loadDescription->UIActionParams.push_back(new CActionParamSLONG(style));
        loadDescription->AddParam(viewport->FindChild("PC_DESCRIPTION_VIEWPORT"), loadDescription);
        loadDescription->AddParam(entry.Get(), loadDescription);
        loadDescription->UIActionParams.push_back(new CActionParamSLONG(count));
        loadDescription->AddParam(owner, loadDescription);
        entry->AddActionOnSelected(loadDescription);
        CCountedPointer<CAction> unloadDescription(new CAction);
        unloadDescription->ActionType = UI_ACTION_UNLOAD_ITEM_DESC;
        unloadDescription->AddParam(entry.Get(), unloadDescription);
        menuEntry->AddActionOnUnselected(unloadDescription);
        CCountedPointer<CAction> loadName(new CAction);
        loadName->ActionType = UI_ACTION_LOAD_ITEM_MENU_ENTRY_NAME;
        loadName->UIActionParams.push_back(new CActionParamSLONG(style));
        loadName->AddParam(entry.Get(), loadName);
        loadName->UIActionParams.push_back(new CActionParamSLONG(count));
        menuEntry->AddActionOnVisible(loadName);
        sub->AddScrollingChildAndSetOffsets(entry);
    }
    list->AddChildDynamically(subList);
    list->SetName("STYLE_CARDS_LIST");
    CCountedPointer<CAction> action(new CAction);
    action->ActionType = UI_ACTION_TYPE_DELETE;
    action->UIActionParams.push_back(list);
    list->AddActionOnInactive(action);
}
