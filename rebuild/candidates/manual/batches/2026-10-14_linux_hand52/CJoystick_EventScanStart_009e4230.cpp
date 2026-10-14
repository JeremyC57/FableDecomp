// CJoystick::EventScanStart(CInputEventScanInfo& scan, CInputEvent& event) -- mark the scan started and count a running scan (+0xd14); when events are stored (+0xd10 > 0) the scan index is 0 and the first stored event (+4) is copied out (13 dwords), returning true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E4230, length 80 bytes.
// Vtable 0x0129C0EC slot 0. Shape: the scan pointer in eax before the saves, `rep movsd` through esi/edi, `mov al,1` / `xor al,al` tails, `ret 8`. The retail counters sit at +0xd10/+0xd14 (the engine header's +0xd18/+0xd1c follow a 12-byte list).
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

bool CJoystick::EventScanStart(CInputEventScanInfo& scan, CInputEvent& event)
{
    scan.Started = true;
    ++NoEventScansRunning;
    if (NoStoredEvents > 0) {
        scan.Index = 0;
        event = EventStore[0];
        return true;
    }
    return false;
}
