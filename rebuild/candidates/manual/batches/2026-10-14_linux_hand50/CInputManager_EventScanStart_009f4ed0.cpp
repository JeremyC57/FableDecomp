// CInputManager::EventScanStart(CInputEventScanInfo& scan, CInputEvent& event) -- mark the scan started, count it in NoEventScansRunning (+0x20); when events are stored (+0x1c) point the scan at index 0, copy out the first stored event (EventStore.first, +0x10) and return true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F4ED0, length 56 bytes (+ int3 padding). No callees.
// CInputEventScanInfo (PDB, 8 bytes): bool Started +0, long Index +4. CInputEvent is 0x34 bytes (13-dword `rep movsd`).
// The first event is read through the retail vector iterator object (`*EventStore.begin()`): a raw `first[0]` swaps the
// `this`/scan registers (eax/ecx) in the lane.
// Shape: the Started byte is stored first, NoStoredEvents is loaded before the counter's increment is stored back, the
// found path clears Index through the scan pointer kept in eax, `mov al,1` / `xor al,al` tails.
struct CInputEvent { unsigned char _body[0x34]; };
template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    struct iterator { T* p; explicit iterator(T* q) : p(q) {} T& operator*() const { return *p; } };
    iterator begin() { return iterator(first); }
};
struct CInputEventScanInfo {
    bool Started;                                   // +0x00
    long Index;                                     // +0x04
};

struct CInputManager {
    virtual void v00();
    unsigned char _pad04[0x0c];                     // +0x04 ProcessedEvents
    CRetailVector<CInputEvent> EventStore;          // +0x10
    long          NoStoredEvents;                   // +0x1c
    long          NoEventScansRunning;              // +0x20
    bool EventScanStart(CInputEventScanInfo& scan, CInputEvent& event);
};

bool CInputManager::EventScanStart(CInputEventScanInfo& scan, CInputEvent& event)
{
    scan.Started = true;
    ++NoEventScansRunning;
    if (NoStoredEvents > 0) {
        scan.Index = 0;
        event = *EventStore.begin();
        return true;
    }
    return false;
}
