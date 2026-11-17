// CTCInventoryClothing::ConstructClothingList(const std::vector<std::pair<long,long> >& items, NUISystem::CViewport* viewport, EClothingSuitPart part, bool plainEntries) -- console clothing list for one suit part: build the clothing-actions list with its back / select prompts and "CLOTHING TITLE" header, then (for a non-empty item list) a sub-list with the same prompts and title and one menu entry per item wired with select (viewport, entry), focus, unfocus, equip and remove actions, each entry either cleared (plain) or given the actions list, and registered with AddClothingMenuEntry; returns the sub-list (null when there are no items)
// Leaf: ?ConstructClothingList@CTCInventoryClothing@@AAEPAVCComponent@NUISystem@@AAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCViewport@3@W4EClothingSuitPart@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B70B8, 1561 bytes, `ret 10h`. Biased frame (lea ebp,[esp-0x68]): optimize("s",on).
// Callees: as ConstructClothingListPC 0x005B6A66 (this chunk) minus OpenInventory and the frame registrations.
// Shape: the sub-list pointer starts null (edi doubles as the `false` argument until the sub-list replaces it) and the
//   sub-list prompts / title live in the non-empty block, so the empty path skips their destructors.
// Layout: engine/CTCInventoryClothing.h is the Ego_r.pdb layout and no data member is read here, so `this` is a method-only
//   local model; CAction 0x10 (thing-param list<long> at +0x08), CActionParamLong 0x0C (vtable 0x012366F4, type 3).
// Name fix (agentNF 2026-11-10): 0x004291DE is CCountedPointer<NUISystem::CComponent>::ReleaseData (the destructor is the 5-byte
//   `jmp` COMDAT 0x00429C53; Ego_r.pdb ??1 is 5 bytes; likewise 0x00429160 = ReleaseData<CAction>, dtor jmp 0x00429B72). The local model's destructor is `{ ReleaseData(); }`
//   (engine/CCountedPointer.h FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY shape).
// NF51 (round 7): ported onto the shared headers with the same-TU twin ConstructClothingListPC 0x005B6A66 (landed) as the model (were
// local CCharString / CWideString / CCountedPointer / NUISystem::CActionParam / CActionParamLong / CAction / CComponent / CList /
// CManager / std::vector models and a same-TU push_back stand-in body). The parameters are NUISystem::CActionParamSLONG (vtable 0x012366F4,
// ??_7CActionParamSLONG@NUISystem@@6B@; was a local CActionParamLong), pushed straight onto UIActionParams (list push_back 0x0042B646);
// the component slots are the header's: 145 CList::AddNonScrollingChild, 144 CText::SetValue, 156 CList::InitialiseOffsets, 152
// CList::AddScrollingChildAndSetOffsets, 67 / 68 AddActionOnSelected / AddActionOnBack, 59 AddChild, 92 MakeUnavailable, CMenuEntry 150
// AddThingActionParam / 152 AddActionOnUnselected / 153 AddActionOnVisible (were descriptive SetHeader / SetText / SetItemCount /
// AddEntry / AddSpecialAction / AddChildAtEnd / Clear / SetUserValue / AddFocusAction / AddUnfocusAction). The function itself is the
// private ?ConstructClothingList@CTCInventoryClothing@@AAE..AAV?$vector..@Z (Ego_r.pdb; engine/CTCInventoryClothing.h _LISTS).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_CALCULATE
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_SCREEN
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_LISTS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
#define FABLE_RETAIL_CMENUENTRY_UNSELECTED_VISIBLE
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#include "engine/CTCInventoryClothing.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamSLONG.h"

NUISystem::CComponent* CTCInventoryClothing::ConstructClothingList(std::vector<std::pair<long, long> >& items,
    NUISystem::CViewport* viewport, EClothingSuitPart part, bool plainEntries)
{
    NUISystem::CManager* fe = NUISystem::CManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> actions(fe->CreateComponent("UI_LIST_CLOTHING_ACTIONS", false));
    NUISystem::CList* actionsList = static_cast<NUISystem::CList*>(actions.Get());
    CCountedPointer<NUISystem::CComponent> back(fe->CreateComponent("UI_BACK_FOR_LIST", false));
    CCountedPointer<NUISystem::CComponent> select(fe->CreateComponent("UI_SELECT_FOR_LIST", false));
    actionsList->AddNonScrollingChild(back);
    actionsList->AddNonScrollingChild(select);
    CCountedPointer<NUISystem::CComponent> title(fe->CreateComponent("UI_TITLE_NEW", false));
    CCountedPointer<NUISystem::CText> titleText;
    title->SetName("CLOTHING TITLE");
    titleText.SetStaticCast(title);
    CWideString titleString;
    ConstructTitleString(titleString);
    titleText->SetValue(titleString, true, true);
    actionsList->AddNonScrollingChild(title);
    NUISystem::CList* subList = 0;
    if (!items.empty()) {
        subList = static_cast<NUISystem::CList*>(fe->CreateComponent("UI_SUB_LIST", false));
        CCountedPointer<NUISystem::CComponent> subBack(fe->CreateComponent("UI_BACK_FOR_LIST", false));
        CCountedPointer<NUISystem::CComponent> subSelect(fe->CreateComponent("UI_SELECT_FOR_LIST", false));
        subList->AddNonScrollingChild(subBack);
        subList->AddNonScrollingChild(subSelect);
        CCountedPointer<NUISystem::CComponent> subTitle(fe->CreateComponent("UI_TITLE_NEW", false));
        subTitle->SetName("CLOTHING TITLE");
        titleText.SetStaticCast(subTitle);
        titleText->SetValue(titleString, true, true);
        subList->AddNonScrollingChild(subTitle);
        subList->InitialiseOffsets(items.size());
        for (unsigned long i = 0; i < items.size(); ++i) {
            long defIndex = items[i].first;
            CCountedPointer<NUISystem::CComponent> entry(fe->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f,
                "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW", 0.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
            NUISystem::CMenuEntry* menuEntry = static_cast<NUISystem::CMenuEntry*>(entry.Get());
            CCountedPointer<NUISystem::CAction> loadDesc(new NUISystem::CAction);
            loadDesc->ActionType = NUISystem::UI_ACTION_LOAD_CLOTHING_DESC;
            loadDesc->UIActionParams.push_back(new NUISystem::CActionParamSLONG(defIndex));
            loadDesc->AddParam(viewport, loadDesc);
            loadDesc->AddParam(entry.Get(), loadDesc);
            entry->AddActionOnSelected(loadDesc);
            CCountedPointer<NUISystem::CAction> unloadDesc(new NUISystem::CAction);
            unloadDesc->ActionType = NUISystem::UI_ACTION_UNLOAD_CLOTHING_DESC;
            unloadDesc->AddParam(entry.Get(), unloadDesc);
            menuEntry->AddActionOnUnselected(unloadDesc);
            CCountedPointer<NUISystem::CAction> loadName(new NUISystem::CAction);
            loadName->ActionType = NUISystem::UI_ACTION_LOAD_CLOTHING_MENU_ENTRY_NAME;
            loadName->UIActionParams.push_back(new NUISystem::CActionParamSLONG(defIndex));
            loadName->AddParam(entry.Get(), loadName);
            menuEntry->AddActionOnVisible(loadName);
            menuEntry->AddThingActionParam(defIndex);
            CCountedPointer<NUISystem::CAction> wear(new NUISystem::CAction);
            wear->ActionType = NUISystem::UI_ACTION_TYPE_UPDATE_MANNEQUIN_CLOTHING;
            wear->ThingActionParams.push_back(defIndex);
            entry->AddActionOnSelected(wear);
            CCountedPointer<NUISystem::CAction> reset(new NUISystem::CAction);
            reset->ActionType = NUISystem::UI_ACTION_TYPE_RESET_CLOTHING_MANNEQUIN;
            entry->AddActionOnBack(reset);
            subList->AddScrollingChildAndSetOffsets(entry);
            if (plainEntries)
                entry->MakeUnavailable();
            else
                entry->AddChild(actions);
            AddClothingMenuEntry(part, entry.Get());
        }
    }
    return subList;
}
