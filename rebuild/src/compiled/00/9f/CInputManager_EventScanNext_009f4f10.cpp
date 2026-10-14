// CInputManager::EventScanNext(CInputEventScanInfo& scan, CInputEvent& event) -- advance the scan past its current index to the next stored event whose type is not NONE; record the index, copy the event out and return true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F4F10, length 95 bytes (+ int3 padding).
// Callees: CInputEvent::GetType @ 0x00A03B40 (the kind word at +0x28; a zero kind is an empty slot).
// CInputEventScanInfo (PDB): bool Started +0, long Index +4; EventStore.first +0x10, NoStoredEvents +0x1c; events are 0x34 bytes.
// Shape: the index starts at scan.Index + 1 (edi); the element pointer is a named local (esi = first + i*0x34, the offset
// strength-reduced in ebx, the loop head padded with `lea ebx,[ebx]`) reused by the 13-dword copy on the found path; the
// count is re-read after each call; `xor al,al` / `mov al,1` tails.
struct CInputEvent {                                // 0x34 bytes
    unsigned char _body[0x34];
    long GetType() const;                           // 0x00A03B40
};
struct CInputEventScanInfo {
    bool Started;                                   // +0x00
    long Index;                                     // +0x04
};

struct CInputManager {
    virtual void v00();
    unsigned char _pad04[0x0c];                     // +0x04 ProcessedEvents
    CInputEvent*  m_pEventStoreFirst;               // +0x10 EventStore.first
    CInputEvent*  m_pEventStoreLast;                // +0x14
    CInputEvent*  m_pEventStoreEnd;                 // +0x18
    long          NoStoredEvents;                   // +0x1c
    long          NoEventScansRunning;              // +0x20
    bool EventScanNext(CInputEventScanInfo& scan, CInputEvent& event);
};

bool CInputManager::EventScanNext(CInputEventScanInfo& scan, CInputEvent& event)
{
    for (long i = scan.Index + 1; i < NoStoredEvents; ++i) {
        CInputEvent* stored = &m_pEventStoreFirst[i];
        if (stored->GetType() != 0) {
            scan.Index = i;
            event = *stored;
            return true;
        }
    }
    return false;
}