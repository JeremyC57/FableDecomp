// CTCInventory::OpenPCStatusMenu() -- open the PC status screen: drop the old menu components, mark the status menu open, reuse an open PC_STATUS_LIST component or build one (the fading frame positions the title, the quests / logbook / experience / personality pictures, descriptions and subtitles and the description viewport, the descriptions also shown in that viewport; the quests and logbook entries get pre-actions that open their pages, the experience and personality entries too when the hero has an inventory-stats component, else they are made unavailable with their right selection arrows deleted; initialised and kept as a menu component), then hook its scroll buttons and back / close clickables to the UI manager's mouse input and put it in state 5
// Leaf: ?OpenPCStatusMenu@CTCInventory@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0058A763, 1965 bytes, thiscall `leave ; ret`; frame-pointer size TU (optimize("y",off) + ("s",on)),
//   frame 0x38. Identity: Ego_r.pdb / FableWin.pdb ?OpenPCStatusMenu@CTCInventory@@QAEXXZ; OpenInventoryShortcut 0x0058E99F sends screens
//   3, 6 and 7 here; the "PC_STATUS_LIST" literal 0x0125119C.
// Callees: as OpenPCSkillsMenu 0x00587BFF (landed this chunk), plus CThing::GetTC (bitset test 0x00410DE0, LowerBound 0x0040F020) for
//   TCI_INVENTORY_STATS, CComponent MakeUnavailable (slot 92) / SetDeletion (slot 62) with NUISystem::CDeletion 0x0042BED4 / copy
//   0x0042CD84 / list dtor 0x0042ABCA.
// Inline-budget: FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ (shared fable_string.h opt-in, as the landed 0x0058AF10) -- retail expands CCharString::operator==(const char*) in place at 0x0058A7B9..0x0058A7E3
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
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CDELETION
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
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_EActionType.h"
#include "engine/CCountedPointer.h"

using NUISystem::CComponent;
using NUISystem::CViewport;
using NUISystem::CAction;
using NUISystem::CActionParam;

class CTCInventoryStats;                                       // interface TCI_INVENTORY_STATS (only its presence is tested)

void CTCInventory::OpenPCStatusMenu()
{
    DeleteMenuComponents();
    CurrentOpenMenu = (CURRENT_OPEN_MENU)7;

    // reuse the status list if it is already one of the open menu components
    CComponent* list = 0;
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    for (unsigned long i = 0; i < Components.size(); ++i) {
        if (list)
            break;
        if (Components[i]->GetName() == "PC_STATUS_LIST")
            list = Components[i];
    }

    if (!list) {
        list = manager->CreateComponent("PC_STATUS_LIST", false);
        CComponent* frame = list->FindChild("PC_UI_FADING_FRAME");
        CComponent* description = list->FindChild("PC_DESCRIPTION_VIEWPORT");
        CViewport* viewport = 0;
        if (description)
            viewport = static_cast<CViewport*>(description);

        // the fading frame positions the screen's pictures, descriptions and subtitles
        if (frame) {
            CComponent* child = list->FindChild("PC_TITLE_STATUS");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_RING_PIC_QUESTS");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_DESCRIPTION_QUESTS");
            if (child) {
                frame->AddPositionChild(child);
                if (viewport)
                    viewport->AddFakeChild(child);
            }
            child = list->FindChild("PC_SUBTITLE_QUESTS");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_RING_PIC_LOGBOOK");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_DESCRIPTION_LOGBOOK");
            if (child) {
                frame->AddPositionChild(child);
                if (viewport)
                    viewport->AddFakeChild(child);
            }
            child = list->FindChild("PC_SUBTITLE_LOGBOOK");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_RING_PIC_EXPERIENCE");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_DESCRIPTION_EXPERIENCE");
            if (child) {
                frame->AddPositionChild(child);
                if (viewport)
                    viewport->AddFakeChild(child);
            }
            child = list->FindChild("PC_SUBTITLE_EXPERIENCE");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_RING_PIC_PERSONALITY");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_DESCRIPTION_PERSONALITY");
            if (child) {
                frame->AddPositionChild(child);
                if (viewport)
                    viewport->AddFakeChild(child);
            }
            child = list->FindChild("PC_SUBTITLE_PERSONALITY");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_DESCRIPTION_VIEWPORT");
            if (child)
                frame->AddPositionChild(child);
        }

        // the four menu entries open their pages (the entry and the status list are the action's parameters)
        CComponent* quests = list->FindChild("PC_MENU_ENTRY_QUESTS");
        CComponent* logBook = list->FindChild("PC_MENU_ENTRY_LOG_BOOK");
        CComponent* experience = list->FindChild("PC_MENU_ENTRY_EXPERIENCE");
        CComponent* personality = list->FindChild("PC_MENU_ENTRY_PERSONALITY");
        if (quests) {
            CCountedPointer<CAction> action(new CAction);
            action->ActionType = NUISystem::UI_ACTION_TYPE_CONSTRUCT_PC_QUESTS_LIST;
            action->UIActionParams.push_back(&static_cast<CActionParam&>(*quests));
            action->UIActionParams.push_back(&static_cast<CActionParam&>(*list));
            quests->AddPreAction(action);
        }
        if (logBook) {
            CCountedPointer<CAction> action(new CAction);
            action->ActionType = NUISystem::UI_ACTION_TYPE_OPEN_PC_LOG_BOOK;
            action->UIActionParams.push_back(&static_cast<CActionParam&>(*logBook));
            action->UIActionParams.push_back(&static_cast<CActionParam&>(*list));
            logBook->AddPreAction(action);
        }
        // experience and personality need the hero's inventory stats: without them the entries are unavailable
        CTCInventoryStats* stats = 0;
        ParentThing->GetTC(TCI_INVENTORY_STATS, &stats);
        if (experience) {
            if (stats) {
                CCountedPointer<CAction> action(new CAction);
                action->ActionType = NUISystem::UI_ACTION_TYPE_CONSTRUCT_PC_EXPERIENCE_LIST;
                action->UIActionParams.push_back(&static_cast<CActionParam&>(*experience));
                action->UIActionParams.push_back(&static_cast<CActionParam&>(*list));
                experience->AddPreAction(action);
            } else {
                experience->MakeUnavailable();
                CComponent* arrow = experience->FindChild("UI_SELECTION_ARROW_RIGHT");
                if (arrow) {
                    NUISystem::CDeletion deletion;
                    deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                    arrow->SetDeletion(deletion);
                }
            }
        }
        if (personality) {
            if (stats) {
                CCountedPointer<CAction> action(new CAction);
                action->ActionType = NUISystem::UI_ACTION_TYPE_CONSTRUCT_PC_PERSONALITY_LIST;
                action->UIActionParams.push_back(&static_cast<CActionParam&>(*personality));
                action->UIActionParams.push_back(&static_cast<CActionParam&>(*list));
                personality->AddPreAction(action);
            } else {
                personality->MakeUnavailable();
                CComponent* arrow = personality->FindChild("UI_SELECTION_ARROW_RIGHT");
                if (arrow) {
                    NUISystem::CDeletion deletion;
                    deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                    arrow->SetDeletion(deletion);
                }
            }
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