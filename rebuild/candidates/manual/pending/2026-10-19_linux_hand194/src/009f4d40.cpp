// CInputManager::IsEventInQueue(const CInputEvent& event) -- true when a stored event has the same type and device number as `event` and, for keyboard events, the same character (char kinds 0x0f/0x10) or key
// Leaf: IsEventInQueue
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F4D40, 213 bytes, `ret 4`.
// Callees: CInputEvent::GetType @ 0x00A03B40, GetDeviceType @ 0x00A03B50, GetDeviceNumber @ 0x00A03B60, GetKey @
//   0x00A03B70, GetChar @ 0x00A03B80, GetJoystickButton @ 0x00A03B90 (the first three carry the propagated label
//   "CWorld::DrawGetEnvironment", the last "GetCanItemBeQuickAssigned"; the landed IsJoystickEventInQueue 0x009F4C60
//   names them).
// Layout: retail CInputManager (0x80 bytes, 12-byte vectors; GOTCHAS wave 24m): ProcessedEvents +0x04, EventStore +0x10,
//   NoStoredEvents +0x1c; CInputEvent (Ego_r.pdb, 0x34 bytes).
// Shape: the stored element is the STLport `*(begin() + i)` re-reading the vector's first pointer at every access (the
//   byte offset strength-reduced in ebx, the index in the frame), so the test is written in the loop body (a predicate
//   taking the element by reference caches it); joystick and mouse events match on type and device alone; keyboard
//   events compare the character (kinds 0x0f/0x10) or the key; `case 3: return true;` shares one `dec eax ; je` with the
//   default.
class CInputEvent {                                // 0x34 bytes (PDB): Data +0, DeviceType +0x20, DeviceNumber +0x24, Type +0x28
public:
    unsigned char _data[0x20];
    long          DeviceType;                      // +0x20
    long          DeviceNumber;                    // +0x24
    long          Type;                            // +0x28
    float         CurrentTime;                     // +0x2c
    float         StartTime;                       // +0x30
    long GetType() const;                          // 0x00A03B40
    long GetDeviceType() const;                    // 0x00A03B50
    long GetDeviceNumber() const;                  // 0x00A03B60
    long GetKey() const;                           // 0x00A03B70 (the dword at +0)
    unsigned short GetChar() const;                // 0x00A03B80 (the wchar_t at +0)
    long GetJoystickButton() const;                // 0x00A03B90 (movzx of the byte at +8)
};
template <class T> struct CRetailVector {          // STLport vector: first / last / end
    T* m_first; T* m_last; T* m_end;
    T* begin() { return m_first; }
    T& operator[](long n) { return *(begin() + n); }
};
class CInputManager {
public:
    virtual void v00();
    CRetailVector<CInputEvent> ProcessedEvents;    // +0x04
    CRetailVector<CInputEvent> EventStore;         // +0x10
    long                       NoStoredEvents;     // +0x1c
    bool IsEventInQueue(const CInputEvent& event);
};

bool CInputManager::IsEventInQueue(const CInputEvent& event)
{
    for (long i = 0; i < NoStoredEvents; ++i) {
        if (EventStore[i].GetType() == event.GetType() && EventStore[i].GetDeviceNumber() == event.GetDeviceNumber()) {
            switch (event.GetDeviceType()) {
            case 1:
                return true;
            case 2:
                if (event.GetType() == 0x0f || event.GetType() == 0x10) {
                    if (event.GetChar() == EventStore[i].GetChar())
                        return true;
                } else {
                    if (event.GetKey() == EventStore[i].GetKey())
                        return true;
                }
                break;
            case 3:
                return true;
            }
        }
    }
    return false;
}
