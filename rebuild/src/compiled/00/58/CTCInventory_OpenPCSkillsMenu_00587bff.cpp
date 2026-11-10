// CTCInventory::OpenPCSkillsMenu() -- open the PC skills screen: drop the old menu components, mark the skills menu open, reuse an open PC_SKILLS_LIST component or build one (the PC frame positions the title, the magic / expressions pictures, descriptions and subtitles and the description viewport, the descriptions also shown in that viewport; the expressions and magic menu entries get pre-actions that construct their lists; initialised and kept as a menu component), then hook its scroll buttons and back / close clickables to the UI manager's mouse input and put it in state 5
// Leaf: ?OpenPCSkillsMenu@CTCInventory@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00587BFF, 1150 bytes, thiscall `leave ; ret`; frame-pointer size TU (optimize("y",off) + ("s",on)),
//   frame 0x1C. Identity: Ego_r.pdb / FableWin.pdb ?OpenPCSkillsMenu@CTCInventory@@QAEXXZ; OpenInventoryShortcut 0x0058E99F sends screens
//   5 and 8 here; the "PC_SKILLS_LIST" literal 0x01250900.
// Callees: CTCInventory::DeleteMenuComponents 0x00587A41; NUISystem::CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D;
//   CComponent::GetName (slot 84) with the inline CCharString::operator==(const char*) (CBasicString<char>::operator== 0x004115A0);
//   CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0; operator new 0x00BFEA1A; CAction() 0x0042BE50; CCountedPointer<CAction>
//   (T*) 0x0042AA29 / ReleaseData 0x00429160; list<CActionParam*>::push_back 0x0042B646; vector<CComponent*>::push_back 0x00486024;
//   CComponent virtuals FindChild (+0x0C), Initialise (+0xAC), ChangeState (+0xC0), AddPreAction (+0x114), AddPositionChild (+0x1F0),
//   CViewport::AddFakeChild (+0x238), CObserver::ObserveEvent, CObservable::AddObserver (the manager's +8).
// Shape: the OpenPCOptionsMenu 0x0058AF10 spellings (landed); the menu entries' CActionParam bases go in through
//   `&static_cast<CActionParam&>(*entry)` (no null guard, as 0x005C5DCB).
// Inline-budget: FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ (shared fable_string.h opt-in, as the landed 0x0058AF10) -- retail expands CCharString::operator==(const char*) in place at 0x00587C52..0x00587C7C
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_OPTIONSMENU
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_EActionType.h"
#include "engine/CCountedPointer.h"

using NUISystem::CComponent;
using NUISystem::CViewport;
using NUISystem::CAction;
using NUISystem::CActionParam;

void CTCInventory::OpenPCSkillsMenu()
{
    DeleteMenuComponents();
    CurrentOpenMenu = (CURRENT_OPEN_MENU)1;

    // reuse the skills list if it is already one of the open menu components
    CComponent* list = 0;
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    for (unsigned long i = 0; i < Components.size(); ++i) {
        if (list)
            break;
        if (Components[i]->GetName() == "PC_SKILLS_LIST")
            list = Components[i];
    }

    if (!list) {
        list = manager->CreateComponent("PC_SKILLS_LIST", false);
        CComponent* frame = list->FindChild("PC_UI_FRAME");
        CComponent* description = list->FindChild("PC_DESCRIPTION_VIEWPORT");
        CViewport* viewport = 0;
        if (description)
            viewport = static_cast<CViewport*>(description);

        // the PC frame positions the screen's pictures, descriptions and subtitles
        if (frame) {
            CComponent* child = list->FindChild("PC_TITLE_SKILLS");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_RING_PIC_MAGIC");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_DESCRIPTION_MAGIC");
            if (child) {
                frame->AddPositionChild(child);
                if (viewport)
                    viewport->AddFakeChild(child);
            }
            child = list->FindChild("PC_SUBTITLE_MAGIC");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_RING_PIC_EXPRESSIONS");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_DESCRIPTION_EXPRESSIONS");
            if (child) {
                frame->AddPositionChild(child);
                if (viewport)
                    viewport->AddFakeChild(child);
            }
            child = list->FindChild("PC_SUBTITLE_EXPRESSIONS");
            if (child)
                frame->AddPositionChild(child);
            if (description)
                frame->AddPositionChild(description);
        }

        // the two menu entries construct their lists (the entry and the skills list are the action's parameters)
        CComponent* expressions = list->FindChild("PC_MENU_ENTRY_EXPRESSIONS");
        CComponent* magic = list->FindChild("PC_MENU_ENTRY_MAGIC");
        if (expressions) {
            CCountedPointer<CAction> action(new CAction);
            action->ActionType = NUISystem::UI_ACTION_TYPE_CONSTRUCT_EXPRESSIONS_LIST;
            action->UIActionParams.push_back(&static_cast<CActionParam&>(*expressions));
            action->UIActionParams.push_back(&static_cast<CActionParam&>(*list));
            expressions->AddPreAction(action);
        }
        if (magic) {
            CCountedPointer<CAction> action(new CAction);
            action->ActionType = NUISystem::UI_ACTION_TYPE_CONSTRUCT_MAGIC_LIST_PC;
            action->UIActionParams.push_back(&static_cast<CActionParam&>(*magic));
            action->UIActionParams.push_back(&static_cast<CActionParam&>(*list));
            magic->AddPreAction(action);
        }
        list->Initialise();
        Components.push_back(list);
    }

    CComponent* scroll = list->FindChild("SCROLL_LIST_UP");
    if (scroll)
        scroll->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    scroll = list->FindChild("SCROLL_DESCRIPTION_UP");
    if (scroll)
        scroll->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    scroll = list->FindChild("SCROLL_LIST_DOWN");
    if (scroll)
        scroll->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    scroll = list->FindChild("SCROLL_DESCRIPTION_DOWN");
    if (scroll)
        scroll->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    CComponent* clickable = list->FindChild("PC_BACK_CLICKABLE");
    if (clickable) {
        NUISystem::CManager::GetInstance()->AddObserver(clickable);
        clickable->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    }
    clickable = list->FindChild("PC_CLOSE_CLICKABLE");
    if (clickable) {
        NUISystem::CManager::GetInstance()->AddObserver(clickable);
        clickable->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    }
    list->ChangeState(5);
}