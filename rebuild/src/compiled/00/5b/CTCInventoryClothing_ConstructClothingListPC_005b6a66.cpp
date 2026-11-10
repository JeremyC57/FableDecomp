// CTCInventoryClothing::ConstructClothingListPC(std::vector<std::pair<long,long> >& items, NUISystem::CViewport* viewport, NUISystem::CComponent* frame, EClothingSuitPart part, bool worn) -- PC clothing list for one suit part: opens the base inventory, builds the PC clothing-actions list with its "CLOTHING TITLE" header (placed in the frame), and for a non-empty item list a PC_SUB_LIST with the same title and one menu entry per item (plain entry template when the hero wears a non-removable item on the part, else the branch template) wired with load-description (viewport, entry, frame) on select, unload-description on unselect, load-entry-name (entry, frame) on visible, the item as thing parameter, update-mannequin on select and reset-mannequin on back; the entry is made unavailable (worn) or given the actions list, then registered with AddClothingMenuEntry; returns the sub-list (null for no items)
// Leaf: ?ConstructClothingListPC@CTCInventoryClothing@@AAEPAVCComponent@NUISystem@@AAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCViewport@3@PAV23@W4EClothingSuitPart@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B6A66, 1,507 bytes, thiscall `ret 14h`, no EH frame, `mov ebp,esp ; sub esp,7Ch` (size TU,
//   optimize("s") as the TU's matched ConstructScreenPC 0x005B8743 / ConstructClothingList 0x005B70B8). Identity: Ego_r.pdb donor 0x00562A81 /
//   FableWin.pdb 0x01E2DC3D ?ConstructClothingListPC@CTCInventoryClothing@@AAE.. (private); sole caller ConstructScreenPC 0x005B8743 (twice,
//   0x005B94D5 / 0x005B94D9). The console twin is ConstructClothingList 0x005B70B8.
// Callees (all PDB-spelled): CTCInventoryBase::OpenInventory 0x005BD4EA (qualified base call), CManager::GetInstance 0x0041E5F2 /
//   CreateComponent 0x0041DB1D / CreateMenuEntryComponent 0x00428494, CCountedPointer<CComponent> (T*) 0x00429C15 / Reset 0x00429C6F /
//   ReleaseData 0x004291DE, CCountedPointer<CText>::ShareData 0x00429460 / ReleaseData 0x0042925C, CCountedPointer<CAction> (T*) 0x0042AA29 /
//   ReleaseData 0x00429160, CAction 0x0042BE50 / AddParam 0x0053FDE0, list<CActionParam*>::push_back 0x0042B646, list<long>::push_back
//   0x0042B687, ConstructTitleString 0x005B6881, AddClothingMenuEntry 0x005B4771; virtuals CText::SetValue (slot 144), CList
//   AddNonScrollingChild / AddScrollingChildAndSetOffsets / InitialiseOffsets, CComponent SetName / AddPositionChild / AddActionOnSelected /
//   AddActionOnBack / AddChild / MakeUnavailable, CMenuEntry AddThingActionParam (150) / AddActionOnUnselected (152) / AddActionOnVisible (153).
// Shape: the empty item list returns before the sub-list block (retail `xor esi,esi ; jmp` to the shared tail 0x005B6B6D, the sub-list kept in
//   esi); a null-initialised sub-list pointer returned at the end instead homes it in [ebp-0Ch] (DIFFER 1506v1507). The menu entry is created
//   in both arms of the worn test (two template literals, two temporaries sets: 0x005B6C88 / 0x005B6D02).
// Header: engine/CTCInventoryClothing.h FABLE_RETAIL_CTCINVENTORYCLOTHING_LISTS (AddClothingMenuEntry), engine/NUISystem_CMenuEntry.h
//   FABLE_RETAIL_CMENUENTRY_UNSELECTED_VISIBLE (slots 152 / 153); graded with FABLE_HDR_OVERLAY=$A/hdr.
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

NUISystem::CComponent* CTCInventoryClothing::ConstructClothingListPC(std::vector<std::pair<long, long> >& items,
    NUISystem::CViewport* viewport, NUISystem::CComponent* frame, EClothingSuitPart part, bool worn)
{
    NInventory::CTCInventoryBase::OpenInventory();
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> actions(manager->CreateComponent("PC_UI_LIST_CLOTHING_ACTIONS", false));
    NUISystem::CList* actionsList = static_cast<NUISystem::CList*>(actions.Get());
    CCountedPointer<NUISystem::CComponent> title(manager->CreateComponent("PC_TITLE", false));
    CCountedPointer<NUISystem::CText> titleText;
    title->SetName("CLOTHING TITLE");
    titleText.SetStaticCast(title);
    CWideString titleString;
    ConstructTitleString(titleString);
    titleText->SetValue(titleString, true, true);
    actionsList->AddNonScrollingChild(title);
    if (frame)
        frame->AddPositionChild(title.Get());
    if (items.empty())
        return 0;
    {
        NUISystem::CList* list = static_cast<NUISystem::CList*>(manager->CreateComponent("PC_SUB_LIST", false));
        CCountedPointer<NUISystem::CComponent> listTitle(manager->CreateComponent("PC_TITLE", false));
        listTitle->SetName("CLOTHING TITLE");
        titleText.SetStaticCast(listTitle);
        titleText->SetValue(titleString, true, true);
        list->AddNonScrollingChild(listTitle);
        if (frame)
            frame->AddPositionChild(listTitle.Get());
        list->InitialiseOffsets(items.size());
        for (unsigned long i = 0; i < items.size(); ++i) {
            long defIndex = items[i].first;
            CCountedPointer<NUISystem::CComponent> entry;
            if (worn)
                entry.Reset(manager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE", 0.0f,
                                                              CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
            else
                entry.Reset(manager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f,
                                                              CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
            NUISystem::CMenuEntry* menuEntry = static_cast<NUISystem::CMenuEntry*>(entry.Get());
            // selecting the entry loads its description into the viewport
            CCountedPointer<NUISystem::CAction> loadDesc(new NUISystem::CAction);
            loadDesc->ActionType = NUISystem::UI_ACTION_LOAD_CLOTHING_DESC;
            loadDesc->UIActionParams.push_back(new NUISystem::CActionParamSLONG(defIndex));
            loadDesc->AddParam(viewport, loadDesc);
            loadDesc->AddParam(entry.Get(), loadDesc);
            loadDesc->AddParam(frame, loadDesc);
            entry->AddActionOnSelected(loadDesc);
            CCountedPointer<NUISystem::CAction> unloadDesc(new NUISystem::CAction);
            unloadDesc->ActionType = NUISystem::UI_ACTION_UNLOAD_CLOTHING_DESC;
            unloadDesc->AddParam(entry.Get(), unloadDesc);
            menuEntry->AddActionOnUnselected(unloadDesc);
            CCountedPointer<NUISystem::CAction> loadName(new NUISystem::CAction);
            loadName->ActionType = NUISystem::UI_ACTION_LOAD_CLOTHING_MENU_ENTRY_NAME;
            loadName->UIActionParams.push_back(new NUISystem::CActionParamSLONG(defIndex));
            loadName->AddParam(entry.Get(), loadName);
            loadName->AddParam(frame, loadName);
            menuEntry->AddActionOnVisible(loadName);
            menuEntry->AddThingActionParam(defIndex);
            CCountedPointer<NUISystem::CAction> wear(new NUISystem::CAction);
            wear->ActionType = NUISystem::UI_ACTION_TYPE_UPDATE_MANNEQUIN_CLOTHING;
            wear->ThingActionParams.push_back(defIndex);
            entry->AddActionOnSelected(wear);
            CCountedPointer<NUISystem::CAction> reset(new NUISystem::CAction);
            reset->ActionType = NUISystem::UI_ACTION_TYPE_RESET_CLOTHING_MANNEQUIN;
            entry->AddActionOnBack(reset);
            list->AddScrollingChildAndSetOffsets(entry);
            if (worn)
                entry->MakeUnavailable();
            else
                entry->AddChild(actions);
            AddClothingMenuEntry(part, entry.Get());
        }
        return list;
    }
}