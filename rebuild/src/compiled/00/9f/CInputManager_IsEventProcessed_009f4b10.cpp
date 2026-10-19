// CInputManager::IsEventProcessed(const CInputEvent& event) -- true when one of the already processed events is the same input as `event`
// Leaf: IsEventProcessed
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F4B10, 240 bytes, `ret 4`.
// Callees: CInputEvent::GetType @ 0x00A03B40, GetDeviceType @ 0x00A03B50, GetDeviceNumber @ 0x00A03B60, GetKey @
//   0x00A03B70, GetChar @ 0x00A03B80, GetJoystickButton @ 0x00A03B90 (the first three carry the propagated label
//   "CWorld::DrawGetEnvironment", the last "GetCanItemBeQuickAssigned"; the landed IsJoystickEventInQueue 0x009F4C60
//   names them).
// Layout: retail CInputManager (0x80 bytes, 12-byte vectors; GOTCHAS wave 24m): ProcessedEvents +0x04, EventStore +0x10,
//   NoStoredEvents +0x1c; CInputEvent (Ego_r.pdb, 0x34 bytes).
// Shape: a pointer walk over ProcessedEvents with the end re-read every pass; the inline predicate tests type and
//   device number (`stored != event`), then switches on the device type: the per-type compares are written
//   `event.X() == stored.X()` as `if (...) return true; return false;` statements (a `return a == b` materialises
//   sete), and `case 3: return true;` shares one `dec eax ; je` with the default.
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
// Two events describe the same input: same type and device number, and per device type the same joystick button (for
// the button kinds 0x13..0x15), the same character (char kinds 0x0f/0x10) or key, any mouse event; else no match.
__forceinline bool IsSameInput(const CInputEvent& stored, const CInputEvent& event)
{
    if (stored.GetType() != event.GetType())
        return false;
    if (stored.GetDeviceNumber() != event.GetDeviceNumber())
        return false;
    switch (event.GetDeviceType()) {
    case 1:
        if (event.GetType() == 0x13 || event.GetType() == 0x14 || event.GetType() == 0x15) {
            if (event.GetJoystickButton() == stored.GetJoystickButton())
                return true;
            return false;
        }
        return true;
    case 2:
        if (event.GetType() == 0x0f || event.GetType() == 0x10) {
            if (event.GetChar() == stored.GetChar())
                return true;
            return false;
        }
        if (event.GetKey() == stored.GetKey())
            return true;
        return false;
    case 3:
        return true;
    }
    return false;
}
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
    bool IsEventProcessed(const CInputEvent& event);
};

bool CInputManager::IsEventProcessed(const CInputEvent& event)
{
    for (CInputEvent* it = ProcessedEvents.m_first; it != ProcessedEvents.m_last; ++it)
        if (IsSameInput(*it, event))
            return true;
    return false;
}