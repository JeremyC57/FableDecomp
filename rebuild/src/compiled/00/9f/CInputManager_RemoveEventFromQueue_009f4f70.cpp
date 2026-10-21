// CInputManager::RemoveEventFromQueue(const CInputEvent& event) -- find the first stored event that matches the given one (same type and device number; for the mouse the button of a button event, for the keyboard the character of a character event or else the key; any joystick event) and clear it
// Leaf: RemoveEventFromQueue
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F4F70, length 295 bytes (+ int3 padding), `ret 4`.
// Signature: engine_api.tsv __fastcall void (CInputManager*, CInputEvent*).
// Callees (CInputEvent's out-of-line accessors): GetType @ 0x00A03B40 (+0x28), GetDeviceType @ 0x00A03B50 (+0x20),
//   GetDeviceNumber @ 0x00A03B60 (+0x24), GetKey @ 0x00A03B70 (+0x00), GetChar @ 0x00A03B80 (+0x00, wchar_t),
//   GetButton @ 0x00A03B90 (+0x08, movzx), Clear @ 0x00A03BE0 (Type = DeviceType = 0). Catalog labels on these
//   (DrawGetEnvironment x3, GetCanItemBeQuickAssigned, CWADFile::Close) are propagated from byte-identical bodies.
// Layout (retail, 12-byte STLport vectors; Ego_r PDB names): ProcessedEvents +0x04, EventStore +0x10, NoStoredEvents +0x1c.
// Shape: the match test indexes EventStore[i] afresh for every accessor call (retail reloads the vector start each
//   time: `mov eax,[edi+10h] ; lea ecx,[ebx+eax]`, which needs operator[] written over an inline begin()), so it is
//   written in the loop body: type, then device number with the given event first (`cmp eax,ebp`), then a switch on
//   the given event's device type whose cases `goto found` on each comparison (given event first) -- a `same` flag is
//   materialised (`sete al`) -- and case 3 jumps straight to found (threaded into the shared `je` through the switch's
//   `dec eax` flags). The match (Clear) is laid after the loop's own epilogue and recomputes `i * 0x34`.
struct CInputEvent {
    unsigned char _body[0x34];
    long           GetType() const;                            // 0x00A03B40
    long           GetDeviceType() const;                      // 0x00A03B50
    long           GetDeviceNumber() const;                    // 0x00A03B60
    long           GetKey() const;                             // 0x00A03B70
    unsigned short GetChar() const;                     // 0x00A03B80 (wchar_t; VC7.1 without /Zc:wchar_t)
    long           GetButton() const;                          // 0x00A03B90
    void           Clear();                                    // 0x00A03BE0
};

template <class T>
struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};

struct CInputManager {
    void*               __vftable;                      // +0x00
    vector<CInputEvent> ProcessedEvents;                // +0x04
    vector<CInputEvent> EventStore;                     // +0x10
    long                NoStoredEvents;                 // +0x1c
    void RemoveEventFromQueue(const CInputEvent& event);
};

void CInputManager::RemoveEventFromQueue(const CInputEvent& event)
{
    long i;
    for (i = 0; i < NoStoredEvents; ++i) {
        if (EventStore[i].GetType() == event.GetType() && event.GetDeviceNumber() == EventStore[i].GetDeviceNumber()) {
            switch (event.GetDeviceType()) {
            case 1:
                if (event.GetType() == 0x13 || event.GetType() == 0x14 || event.GetType() == 0x15) {
                    if (event.GetButton() == EventStore[i].GetButton())
                        goto found;
                } else
                    goto found;
                break;
            case 2:
                if (event.GetType() == 0xf || event.GetType() == 0x10) {
                    if (event.GetChar() == EventStore[i].GetChar())
                        goto found;
                } else if (event.GetKey() == EventStore[i].GetKey())
                    goto found;
                break;
            case 3:
                goto found;
            }
        }
    }
    return;
found:
    EventStore[i].Clear();
}