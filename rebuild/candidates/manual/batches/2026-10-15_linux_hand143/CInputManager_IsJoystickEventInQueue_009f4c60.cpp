// CInputManager::IsJoystickEventInQueue(EInputEventType type, long button, CInputEvent* event, long device) -- find the first stored event of this type from this device (for the button kinds 0x13..0x15 also with this joystick button) and copy it out; false when there is none
// Leaf: IsJoystickEventInQueue
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F4C60, length 133 bytes.
// Callees: CInputEvent::GetType @ 0x00A03B40, GetDeviceNumber @ 0x00A03B60, GetJoystickButton @ 0x00A03B90 (manifest labels
// "CWorld::DrawGetEnvironment" propagated onto these one-line getters).
// Shape: every element access is the STLport `*(begin() + i)` re-reading the vector's first pointer after each call; the
// found event is copied out with the implicit assignment (`rep movsd` x13: CInputEvent has a user copy constructor but no
// operator=), the element re-indexed (`imul esi,34h`).
// Layout: retail CInputManager (0x80 bytes, 12-byte vectors; GOTCHAS wave 24m): ProcessedEvents +0x04, EventStore +0x10,
// NoStoredEvents +0x1c, NoEventScansRunning +0x20, PMouse +0x24, PKeyboard +0x28, PJoysticks +0x2c (8-byte
// CCountedPointer<CJoystick> elements), Loading +0x38, Saving +0x39, Checksum +0x3c, LoadedEventPackages +0x4c.
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
    bool IsJoystickEventInQueue(long type, long button, CInputEvent* event, long device);
};

bool CInputManager::IsJoystickEventInQueue(long type, long button, CInputEvent* event, long device)
{
    for (long i = 0; i < NoStoredEvents; ++i) {
        if (EventStore[i].GetType() == type && EventStore[i].GetDeviceNumber() == device) {
            if ((type != 0x13 && type != 0x14 && type != 0x15) || EventStore[i].GetJoystickButton() == button) {
                *event = EventStore[i];
                return true;
            }
        }
    }
    return false;
}
