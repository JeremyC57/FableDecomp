// CTCInventoryMap::ConstructQuestsScreenPC(NUISystem::CComponent* screen) -- builds the PC (mouse) quests screen: the PC_QUESTS_MENU list, mouse-movement observe / ignore actions for the screen's and the quest frame's scroll buttons, the current-quests and past-quests menu entries (a branch entry holding the review / past list, or an unavailable plain entry) with their description text, subtitle and ring picture, all added to the menu as scrolling children; returns the menu
// Evidence (agentC31; twin = landed 0x005FE090 CTCInventoryMap::ConstructQuestsScreen):
//   - named `CText* descriptionText` taken after `frame->AddPositionChild(description.Get())`: retail 0x00602972 passes the reloaded
//     pointer in eax, then keeps it in edi from 0x0060297F for SetValue / AddFakeChild (the twin names it the same way);
//   - FABLE_RETAIL_CCOUNTEDPOINTER (the (T*) constructor tests the member it stored, as the twin's TU text does): VC7.1 threads
//     `if (!curList.IsNull())` through the constructor's null arm (retail 0x006027F4 `je 0x006028D0`, 0x00602806 `je 0x006028C7`);
//   - STLport 4.6 inline layers (same code, they only count in the caller size VC7.1's inliner scores later sites with; retail calls
//     the LAST end-of-scope ~CCountedPointer<CComponent> 0x00429C53 at 0x00602E09 and expands the six before it):
//     FABLE_RETAIL_STLP_MALLOC_ALLOC_ALLOCATE (allocator<T>::allocate through __sgi_alloc::allocate), FABLE_RETAIL_STLP_LIST_CONSTRUCT_HELPER
//     (_M_create_node through _Construct), FABLE_RETAIL_STLP_LIST_BASE_CONVERT_ALLOCATOR (_STLP_CONVERT_ALLOCATOR(__a, _Node) == __a).
//   - Inferred: the sixteen `new CActionParamULONG` results are named before push_back. No retail byte distinguishes the spelling;
//     with the three layers above the last destructor's caller size is 9341 and retail's decision needs [9347, 9403) (measured,
//     NOTES.md); the named locals give 9357. FableWin's /Od body is +151 instructions over the text without them, +119 with them.
// Leaf: ?ConstructQuestsScreenPC@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00601310, 6931 bytes, thiscall `ret 4`, esp frame (sub esp,78h), speed TU.
// Callees: NUISystem::CManager::GetInstance 0x0041E5F2, CManager::CreateComponent 0x0041DB1D, CManager::CreateMenuEntryComponent
//   0x00428494, CCountedPointer<CComponent>::Reset 0x00429C6F, ~CCountedPointer<CComponent> 0x00429C53, CAction::AddParam 0x0053FDE0,
//   ConstructPastQuestsListPC 0x005FDDD0, ConstructQuestReviewListPC 0x005FC8A0, NGameText::CDataBank::GetTextBySymbol 0x009C95E0,
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, ~CWideString 0x0099B510; CComponent / CList / CText /
//   CViewport virtuals FindChild 3, AddChild 59, AddActionOnSelected 67, AddActionOnInactive 70, SetName 83, MakeUnavailable 92,
//   AddPositionChild 124, AddFakeChild 142, SetValue 144, AddScrollingChildAndSetOffsets 152, InitialiseOffsets 156.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_STLP_MALLOC_ALLOC_ALLOCATE
#define FABLE_RETAIL_STLP_LIST_BASE_CONVERT_ALLOCATOR
#define FABLE_RETAIL_STLP_LIST_CONSTRUCT_HELPER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYMAP_PASTQUESTS
#define FABLE_RETAIL_CTCINVENTORYMAP_QUESTREVIEW
#define FABLE_RETAIL_CTCINVENTORYMAP_QUESTSSCREENPC
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CACTIONPARAMULONG
#define FABLE_RETAIL_CACTION_INLINE_CTOR
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#include "engine/CTCInventoryMap.h"
#include "engine/CCountedPointer.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamULONG.h"

using NUISystem::CComponent;
using NUISystem::CAction;
using NUISystem::CActionParamULONG;

CComponent* CTCInventoryMap::ConstructQuestsScreenPC(CComponent* screen)
{
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    NUISystem::CList* menu = static_cast<NUISystem::CList*>(manager->CreateComponent("PC_QUESTS_MENU", false));
    CComponent* frame = menu->FindChild("PC_UI_FRAME_QUESTS");

    {
        CComponent* up = screen->FindChild("SCROLL_LIST_UP");
        CComponent* down = screen->FindChild("SCROLL_LIST_DOWN");
        CCountedPointer<CAction> observeUp(new CAction);
        observeUp->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeUp->AddParam(up, observeUp);
        CActionParamULONG* observeUpParam = new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        observeUp->UIActionParams.push_back(observeUpParam);
        CCountedPointer<CAction> ignoreUp(new CAction);
        ignoreUp->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreUp->AddParam(up, ignoreUp);
        CActionParamULONG* ignoreUpParam = new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        ignoreUp->UIActionParams.push_back(ignoreUpParam);
        menu->AddActionOnSelected(ignoreUp);
        menu->AddActionOnInactive(observeUp);
        CCountedPointer<CAction> observeDown(new CAction);
        observeDown->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeDown->AddParam(down, observeDown);
        CActionParamULONG* observeDownParam = new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        observeDown->UIActionParams.push_back(observeDownParam);
        CCountedPointer<CAction> ignoreDown(new CAction);
        ignoreDown->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreDown->AddParam(down, ignoreDown);
        CActionParamULONG* ignoreDownParam = new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        ignoreDown->UIActionParams.push_back(ignoreDownParam);
        menu->AddActionOnSelected(ignoreDown);
        menu->AddActionOnInactive(observeDown);
    }
    {
        CComponent* up = screen->FindChild("SCROLL_DESCRIPTION_UP");
        CComponent* down = screen->FindChild("SCROLL_DESCRIPTION_DOWN");
        CCountedPointer<CAction> observeUp(new CAction);
        observeUp->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeUp->AddParam(up, observeUp);
        CActionParamULONG* observeUpParam = new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        observeUp->UIActionParams.push_back(observeUpParam);
        CCountedPointer<CAction> ignoreUp(new CAction);
        ignoreUp->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreUp->AddParam(up, ignoreUp);
        CActionParamULONG* ignoreUpParam = new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        ignoreUp->UIActionParams.push_back(ignoreUpParam);
        menu->AddActionOnSelected(ignoreUp);
        menu->AddActionOnInactive(observeUp);
        CCountedPointer<CAction> observeDown(new CAction);
        observeDown->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeDown->AddParam(down, observeDown);
        CActionParamULONG* observeDownParam = new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        observeDown->UIActionParams.push_back(observeDownParam);
        CCountedPointer<CAction> ignoreDown(new CAction);
        ignoreDown->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreDown->AddParam(down, ignoreDown);
        CActionParamULONG* ignoreDownParam = new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        ignoreDown->UIActionParams.push_back(ignoreDownParam);
        menu->AddActionOnSelected(ignoreDown);
        menu->AddActionOnInactive(observeDown);
    }
    {
        CComponent* up = frame->FindChild("SCROLL_LIST_UP");
        CComponent* down = frame->FindChild("SCROLL_LIST_DOWN");
        CCountedPointer<CAction> observeUp(new CAction);
        observeUp->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeUp->AddParam(up, observeUp);
        CActionParamULONG* observeUpParam = new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        observeUp->UIActionParams.push_back(observeUpParam);
        CCountedPointer<CAction> ignoreUp(new CAction);
        ignoreUp->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreUp->AddParam(up, ignoreUp);
        CActionParamULONG* ignoreUpParam = new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        ignoreUp->UIActionParams.push_back(ignoreUpParam);
        menu->AddActionOnSelected(observeUp);
        menu->AddActionOnInactive(ignoreUp);
        CCountedPointer<CAction> observeDown(new CAction);
        observeDown->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeDown->AddParam(down, observeDown);
        CActionParamULONG* observeDownParam = new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        observeDown->UIActionParams.push_back(observeDownParam);
        CCountedPointer<CAction> ignoreDown(new CAction);
        ignoreDown->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreDown->AddParam(down, ignoreDown);
        CActionParamULONG* ignoreDownParam = new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        ignoreDown->UIActionParams.push_back(ignoreDownParam);
        menu->AddActionOnSelected(observeDown);
        menu->AddActionOnInactive(ignoreDown);
    }
    {
        CComponent* up = frame->FindChild("SCROLL_DESCRIPTION_UP");
        CComponent* down = frame->FindChild("SCROLL_DESCRIPTION_DOWN");
        CCountedPointer<CAction> observeUp(new CAction);
        observeUp->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeUp->AddParam(up, observeUp);
        CActionParamULONG* observeUpParam = new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        observeUp->UIActionParams.push_back(observeUpParam);
        CCountedPointer<CAction> ignoreUp(new CAction);
        ignoreUp->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreUp->AddParam(up, ignoreUp);
        CActionParamULONG* ignoreUpParam = new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        ignoreUp->UIActionParams.push_back(ignoreUpParam);
        menu->AddActionOnSelected(observeUp);
        menu->AddActionOnInactive(ignoreUp);
        CCountedPointer<CAction> observeDown(new CAction);
        observeDown->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeDown->AddParam(down, observeDown);
        CActionParamULONG* observeDownParam = new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        observeDown->UIActionParams.push_back(observeDownParam);
        CCountedPointer<CAction> ignoreDown(new CAction);
        ignoreDown->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreDown->AddParam(down, ignoreDown);
        CActionParamULONG* ignoreDownParam = new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        ignoreDown->UIActionParams.push_back(ignoreDownParam);
        menu->AddActionOnSelected(observeDown);
        menu->AddActionOnInactive(ignoreDown);
    }

    CComponent* title = menu->FindChild("PC_TITLE_QUESTS_MENU");
    frame->AddPositionChild(title);
    CWideString pastText = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_PAST_QUESTS");
    CCountedPointer<CComponent> pastEntry;
    CCountedPointer<CComponent> pastList(ConstructPastQuestsListPC(screen, menu));
    menu->InitialiseOffsets(2);
    NUISystem::CViewport* viewport = static_cast<NUISystem::CViewport*>(screen->FindChild("PC_DESCRIPTION_VIEWPORT"));
    CCountedPointer<CComponent> description;
    const NGameText::CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    CCountedPointer<CComponent> subtitle;
    CCountedPointer<CComponent> ringPic;
    CWideString curText = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_CURRENT_QUESTS");
    CCountedPointer<CComponent> curEntry;
    CCountedPointer<CComponent> curList(ConstructQuestReviewListPC(screen, menu));
    if (!curList.IsNull()) {
        curEntry.Reset(manager->CreateMenuEntryComponent(curText, 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f,
                                                    CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
        curEntry->AddChild(curList);
        curEntry->SetName("CURRENT_QUESTS_ENTRY");
    } else {
        curEntry.Reset(manager->CreateMenuEntryComponent(curText, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE", 0.0f,
                                                    CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
        curEntry->MakeUnavailable();
    }
    description.Reset(manager->CreateComponent("PC_DESCRIPTION", false));
    frame->AddPositionChild(description.Get());
    NUISystem::CText* descriptionText = static_cast<NUISystem::CText*>(description.Get());
    descriptionText->SetValue(textBank.GetTextBySymbol("TEXT_GUI_MENUGUIDE_QUESTS_CURRENTQUESTS"), true, true);
    viewport->AddFakeChild(descriptionText);
    curEntry->AddChild(description);
    subtitle.Reset(manager->CreateComponent("PC_CURRENTQUESTS_SUBTITLE", false));
    frame->AddPositionChild(subtitle.Get());
    curEntry->AddChild(subtitle);
    ringPic.Reset(manager->CreateComponent("PC_RING_PIC_CURRENT_QUESTS", false));
    frame->AddPositionChild(ringPic.Get());
    curEntry->AddChild(ringPic);
    if (!pastList.IsNull()) {
        pastEntry.Reset(manager->CreateMenuEntryComponent(pastText, 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f,
                                                    CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
        pastEntry->AddChild(pastList);
    } else {
        pastEntry.Reset(manager->CreateMenuEntryComponent(pastText, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE", 0.0f,
                                                    CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
        pastEntry->MakeUnavailable();
    }
    description.Reset(manager->CreateComponent("PC_DESCRIPTION", false));
    frame->AddPositionChild(description.Get());
    descriptionText = static_cast<NUISystem::CText*>(description.Get());
    descriptionText->SetValue(textBank.GetTextBySymbol("TEXT_GUI_MENUGUIDE_QUESTS_PASTQUESTS"), true, true);
    viewport->AddFakeChild(descriptionText);
    pastEntry->AddChild(description);
    subtitle.Reset(manager->CreateComponent("PC_PASTQUESTS_SUBTITLE", false));
    frame->AddPositionChild(subtitle.Get());
    pastEntry->AddChild(subtitle);
    ringPic.Reset(manager->CreateComponent("PC_RING_PIC_PAST_QUESTS", false));
    frame->AddPositionChild(ringPic.Get());
    pastEntry->AddChild(ringPic);
    menu->AddScrollingChildAndSetOffsets(curEntry);
    menu->AddScrollingChildAndSetOffsets(pastEntry);
    return menu;
}
