// CMouse::EventScanStart(CInputEventScanInfo& scan, CInputEvent& event) -- mark the scan started and count a running scan (+0x3408); when events are stored (+0x3404 > 0) the scan count is bumped again, the scan index is 0 and the first stored event (+4) is copied out (13 dwords), returning true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A66B70, length 74 bytes.
// No callees. engine/CMouse.h: EventStore +0x04 (CInputEvent[256]), NoStoredEvents +0x3404, NoEventScansRunning +0x3408. Shape: the scan pointer in eax before the saves, the counter incremented twice on the stored path (retail stores both), `rep movsd` through esi/edi, `mov al,1` / `xor al,al` tails, `ret 8`.
struct CInputEvent {                               // 0x34 bytes (PDB)
    unsigned char _body[0x34];
};
struct CInputEventScanInfo {                       // PDB: bool Started +0, long Index +4
    bool Started;
    long Index;
};
struct CInputManager;

struct CMouse {                                    // engine/CMouse.h layout
    virtual bool EventScanStart(CInputEventScanInfo& scan, CInputEvent& event);   // slot 0
    CInputEvent    EventStore[256];                // +0x0004
    long           NoStoredEvents;                 // +0x3404
    long           NoEventScansRunning;            // +0x3408
    unsigned char  MaintainedEventsList[0xc];      // +0x340c
    CInputManager* InputManager;                   // +0x3418
};

bool CMouse::EventScanStart(CInputEventScanInfo& scan, CInputEvent& event)
{
    scan.Started = true;
    ++NoEventScansRunning;
    if (NoStoredEvents > 0) {
        ++NoEventScansRunning;
        scan.Index = 0;
        event = EventStore[0];
        return true;
    }
    return false;
}