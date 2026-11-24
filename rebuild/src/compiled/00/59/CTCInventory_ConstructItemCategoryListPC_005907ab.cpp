// CTCInventory::ConstructItemCategoryListPC(CWideString& title, NUISystem::CList* list, std::vector<std::pair<long,long> >& items, NUISystem::CComponent* owner, const CCharString& templateName, const CCharString& descriptionSymbol, bool checkUsable) -- PC inventory category: a branch entry captioned "title x <total count>" with a subtitle, the category description (shown in the owner's viewport) and an optional template component, all registered with the owner's frame; an empty category clears the entry, otherwise the items are merged per def into a map, a sub-list titled "<title>: <name>" gets the merged count, and an items-actions list (the "can't use" variant, with its not-usable entry cleared and the icon action reset, when the hero's carried thing is not alive) gets the same title before ContructItemListsPC fills them
// Leaf: ?ConstructItemCategoryListPC@CTCInventory@@QAEXAAVCWideString@@PAVCList@NUISystem@@AAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCComponent@4@ABVCCharString@@4_N@Z
// Inline-budget: __declspec(noinline) through the header opt-ins FABLE_RETAIL_STLP_MAP_CTOR_NOINLINE (map<long,long>() COMDAT 0x004DF86C),
//   FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE (tree destructor COMDAT 0x004DD2B1) and FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE (_M_find 0x00592F92)
//   -- retail calls all three out of line here (bodies visible, as STLport puts them in every TU); recheck_final.log RELOCATION_MATCH PASS.
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) @ 0x005907AB, 1415 bytes, `ret 1Ch`. Frame-pointer size TU: optimize("y",off) + ("s",on).
//   FableWin.pdb ?ConstructItemCategoryListPC@CTCInventory@@QAEXAAVCWideString@@PAVCList@NUISystem@@AAV?$vector@...@ABVCCharString@@4_N@Z
//   (the catalog label "CTCInventoryAbilities::ConstructPCExpressionCategoryList" is propagated).
// Callees: CManager::GetInstance 0x0041E5F2, CManager::CreateComponent 0x0041DB1D / CreateMenuEntryComponent 0x00428494,
//   CCountedPointer<CComponent>(T*) 0x00429C15 / Reset 0x00429C6F / dtor 0x004291DE, CTCInventory::CreateMenuDescPC 0x00582673
//   (CCountedPointer by hidden pointer), GetXString 0x005CC772, GFIntToWideString 0x00405F00, CWideString copy 0x0099B720 /
//   += 0x0099B8D0 / += (const wchar*) 0x0099B940 / ~ 0x0099B510, CCharString 0x0099EBF0 / ~ 0x0099EAE0 / != 0x0099E960,
//   map<long,long> ctor 0x004DF86C / find 0x00592F92 / insert 0x004E539F / dtor 0x004DD2B1, std::bitset::test 0x00410DE0 +
//   const CVectorMap::LowerBound 0x004365B0 (the parent thing's interface 8), IS_THING_ALIVE 0x0040DCE0 (`__fastcall`,
//   CBaseObject* const&), NUISystem::CAction() 0x0042BED4 / copy 0x0042CD84 / param-list dtor 0x0042ABCA,
//   CTCInventory::ContructItemListsPC 0x00590198. The global name 0x013B8B4C is CCharString("ITEMS TITLE").
// Shape: the CTCInventoryAbilities::ConstructPCExpressionCategoryList 0x005F45B5 family (this chunk); the total count is a
//   plain index loop VC strength-reduces; the merge loop index is signed against the pointer difference (`jl`); the
//   actions-list name is a ternary on a `usable` flag (VC threads the flag test, laying the normal-name arm out of line)
//   built as a named CCharString; the merged entry goes in through `insert(make_pair(key, count))` (the count is loaded
//   into a register before the key store; a direct pair constructor stores it at once).
// Name fix (agentNF 2026-11-10): 0x004291DE is CCountedPointer<NUISystem::CComponent>::ReleaseData (the destructor is the 5-byte
//   `jmp` COMDAT 0x00429C53; Ego_r.pdb ??1 is 5 bytes): FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (engine/CCountedPointer.h).
// NF80 (q10943): moved onto the shared headers, the 0x005F45B5 recipe: engine/CTCInventory.h (retail branch, PCMENUS + the overlay's
//   ITEMCATEGORYLIST: ContructItemListsPC / private CreateMenuDescPC), engine/NUISystem_CManager / _CList / _CText / _CViewport /
//   _CDeletion.h, engine/CThing.h (PeekTC expanded: the const LowerBound 0x004365B0), engine/CTCTargetingBase.h and engine/ThingAlive.h. The old placeholder vtable's
//   names are the headers' slots: 59 AddChild, 62 SetDeletion(CDeletion) (the local "CAction" record is NUISystem::CDeletion: Method,
//   then the AssociatedParents list; FableWin.pdb ??0CDeletion@NUISystem@@QAE@ABV01@@Z = 0x0042CD84), 92 MakeUnavailable, 124
//   AddPositionChild, CText 142 GetValue / 144 SetValue, CList AddNonScrollingChild / AddScrollingChildAndSetOffsets /
//   InitialiseOffsets / SetDefaultChild, CViewport 142 AddFakeChild, and the parent thing's interface 8 is CTCTargetingBase (TCI_TARGETING;
//   slot 33 GetPTargetedThing).
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY_YOFF_TU   // NF364: Reset<CComponent> 0x00429C6F pick-any copy in this y-off / s-on TU
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORY_PCMENUS
#define FABLE_RETAIL_CTCINVENTORY_ITEMCATEGORYLIST
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_CTOR_NOINLINE
#define FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CCountedPointer.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
#include "engine/CTCTargetingBase.h"
#include "engine/ThingAlive.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/CTCInventoryStats.h"
CWideString FABLE_FASTCALL GFIntToWideString(long value);     // 0x00405F00

namespace { extern CCharString TITLE_NAME; }                  // 0x013B8B4C ("ITEMS TITLE")

void CTCInventory::ConstructItemCategoryListPC(CWideString& title, NUISystem::CList* list, std::vector<std::pair<long, long> >& items,
                                               NUISystem::CComponent* owner, const CCharString& templateName,
                                               const CCharString& descriptionSymbol, bool checkUsable)
{
    NUISystem::CManager* fe = NUISystem::CManager::GetInstance();
    NUISystem::CViewport* viewport = static_cast<NUISystem::CViewport*>(owner->FindChild("PC_DESCRIPTION_VIEWPORT"));
    NUISystem::CComponent* frame = owner->FindChild("PC_UI_FRAME");
    CCountedPointer<NUISystem::CComponent> subtitle;
    CWideString caption(title);
    if (!items.empty()) {
        caption += CTCInventoryStats::GetXString();
        long total = 0;
        for (unsigned long i = 0; i < items.size(); ++i)
            total += items[i].second;
        caption += GFIntToWideString(total);
    }
    subtitle.Reset(fe->CreateComponent("PC_SUBTITLE", false));
    static_cast<NUISystem::CText*>(subtitle.Get())->SetValue(caption, true, true);
    CCountedPointer<NUISystem::CComponent> entry(fe->CreateMenuEntryComponent(caption, 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE",
                                                                              0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    entry->AddChild(subtitle);
    if (frame)
        frame->AddPositionChild(subtitle.Get());
    list->AddScrollingChildAndSetOffsets(entry);
    CCountedPointer<NUISystem::CComponent> description = CreateMenuDescPC(descriptionSymbol);
    entry->AddChild(description);
    if (frame)
        frame->AddPositionChild(description.Get());
    viewport->AddFakeChild(description.Get());
    if (templateName != "") {
        CCountedPointer<NUISystem::CComponent> extra(fe->CreateComponent(templateName, false));
        entry->AddChild(extra);
        if (frame)
            frame->AddPositionChild(extra.Get());
    }
    if (items.empty()) {
        entry->MakeUnavailable();
    } else {
        std::map<long, long> merged;
        for (long i = 0; i < items.end() - items.begin(); ++i) {
            std::pair<long, long>* item = &items[i];
            long key = item->first;
            std::map<long, long>::iterator found = merged.find(key);
            if (found == merged.end())
                merged.insert(std::make_pair(key, item->second));
            else
                (*found).second += item->second;
        }
        CCountedPointer<NUISystem::CComponent> subList(fe->CreateComponent("PC_SUB_LIST", false));
        NUISystem::CList* sub = static_cast<NUISystem::CList*>(subList.Get());
        entry->AddChild(subList);
        sub->InitialiseOffsets(merged.size());
        CCountedPointer<NUISystem::CComponent> titleComp(fe->CreateComponent("PC_ITEM_TITLE", false));
        NUISystem::CText* titleObject = static_cast<NUISystem::CText*>(titleComp.Get());
        CWideString titleText(titleObject->GetValue());
        titleText += L":";
        titleText += title;
        titleObject->SetValue(titleText, true, true);
        titleComp->SetName(TITLE_NAME);
        sub->AddNonScrollingChild(titleComp);
        if (frame)
            frame->AddPositionChild(titleComp.Get());
        bool usable = true;
        const CTCTargetingBase* targeting;
        if (checkUsable && ParentThing->PeekTC(TCI_TARGETING, &targeting) && !IS_THING_ALIVE(targeting->GetPTargetedThing()))
            usable = false;
        CCharString actionsListName(usable ? "PC_UI_LIST_ITEMS_ACTIONS" : "PC_UI_LIST_ITEMS_ACTIONS_CANT_USE");
        CCountedPointer<NUISystem::CComponent> actions(NUISystem::CManager::GetInstance()->CreateComponent(actionsListName, false));
        NUISystem::CList* actionsList = static_cast<NUISystem::CList*>(actions.Get());
        actionsList->SetDefaultChild((long)0);
        if (!usable) {
            NUISystem::CComponent* notUsable = actionsList->FindChild("PC_UI_MENU_ENTRY_ITEM_ACTION_NOT_USE");
            notUsable->MakeUnavailable();
            NUISystem::CComponent* icon = notUsable->FindChild("UI_ACTION_ICON");
            if (icon) {
                NUISystem::CDeletion deletion;
                deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                icon->SetDeletion(deletion);
            }
        }
        CCountedPointer<NUISystem::CComponent> actionsTitle(fe->CreateComponent("PC_ITEM_TITLE", false));
        static_cast<NUISystem::CText*>(actionsTitle.Get())->SetValue(titleText, true, true);
        actionsTitle->SetName(TITLE_NAME);
        actionsList->AddNonScrollingChild(actionsTitle);
        if (frame)
            frame->AddPositionChild(actionsTitle.Get());
        ContructItemListsPC(merged, sub, owner, actions);
    }
}
