// CMouse::ProcessMaintainedEventsFromEvent(const CInputEvent* event) -- a button press appends a maintained "button held" event at the press position; the matching release erases the first held event of that button
// Leaf: ?ProcessMaintainedEventsFromEvent@CMouse@@IAEXPBVCInputEvent@@@Z
// Identity: Ego_r.pdb ?ProcessMaintainedEventsFromEvent@CMouse@@IAEXPBVCInputEvent@@@Z (protected, the event by pointer).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A66FD0, length 976 bytes (incl. a 3-entry jump table + 15-entry byte index table), `ret 4`.
// Callees: CInputEvent::GetType @ 0x00A03B40 (labelled "CWorld::DrawGetEnvironment"), CInputEvent::GetMousePos @ 0x00A04380 (labelled
//          "CMessageEventFilter_IsOriginatedByThing"), CInputEvent::SetAsLeft/Middle/RightMouseButtonHeld @ 0x00A03D10 / 0x00A03DC0 /
//          0x00A03E70, SetAsExtraMouseButtonHeld @ 0x00A03F20, list<CMaintainedEvent>::insert(iterator) @ 0x009E4A20 (labelled
//          "CInputEvent_AllocNode"), the out-of-line default push_back @ 0x009E4AA0 (`insert(end())`; labelled "CDefString::ToWideString"),
//          list<CMaintainedEvent>::erase(iterator) @ 0x009E47E0 (labelled "EventNode_Unlink").
// Layout: engine/CMouse.h retail form (CBaseClassNonCopyable; MaintainedEventsList the 4-byte STLport list at +0x340c), CInputEvent::StartTime +0x30.
// Shape: an if-chain re-reading GetType() for the three standard buttons (press 4/7/10 -> held 5/8/11, release 6/12/9), then a switch for
//   the five extra buttons (press 22..26, release 32..36 erases the held type `type - 5`); each arm returns.
// Lever: retail's TU holds the list<CMaintainedEvent> insert(iterator) / push_back() / erase(iterator) bodies (0x009E4A20 /
//   0x009E4AA0 / 0x009E47E0), called out of line: the shared STLport header's visible-noinline opt-ins
//   (FABLE_RETAIL_STLP_LIST_INSERT_DEFAULT_NOINLINE / _ERASE_NOINLINE) tell c2 the iterators are not kept, which gives retail's frame.
// Headers: engine/CMouse.h (FABLE_RETAIL_CMOUSE: retail one-word STLport list at +0x340c), engine/CMaintainedEvent.h,
//   engine/CInputEvent.h (FABLE_RETAIL_CINPUTEVENT_MOUSE_SETTERS, _COPY), stlport/fable_containers.h. Grade with FABLE_HDR_OVERLAY=agentQ6/hdr.
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_INSERT_DEFAULT_NOINLINE
#define FABLE_RETAIL_STLP_LIST_ERASE_NOINLINE
#define FABLE_RETAIL_CINPUTEVENT_COPY
#define FABLE_RETAIL_CINPUTEVENT_MOUSE_SETTERS
#define FABLE_RETAIL_CMOUSE
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CInputEvent.h"
#include "engine/CMaintainedEvent.h"
#include "engine/CMouse.h"
enum {                                          // EInputEventType values (Ego_r.pdb enumerator names are not in the repo's extracts; button order Left 4-6, Middle 7-9, Right 10-12 from the type each retail SetAs*MouseButtonHeld body stores: 0x00A03D10 = 5, 0x00A03DC0 = 8, 0x00A03E70 = 0xB)
    INPUT_EVENT_LMB_PRESSED = 4,
    INPUT_EVENT_LMB_HELD = 5,
    INPUT_EVENT_LMB_RELEASED = 6,
    INPUT_EVENT_MMB_PRESSED = 7,
    INPUT_EVENT_MMB_HELD = 8,
    INPUT_EVENT_MMB_RELEASED = 9,
    INPUT_EVENT_RMB_PRESSED = 10,
    INPUT_EVENT_RMB_HELD = 11,
    INPUT_EVENT_RMB_RELEASED = 12,
    INPUT_EVENT_EXTRA_BUTTON_PRESSED_0 = 22,
    INPUT_EVENT_EXTRA_BUTTON_PRESSED_1,
    INPUT_EVENT_EXTRA_BUTTON_PRESSED_2,
    INPUT_EVENT_EXTRA_BUTTON_PRESSED_3,
    INPUT_EVENT_EXTRA_BUTTON_PRESSED_4,
    INPUT_EVENT_EXTRA_BUTTON_HELD_0,
    INPUT_EVENT_EXTRA_BUTTON_HELD_1,
    INPUT_EVENT_EXTRA_BUTTON_HELD_2,
    INPUT_EVENT_EXTRA_BUTTON_HELD_3,
    INPUT_EVENT_EXTRA_BUTTON_HELD_4,
    INPUT_EVENT_EXTRA_BUTTON_RELEASED_0,
    INPUT_EVENT_EXTRA_BUTTON_RELEASED_1,
    INPUT_EVENT_EXTRA_BUTTON_RELEASED_2,
    INPUT_EVENT_EXTRA_BUTTON_RELEASED_3,
    INPUT_EVENT_EXTRA_BUTTON_RELEASED_4
};

void CMouse::ProcessMaintainedEventsFromEvent(const CInputEvent* event)
{
    if (event->GetType() == INPUT_EVENT_LMB_PRESSED) {
        MaintainedEventsList.insert(MaintainedEventsList.end());
        CInputEvent& held = MaintainedEventsList.back().Event;
        C2DVector pos;
        pos = event->GetMousePos();
        C3DVector movement(0.0f, 0.0f, 0.0f);
        double startTime = event->StartTime;
        double currentTime = event->StartTime;
        held.SetAsLeftMouseButtonHeld(pos, movement, startTime, currentTime);
        return;
    }
    if (event->GetType() == INPUT_EVENT_MMB_PRESSED) {
        MaintainedEventsList.insert(MaintainedEventsList.end());
        CInputEvent& held = MaintainedEventsList.back().Event;
        C2DVector pos;
        pos = event->GetMousePos();
        C3DVector movement(0.0f, 0.0f, 0.0f);
        double startTime = event->StartTime;
        double currentTime = event->StartTime;
        held.SetAsMiddleMouseButtonHeld(pos, movement, startTime, currentTime);
        return;
    }
    if (event->GetType() == INPUT_EVENT_RMB_PRESSED) {
        MaintainedEventsList.insert(MaintainedEventsList.end());
        CInputEvent& held = MaintainedEventsList.back().Event;
        C2DVector pos;
        pos = event->GetMousePos();
        C3DVector movement(0.0f, 0.0f, 0.0f);
        double startTime = event->StartTime;
        double currentTime = event->StartTime;
        held.SetAsRightMouseButtonHeld(pos, movement, startTime, currentTime);
        return;
    }
    if (event->GetType() == INPUT_EVENT_LMB_RELEASED) {
        for (std::list<CMaintainedEvent>::iterator it = MaintainedEventsList.begin(); it != MaintainedEventsList.end(); ++it) {
            if (it->Event.GetType() == INPUT_EVENT_LMB_HELD) {
                MaintainedEventsList.erase(it);
                return;
            }
        }
        return;
    }
    if (event->GetType() == INPUT_EVENT_RMB_RELEASED) {
        for (std::list<CMaintainedEvent>::iterator it = MaintainedEventsList.begin(); it != MaintainedEventsList.end(); ++it) {
            if (it->Event.GetType() == INPUT_EVENT_RMB_HELD) {
                MaintainedEventsList.erase(it);
                return;
            }
        }
        return;
    }
    if (event->GetType() == INPUT_EVENT_MMB_RELEASED) {
        for (std::list<CMaintainedEvent>::iterator it = MaintainedEventsList.begin(); it != MaintainedEventsList.end(); ++it) {
            if (it->Event.GetType() == INPUT_EVENT_MMB_HELD) {
                MaintainedEventsList.erase(it);
                return;
            }
        }
        return;
    }
    switch (event->GetType()) {
    case INPUT_EVENT_EXTRA_BUTTON_PRESSED_0:
    case INPUT_EVENT_EXTRA_BUTTON_PRESSED_1:
    case INPUT_EVENT_EXTRA_BUTTON_PRESSED_2:
    case INPUT_EVENT_EXTRA_BUTTON_PRESSED_3:
    case INPUT_EVENT_EXTRA_BUTTON_PRESSED_4: {
        MaintainedEventsList.push_back();
        CInputEvent& held = MaintainedEventsList.back().Event;
        C2DVector pos;
        pos = event->GetMousePos();
        C3DVector movement(0.0f, 0.0f, 0.0f);
        double currentTime = event->StartTime;
        double startTime = event->StartTime;
        held.SetAsExtraMouseButtonHeld(pos, movement, startTime, currentTime, event->GetType() - INPUT_EVENT_EXTRA_BUTTON_PRESSED_0);
        return;
    }
    case INPUT_EVENT_EXTRA_BUTTON_RELEASED_0:
    case INPUT_EVENT_EXTRA_BUTTON_RELEASED_1:
    case INPUT_EVENT_EXTRA_BUTTON_RELEASED_2:
    case INPUT_EVENT_EXTRA_BUTTON_RELEASED_3:
    case INPUT_EVENT_EXTRA_BUTTON_RELEASED_4: {
        long held = event->GetType() - (INPUT_EVENT_EXTRA_BUTTON_RELEASED_0 - INPUT_EVENT_EXTRA_BUTTON_HELD_0);
        for (std::list<CMaintainedEvent>::iterator it = MaintainedEventsList.begin(); it != MaintainedEventsList.end(); ++it) {
            if (it->Event.GetType() == held) {
                MaintainedEventsList.erase(it);
                return;
            }
        }
        return;
    }
    }
}