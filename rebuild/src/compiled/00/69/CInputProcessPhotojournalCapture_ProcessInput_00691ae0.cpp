// CInputProcessPhotojournalCapture::ProcessInput(const CInputEvent&, CProcessedInput&) -- translate character and photojournal actions into GUI events
// Leaf: ?ProcessInput@CInputProcessPhotojournalCapture@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00691AE0, 278 bytes, thiscall, `ret 8`. Callees: CInputEvent::GetType 0x00A03B40, GetChar 0x00A03B80,
//   NUISystem::CManager::GetInstance 0x0041E5F2 (slot 12 SetKeyPressed [+0x30], slot 0 SetEvent [+0x00] of CObservable),
//   CInputProcessGameBase::IsEventButtonPressed 0x00687A90 (protected const) and IsEventGameAction 0x00687CF0; GetMouseMovement
//   (Ego_r 0x006B2E30) and CManager::SetMouseMovement are inline (the event's movement +0x0C/+0x10 into MouseMovement +0xB0/+0xB4).
// Event values: EInputEventType 1 (character) / 15 (repeated character; the header declares the enum without enumerators);
//   EGameAction 71 / 25 / 76 (MOVE_MOUSE_ON_GUI / INVENTORY_SELECT / INVENTORY_UNSELECT); NUISystem::EEvent
//   UI_EVENT_KEY_DOWN 33, UI_EVENT_CHAR_DOWN 34, UI_EVENT_MOUSE_MOVEMENT 25, UI_EVENT_LEFT_MOUSE_CLICK 26, UI_EVENT_BACK 5.
// NF62 (2026-11-17): ported onto engine/CInputProcessPhotojournalCapture.h / CInputProcessGameBase.h (ZTARGET) / CInputEvent.h /
//   NUISystem_CManager.h (the PhotoInput namespace, PhotoCapture / InputEvent / Manager stand-ins and member-pointer vtable calls deleted).
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_GETMOUSEMOVEMENT
#define FABLE_RETAIL_CINPUTEVENT_GETCHAR
#define FABLE_RETAIL_CINPUTPROCESSPHOTOJOURNALCAPTURE_PROCESSINPUT
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CMANAGER_SETMOUSEMOVEMENT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/EGameAction.h"
#include "engine/CInputEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/NUISystem_enums.h"
#include "engine/NUISystem_CManager.h"
#include "engine/CInputProcessPhotojournalCapture.h"

void CInputProcessPhotojournalCapture::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input)
{
    if (event.GetType() == 1)
    {
        NUISystem::CManager::GetInstance()->SetKeyPressed(event.GetChar());
        NUISystem::CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_KEY_DOWN);
    }
    if (event.GetType() == 15)
    {
        NUISystem::CManager::GetInstance()->SetKeyPressed(event.GetChar());
        NUISystem::CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_CHAR_DOWN);
    }
    if (IsEventButtonPressed(event))
    {
        if (IsEventGameAction(GAME_ACTION_MOVE_MOUSE_ON_GUI, event))
        {
            C2DVector movement = GetMouseMovement(event);
            NUISystem::CManager::GetInstance()->SetMouseMovement(movement);
            NUISystem::CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        }
        else if (IsEventGameAction(GAME_ACTION_INVENTORY_SELECT, event))
            NUISystem::CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_LEFT_MOUSE_CLICK);
        else if (IsEventGameAction(GAME_ACTION_INVENTORY_UNSELECT, event))
            NUISystem::CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_BACK);
    }
    else if (IsEventGameAction(GAME_ACTION_MOVE_MOUSE_ON_GUI, event))
    {
        C2DVector movement = GetMouseMovement(event);
        NUISystem::CManager::GetInstance()->SetMouseMovement(movement);
        NUISystem::CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    }
}
