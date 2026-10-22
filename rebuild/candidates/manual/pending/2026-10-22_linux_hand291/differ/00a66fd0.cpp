// Grade: DIFFER(963v963) -- stack-slot layout only: retail homes the discarded insert/erase iterator at esp+10h and pos at esp+18h (ours 0Ch / 10h); the extra-button doubles sit at 10h/20h around pos (ours pos 10h, doubles 18h/20h)
// CMouse::ProcessMaintainedEventsFromEvent(const CInputEvent& event) -- a button press appends a maintained "button held" event at the press position; the matching release erases the first held event of that button
// Leaf: ProcessMaintainedEventsFromEvent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A66FD0, length 976 bytes (incl. a 3-entry jump table + 15-entry byte index table), `ret 4`.
// Callees: CInputEvent::GetType @ 0x00A03B40 (labelled "CWorld::DrawGetEnvironment"), CInputEvent::GetMousePos @ 0x00A04380 (labelled
//          "CMessageEventFilter_IsOriginatedByThing"), CInputEvent::SetAsLeft/Right/MiddleMouseButtonHeld @ 0x00A03D10 / 0x00A03DC0 /
//          0x00A03E70, SetAsExtraMouseButtonHeld @ 0x00A03F20, list<CMaintainedEvent>::insert(iterator) @ 0x009E4A20 (labelled
//          "CInputEvent_AllocNode"), the out-of-line default push_back @ 0x009E4AA0 (`insert(end())`; labelled "CDefString::ToWideString"),
//          list<CMaintainedEvent>::erase(iterator) @ 0x009E47E0 (labelled "EventNode_Unlink").
// Layout: as the landed CKeyboard::ProcessMaintainedEventsFromEvent 0x00A669E0 (one-word sentinel list at +0x340c, CInputEvent::StartTime +0x30).
// Shape: an if-chain re-reading GetType() for the three standard buttons (press 4/7/10 -> held 5/8/11, release 6/12/9), then a switch for
//   the five extra buttons (press 22..26, release 32..36 erases the held type `type - 5`); each arm returns.
struct C2DVector {
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

enum EInputEventType {
    INPUT_EVENT_LMB_PRESSED = 4,
    INPUT_EVENT_LMB_HELD = 5,
    INPUT_EVENT_LMB_RELEASED = 6,
    INPUT_EVENT_RMB_PRESSED = 7,
    INPUT_EVENT_RMB_HELD = 8,
    INPUT_EVENT_RMB_RELEASED = 9,
    INPUT_EVENT_MMB_PRESSED = 10,
    INPUT_EVENT_MMB_HELD = 11,
    INPUT_EVENT_MMB_RELEASED = 12,
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

struct CInputEvent {                            // 0x34 bytes
    unsigned char Data[0x20];                   // +0x00
    long          DeviceType;                   // +0x20
    long          DeviceNumber;                 // +0x24
    long          Type;                         // +0x28
    float         CurrentTime;                  // +0x2c
    float         StartTime;                    // +0x30
    long GetType() const;                                                   // 0x00A03B40
    C2DVector GetMousePos() const;                                          // 0x00A04380
    void SetAsLeftMouseButtonHeld(const C2DVector& pos, const C3DVector& movement, double startTime, double currentTime);   // 0x00A03D10
    void SetAsRightMouseButtonHeld(const C2DVector& pos, const C3DVector& movement, double startTime, double currentTime);  // 0x00A03DC0
    void SetAsMiddleMouseButtonHeld(const C2DVector& pos, const C3DVector& movement, double startTime, double currentTime); // 0x00A03E70
    void SetAsExtraMouseButtonHeld(const C2DVector& pos, const C3DVector& movement, double startTime, double currentTime, long button); // 0x00A03F20
};
struct CMaintainedEvent {                       // CBaseClass-derived: vptr +0, Event +4
    virtual ~CMaintainedEvent();
    CInputEvent Event;
};
struct CMaintainedEventList {                   // retail list<CMaintainedEvent>: one sentinel pointer
    struct Node { Node* next; Node* prev; CMaintainedEvent value; };
    struct iterator {
        Node* ptr;
        iterator() { }
        iterator(Node* p) : ptr(p) { }
        iterator(const iterator& o) : ptr(o.ptr) { }
    };
    Node* head;
    iterator end() { return iterator(head); }
    iterator insert(iterator where);            // 0x009E4A20
    void push_back();                           // 0x009E4AA0
    iterator erase(iterator where);             // 0x009E47E0
    Node* last() { return head->prev; }
};
struct CInputManager;
struct CMouse {
    void*                __vftable;             // +0x00
    CInputEvent          EventStore[256];       // +0x04
    long                 NoStoredEvents;        // +0x3404
    long                 NoEventScansRunning;   // +0x3408
    CMaintainedEventList MaintainedEventsList;  // +0x340c
    void ProcessMaintainedEventsFromEvent(const CInputEvent& event);
};

void CMouse::ProcessMaintainedEventsFromEvent(const CInputEvent& event)
{
    if (event.GetType() == INPUT_EVENT_LMB_PRESSED) {
        MaintainedEventsList.insert(MaintainedEventsList.end());
        CInputEvent& held = MaintainedEventsList.last()->value.Event;
        C2DVector pos;
        pos = event.GetMousePos();
        C3DVector movement(0.0f, 0.0f, 0.0f);
        double startTime = event.StartTime;
        double currentTime = event.StartTime;
        held.SetAsLeftMouseButtonHeld(pos, movement, startTime, currentTime);
        return;
    }
    if (event.GetType() == INPUT_EVENT_RMB_PRESSED) {
        MaintainedEventsList.insert(MaintainedEventsList.end());
        CInputEvent& held = MaintainedEventsList.last()->value.Event;
        C2DVector pos;
        pos = event.GetMousePos();
        C3DVector movement(0.0f, 0.0f, 0.0f);
        double startTime = event.StartTime;
        double currentTime = event.StartTime;
        held.SetAsRightMouseButtonHeld(pos, movement, startTime, currentTime);
        return;
    }
    if (event.GetType() == INPUT_EVENT_MMB_PRESSED) {
        MaintainedEventsList.insert(MaintainedEventsList.end());
        CInputEvent& held = MaintainedEventsList.last()->value.Event;
        C2DVector pos;
        pos = event.GetMousePos();
        C3DVector movement(0.0f, 0.0f, 0.0f);
        double startTime = event.StartTime;
        double currentTime = event.StartTime;
        held.SetAsMiddleMouseButtonHeld(pos, movement, startTime, currentTime);
        return;
    }
    if (event.GetType() == INPUT_EVENT_LMB_RELEASED) {
        for (CMaintainedEventList::Node* node = MaintainedEventsList.head->next; node != MaintainedEventsList.head; node = node->next) {
            if (node->value.Event.GetType() == INPUT_EVENT_LMB_HELD) {
                MaintainedEventsList.erase(CMaintainedEventList::iterator(node));
                return;
            }
        }
        return;
    }
    if (event.GetType() == INPUT_EVENT_MMB_RELEASED) {
        for (CMaintainedEventList::Node* node = MaintainedEventsList.head->next; node != MaintainedEventsList.head; node = node->next) {
            if (node->value.Event.GetType() == INPUT_EVENT_MMB_HELD) {
                MaintainedEventsList.erase(CMaintainedEventList::iterator(node));
                return;
            }
        }
        return;
    }
    if (event.GetType() == INPUT_EVENT_RMB_RELEASED) {
        for (CMaintainedEventList::Node* node = MaintainedEventsList.head->next; node != MaintainedEventsList.head; node = node->next) {
            if (node->value.Event.GetType() == INPUT_EVENT_RMB_HELD) {
                MaintainedEventsList.erase(CMaintainedEventList::iterator(node));
                return;
            }
        }
        return;
    }
    switch (event.GetType()) {
    case INPUT_EVENT_EXTRA_BUTTON_PRESSED_0:
    case INPUT_EVENT_EXTRA_BUTTON_PRESSED_1:
    case INPUT_EVENT_EXTRA_BUTTON_PRESSED_2:
    case INPUT_EVENT_EXTRA_BUTTON_PRESSED_3:
    case INPUT_EVENT_EXTRA_BUTTON_PRESSED_4: {
        MaintainedEventsList.push_back();
        CInputEvent& held = MaintainedEventsList.last()->value.Event;
        C2DVector pos;
        pos = event.GetMousePos();
        C3DVector movement(0.0f, 0.0f, 0.0f);
        double currentTime = event.StartTime;
        double startTime = event.StartTime;
        held.SetAsExtraMouseButtonHeld(pos, movement, startTime, currentTime, event.GetType() - INPUT_EVENT_EXTRA_BUTTON_PRESSED_0);
        return;
    }
    case INPUT_EVENT_EXTRA_BUTTON_RELEASED_0:
    case INPUT_EVENT_EXTRA_BUTTON_RELEASED_1:
    case INPUT_EVENT_EXTRA_BUTTON_RELEASED_2:
    case INPUT_EVENT_EXTRA_BUTTON_RELEASED_3:
    case INPUT_EVENT_EXTRA_BUTTON_RELEASED_4: {
        long held = event.GetType() - (INPUT_EVENT_EXTRA_BUTTON_RELEASED_0 - INPUT_EVENT_EXTRA_BUTTON_HELD_0);
        for (CMaintainedEventList::Node* node = MaintainedEventsList.head->next; node != MaintainedEventsList.head; node = node->next) {
            if (node->value.Event.GetType() == held) {
                MaintainedEventsList.erase(CMaintainedEventList::iterator(node));
                return;
            }
        }
        return;
    }
    }
}
