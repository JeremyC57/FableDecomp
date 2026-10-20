// Grade: DIFFER(304v295) -- element address `mov ecx,[edi+10h]; add ecx,ebx` (retail `mov eax,[edi+10h]; lea ecx,[ebx+eax]`), device-number compare operands swapped, and the match block (Clear + return) laid out inside the loop instead of after it with the device-type-3 case threaded into its je
// CInputManager::RemoveEventFromQueue(const CInputEvent& event) -- find the first stored event that matches the given one (same type and device number; for the mouse the button of a button event, for the keyboard the character of a character event or else the key; any joystick event) and clear it
// Leaf: RemoveEventFromQueue
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F4F70, length 295 bytes (+ int3 padding), `ret 4`.
// Signature: engine_api.tsv __thiscall void (CInputManager*, CInputEvent*).
// Callees (CInputEvent's out-of-line accessors): GetType @ 0x00A03B40 (+0x28), GetDeviceType @ 0x00A03B50 (+0x20),
//   GetDeviceNumber @ 0x00A03B60 (+0x24), GetKey @ 0x00A03B70 (+0x00), GetChar @ 0x00A03B80 (+0x00, wchar_t),
//   GetButton @ 0x00A03B90 (+0x08, movzx), Clear @ 0x00A03BE0 (Type = DeviceType = 0). Catalog labels on these
//   (DrawGetEnvironment x3, GetCanItemBeQuickAssigned, CWADFile::Close) are propagated from byte-identical bodies.
// Layout (retail, 12-byte STLport vectors; Ego_r PDB names): ProcessedEvents +0x04, EventStore +0x10, NoStoredEvents +0x1c.
// Shape: the match test indexes EventStore[i] afresh for every accessor call (retail reloads the vector start each
//   time), so it is written in the loop body: type and device number, then a `same` flag from a switch on the given
//   event's device type.
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
    T& operator[](unsigned int n) { return *(_M_start + n); }
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
    for (long i = 0; i < NoStoredEvents; ++i) {
        if (EventStore[i].GetType() == event.GetType() && EventStore[i].GetDeviceNumber() == event.GetDeviceNumber()) {
            bool same;
            switch (event.GetDeviceType()) {
            case 1:
                same = true;
                if (event.GetType() == 0x13 || event.GetType() == 0x14 || event.GetType() == 0x15)
                    same = EventStore[i].GetButton() == event.GetButton();
                break;
            case 2:
                if (event.GetType() == 0xf || event.GetType() == 0x10)
                    same = EventStore[i].GetChar() == event.GetChar();
                else
                    same = EventStore[i].GetKey() == event.GetKey();
                break;
            case 3:
                same = true;
                break;
            default:
                same = false;
                break;
            }
            if (same) {
                EventStore[i].Clear();
                return;
            }
        }
    }
}
