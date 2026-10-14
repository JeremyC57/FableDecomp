// CJoystick::AddMaintainedEventsToEventStore() -- walk the maintained-event list (+0xd0c, a VC6-era Dinkumware ring) and append each node's event (node +0xc, 13 dwords) to the 64-entry event store while the stored count (+0xd10) is in range
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E4360, length 80 bytes.
// No callees. Shape: `this` in edx (no calls, so a volatile register), the node in ebx, the head re-read for the loop test, the count re-read and incremented in memory, `rep movsd`, `ret`.
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
struct CJoystick {                                 // retail layout (0xd18 bytes): the engine header's list is a 12-byte VC7 list, retail's is the one-word head at +0xd0c
    virtual bool EventScanStart(CInputEventScanInfo& scan, CInputEvent& event);   // slot 0
    CInputEvent           EventStore[64];          // +0x04
    CInputManager*        InputManager;            // +0xd04
    bool                  ExclusiveMode;           // +0xd08
    unsigned char         _pad0d09[3];
    SMaintainedEventNode* m_pMaintainedEvents;     // +0xd0c list<CMaintainedEvent> head
    long                  NoStoredEvents;          // +0xd10
    long                  NoEventScansRunning;     // +0xd14
    void AddMaintainedEventsToEventStore();
};

void CJoystick::AddMaintainedEventsToEventStore()
{
    for (SMaintainedEventNode* node = m_pMaintainedEvents->m_pNext; node != m_pMaintainedEvents; node = node->m_pNext) {
        if (NoStoredEvents >= 0 && NoStoredEvents < 64) {
            EventStore[NoStoredEvents] = node->m_value.m_event;
            ++NoStoredEvents;
        }
    }
}
