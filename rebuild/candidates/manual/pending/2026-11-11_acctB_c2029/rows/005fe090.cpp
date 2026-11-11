// CTCInventoryMap::ConstructQuestsScreen(NUISystem::CComponent* parent) -- console quest screen: the UI_QUESTS_MENU list with the scrolling description viewport and two menu entries (current quests, past quests), each with its sub-list (ConstructQuestReviewList / ConstructPastQuestsList) or made unavailable, its guide text, subtitle and ring picture; returns the menu
// Leaf: ?ConstructQuestsScreen@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FE090, 1937 bytes (`ret 4` at 0x005FE81E, int3 padding after), thiscall, esp frame aligned
//   to 8 (`push ebp ; mov ebp,esp ; and esp,-8 ; sub esp,6Ch`), no EH frame; speed TU (the CTCInventoryMap TU, as the landed
//   ConstructPastQuestsList 0x005FCBE0 / ConstructQuestReviewList 0x005FC2D0 it calls).
// Identity: Ego_r.pdb (donor 0x006522D0) + FableWin.pdb (0x02083ED0) ?ConstructQuestsScreen@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@@Z
//   (public, one CComponent* argument, returns CComponent*: `ret 4`, `mov eax,esi` = the menu); reloc_bindings.tsv binds that symbol at
//   0x005FE090 from the landed NUISystem::CManager::Action 0x0041E6D3 (its only caller, 0x004203E7). Its literals are the quest screen's
//   (UI_QUESTS_MENU, TEXT_GUI_MENU_PAST_QUESTS / _CURRENT_QUESTS, TEXT_GUI_MENUGUIDE_QUESTS_*, UI_*QUESTS_SUBTITLE, UI_RING_PIC_*_QUESTS)
//   and it calls the two quest-list builders; the PC twin ConstructQuestsScreenPC is 0x00601310.
// Callees (rel.py: every call target equals retail's): NUISystem::CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D /
//   CreateMenuEntryComponent 0x00428494, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, NGameText::CDataBank::GetTextBySymbol
//   0x009C95E0, ~CWideString 0x0099B510, ConstructPastQuestsList 0x005FCBE0, ConstructQuestReviewList 0x005FC2D0, operator new 0x00BFEA1A /
//   delete 0x00BFE9BC, CCountedPointer<CComponent>::DeleteData 0x00428B10 (stored) / Reset 0x00429C6F (called; the (T*) ctor and the dtor
//   are expanded); virtuals CList InitialiseOffsets (+0x270) / AddNonScrollingChild (+0x244) / AddScrollingChildAndSetOffsets (+0x260),
//   CComponent AddChild (slot 59) / MakeUnavailable (slot 92), CText SetValue(const CWideString&, bool, bool) (slot 144), CViewport
//   AddFakeChild (slot 142); the text bank is CMainGameComponent::PMainGameComponent 0x013B86A0 +0x14 (inline GetTextBank).
// Shape: locals constructed in retail's order (read off the destructor order at the end: the review list, the current entry, the
//   current title, ring picture, subtitle, description, text area, past list, past entry, past title). The current-quests title re-reads
//   PMainGameComponent (0x005FE200) while the guide texts use the `textBank` reference homed at [esp+24h]. Each entry is an if/else of
//   two complete Reset(CreateMenuEntryComponent(...)) statements (BRANCH template + AddChild of the list, or the plain template +
//   MakeUnavailable); the past entry uses the second-row layout (25, 2/3, 5) the landed CTCInventoryWeapons::ConstructScreen uses.
// Levers (2 grades): FABLE_RETAIL_CCOUNTEDPOINTER (the (T*) ctor tests the stored member, as 0x00626E40): c2 then sees the later
//   `!currentQuestsList.IsNull()` read the same value and threads the ctor's null path straight into the else arm (0x005FE23C
//   `je 0x005FE2E5`, the failed-new path keeps its own `Info = 0` block at 0x005FE2DF); with the default `if (data)` both paths merge and
//   the common arm prologue is hoisted above the test. `description` declared before the `textBank` reference gives retail's
//   `mov ecx,[PMainGameComponent] ; <description zeroed> ; mov edx,[ecx+14h]` (0x005FE1CB..0x005FE1D9).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CTCINVENTORYMAP_PASTQUESTS
#define FABLE_RETAIL_CTCINVENTORYMAP_QUESTREVIEW
#define FABLE_RETAIL_CTCINVENTORYMAP_QUESTSSCREEN
#include "stlport/fable_containers.h"
#include "engine/CTCInventoryMap.h"
#include "engine/CCountedPointer.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CViewport.h"
using namespace NUISystem;

CComponent* CTCInventoryMap::ConstructQuestsScreen(CComponent* parent)
{
    CManager* manager = CManager::GetInstance();
    CList* menu = static_cast<CList*>(manager->CreateComponent("UI_QUESTS_MENU", false));
    CWideString pastQuestsText = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_PAST_QUESTS");
    CCountedPointer<CComponent> pastQuestsEntry;
    CCountedPointer<CComponent> pastQuestsList(ConstructPastQuestsList(parent));
    menu->InitialiseOffsets(2);
    CCountedPointer<CComponent> textArea(manager->CreateComponent("UI_SCROLLING_VIEWPORT_TEXT_AREA", false));
    CViewport* viewport = static_cast<CViewport*>(textArea.Get());
    menu->AddNonScrollingChild(textArea);

    CCountedPointer<CComponent> description;
    const NGameText::CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    CCountedPointer<CComponent> subtitle;
    CCountedPointer<CComponent> ringPicture;

    // current quests
    CWideString currentQuestsText = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_CURRENT_QUESTS");
    CCountedPointer<CComponent> currentQuestsEntry;
    CCountedPointer<CComponent> currentQuestsList(ConstructQuestReviewList(parent));
    if (!currentQuestsList.IsNull()) {
        currentQuestsEntry.Reset(manager->CreateMenuEntryComponent(currentQuestsText, 0.0f, 1.0f,
            "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW", 0.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
        currentQuestsEntry->AddChild(currentQuestsList);
    } else {
        currentQuestsEntry.Reset(manager->CreateMenuEntryComponent(currentQuestsText, 0.0f, 1.0f,
            "UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW", 0.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
        currentQuestsEntry->MakeUnavailable();
    }
    description.Reset(manager->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF", false));
    CText* descriptionText = static_cast<CText*>(description.Get());
    descriptionText->SetValue(textBank.GetTextBySymbol("TEXT_GUI_MENUGUIDE_QUESTS_CURRENTQUESTS"), true, true);
    viewport->AddFakeChild(descriptionText);
    currentQuestsEntry->AddChild(description);
    subtitle.Reset(manager->CreateComponent("UI_CURRENTQUESTS_SUBTITLE", false));
    currentQuestsEntry->AddChild(subtitle);
    ringPicture.Reset(manager->CreateComponent("UI_RING_PIC_CURRENT_QUESTS", false));
    currentQuestsEntry->AddChild(ringPicture);

    // past quests
    if (!pastQuestsList.IsNull()) {
        pastQuestsEntry.Reset(manager->CreateMenuEntryComponent(pastQuestsText, 25.0f, 0.6666667f,
            "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW", 5.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
        pastQuestsEntry->AddChild(pastQuestsList);
    } else {
        pastQuestsEntry.Reset(manager->CreateMenuEntryComponent(pastQuestsText, 25.0f, 0.6666667f,
            "UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW", 5.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
        pastQuestsEntry->MakeUnavailable();
    }
    description.Reset(manager->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF", false));
    descriptionText = static_cast<CText*>(description.Get());
    descriptionText->SetValue(textBank.GetTextBySymbol("TEXT_GUI_MENUGUIDE_QUESTS_PASTQUESTS"), true, true);
    viewport->AddFakeChild(descriptionText);
    pastQuestsEntry->AddChild(description);
    subtitle.Reset(manager->CreateComponent("UI_PASTQUESTS_SUBTITLE", false));
    pastQuestsEntry->AddChild(subtitle);
    ringPicture.Reset(manager->CreateComponent("UI_RING_PIC_PAST_QUESTS", false));
    pastQuestsEntry->AddChild(ringPicture);

    menu->AddScrollingChildAndSetOffsets(currentQuestsEntry);
    menu->AddScrollingChildAndSetOffsets(pastQuestsEntry);
    return menu;
}
