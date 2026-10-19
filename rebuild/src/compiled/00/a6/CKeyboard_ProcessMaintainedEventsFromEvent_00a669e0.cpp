// CKeyboard::ProcessMaintainedEventsFromEvent(const CInputEvent& event) -- a key-down event (type 1) appends a maintained "key held" event; a key-up (type 3) erases every held event (type 2) of that key
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A669E0, length 212 bytes (`ret 4`).
// Callees: CInputEvent::GetType @ 0x00A03B40 (labelled CWorld::DrawGetEnvironment), CInputEvent::GetKey @ 0x00A03B70,
// CInputEvent::SetAsKeyboardKeyHeld(EInputKey, double, double) @ 0x00A03C20, list<CMaintainedEvent>::insert(iterator) @ 0x009E4A20
// (default-constructed value, iterator returned by hidden pointer), free @ 0x00BFEA14 (the node), the element's virtual destructor (slot 0).
// Layout: the engine header CKeyboard.h gives MaintainedEventsList at +0x340C as a 12-byte PDB list; retail's list is the one-word
// sentinel pointer used by the landed 0x00A66880/0x00A668E0, so `this` is modelled locally with the PDB member names.
// CMaintainedEvent is a CBaseClass (vptr at +0) with Event at +4 (node +0x0C); CInputEvent::StartTime is +0x30.
// Shape: the discarded insert result is homed in the dead `event` argument slot (`lea edx,[esp+18h]`); the by-value iterator has a user
// copy ctor so it is built in the argument area; back() is head->prev; the erase is inlined (unlink, virtual dtor, free) with no size count.
extern "C" void __cdecl free(void* p);          // 0x00BFEA14
struct CInputEvent {                            // 0x34 bytes
    unsigned char Data[0x20];                   // +0x00
    long          DeviceType;                   // +0x20
    long          DeviceNumber;                 // +0x24
    long          Type;                         // +0x28
    float         CurrentTime;                  // +0x2c
    float         StartTime;                    // +0x30
    long GetType() const;                                                   // 0x00A03B40
    long GetKey() const;                                                    // 0x00A03B70
    void SetAsKeyboardKeyHeld(long key, double startTime, double currentTime);  // 0x00A03C20
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
    Node* last() { return head->prev; }
};
struct CInputManager;
struct CKeyboard {
    void*                __vftable;             // +0x00
    CInputEvent          EventStore[256];       // +0x04
    long                 NoStoredEvents;        // +0x3404
    long                 NoEventScansRunning;   // +0x3408
    CMaintainedEventList MaintainedEventsList;  // +0x340c
    CInputManager*       InputManager;          // +0x3410 (retail)
    void ProcessMaintainedEventsFromEvent(const CInputEvent& event);
};

void CKeyboard::ProcessMaintainedEventsFromEvent(const CInputEvent& event)
{
    if (event.GetType() == 1) {
        MaintainedEventsList.insert(MaintainedEventsList.end());
        MaintainedEventsList.last()->value.Event.SetAsKeyboardKeyHeld(event.GetKey(), event.StartTime, event.StartTime);
        return;
    }
    if (event.GetType() == 3) {
        CMaintainedEventList::Node* node = MaintainedEventsList.head->next;
        while (node != MaintainedEventsList.head) {
            if (node->value.Event.GetType() == 2) {
                if (node->value.Event.GetKey() == event.GetKey()) {
                    CMaintainedEventList::Node* next = node->next;
                    CMaintainedEventList::Node* prev = node->prev;
                    prev->next = next;
                    next->prev = prev;
                    node->value.~CMaintainedEvent();
                    free(node);
                    node = next;
                } else {
                    node = node->next;
                }
            }
        }
    }
}