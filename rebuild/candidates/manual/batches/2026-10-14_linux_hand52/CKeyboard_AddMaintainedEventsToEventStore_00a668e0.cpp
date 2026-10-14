// CKeyboard::AddMaintainedEventsToEventStore() -- walk the maintained-event list (+0x340c) and append each node's event (node +0xc, 13 dwords) to the 256-entry event store while the stored count (+0x3404) is in range
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A668E0, length 80 bytes.
// The chunk label "CJoystick::AddMaintainedEventsToEventStore" is propagated: the +0x3404/+0x340c layout is CKeyboard's. Shape: as 0x009E4360 with the 0x100 cap, `ret`.
struct CInputEvent {                                // 0x34 bytes (PDB): the kind word at +0x28, the time at +0x2c
    unsigned char _body[0x28];
    long          m_kind;                          // +0x28
    float         m_time;                          // +0x2c
    unsigned char _pad30[4];
    long GetType() const;                          // 0x00A03B40 (labelled "CWorld::DrawGetEnvironment")
};
struct CInputEventScanInfo {                       // PDB: bool Started +0, long Index +4
    bool Started;
    long Index;
};
struct CMaintainedEvent {                          // the list element: a word then the event
    long        m_0;
    CInputEvent m_event;                           // +0x04
};
struct SMaintainedEventNode {                      // retail (VC6-era Dinkumware) list node: next, prev, value
    SMaintainedEventNode* m_pNext;
    SMaintainedEventNode* m_pPrev;
    CMaintainedEvent      m_value;                 // +0x08 (the event at +0x0c)
};
struct CInputManager;
struct CKeyboard {                                 // retail layout: 256 stored events, the one-word list head at +0x340c
    virtual void v00();
    CInputEvent           EventStore[256];         // +0x04
    long                  NoStoredEvents;          // +0x3404
    long                  NoEventScansRunning;     // +0x3408
    SMaintainedEventNode* m_pMaintainedEvents;     // +0x340c list<CMaintainedEvent> head
    bool EventScanStart(CInputEventScanInfo& scan, CInputEvent& event);
    void AddMaintainedEventsToEventStore();
    void UpdateMaintainedEvents();
};

void CKeyboard::AddMaintainedEventsToEventStore()
{
    for (SMaintainedEventNode* node = m_pMaintainedEvents->m_pNext; node != m_pMaintainedEvents; node = node->m_pNext) {
        if (NoStoredEvents >= 0 && NoStoredEvents < 256) {
            EventStore[NoStoredEvents] = node->m_value.m_event;
            ++NoStoredEvents;
        }
    }
}
