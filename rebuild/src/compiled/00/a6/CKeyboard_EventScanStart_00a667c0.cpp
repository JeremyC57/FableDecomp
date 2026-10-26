// CKeyboard::EventScanStart(CInputEventScanInfo& scan, CInputEvent& event) -- mark the scan started and count a running scan (+0x3408); when events are stored (+0x3404 > 0) the scan index is 0 and the first stored event (+4) is copied out (13 dwords), returning true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A667C0, length 80 bytes.
// The chunk label "CJoystick::EventScanStart" is propagated: the +0x3404/+0x3408 counters are CKeyboard's (engine header). Shape: as the joystick body (0x009E4230), `ret 8`.
class CInputEvent { public:                                // 0x34 bytes (PDB): the kind word at +0x28, the time at +0x2c
    unsigned char _body[0x28];
    long          m_kind;                          // +0x28
    float         m_time;                          // +0x2c
    unsigned char _pad30[4];
    long GetType() const;                          // 0x00A03B40 (labelled "CWorld::DrawGetEnvironment")
};
class CInputEventScanInfo { public:                       // PDB: bool Started +0, long Index +4
    bool Started;
    long Index;
};
class CMaintainedEvent { public:                          // the list element: a word then the event
    long        m_0;
    CInputEvent m_event;                           // +0x04
};
struct SMaintainedEventNode {                      // retail (VC6-era Dinkumware) list node: next, prev, value
    SMaintainedEventNode* m_pNext;
    SMaintainedEventNode* m_pPrev;
    CMaintainedEvent      m_value;                 // +0x08 (the event at +0x0c)
};
class CInputManager;
class CKeyboard { public:                                 // retail layout: 256 stored events, the one-word list head at +0x340c
    virtual void v00();
    CInputEvent           EventStore[256];         // +0x04
    long                  NoStoredEvents;          // +0x3404
    long                  NoEventScansRunning;     // +0x3408
    SMaintainedEventNode* m_pMaintainedEvents;     // +0x340c list<CMaintainedEvent> head
    bool EventScanStart(CInputEventScanInfo& scan, CInputEvent& event);
    void AddMaintainedEventsToEventStore();
    void UpdateMaintainedEvents();
};

bool CKeyboard::EventScanStart(CInputEventScanInfo& scan, CInputEvent& event)
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