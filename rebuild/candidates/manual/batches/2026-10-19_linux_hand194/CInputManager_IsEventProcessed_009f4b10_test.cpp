// Behaviour test for CInputManager::IsEventProcessed (0x009F4B10): matching per device type.
#include <cstdio>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
#ifndef _MSC_VER
#define __int64 long long
#endif
class CInputEvent {                                // 0x34 bytes (PDB): Data +0, DeviceType +0x20, DeviceNumber +0x24, Type +0x28
public:
    unsigned char _data[0x20];
    long          DeviceType;                      // +0x20
    long          DeviceNumber;                    // +0x24
    long          Type;                            // +0x28
    float         CurrentTime;                     // +0x2c
    float         StartTime;                       // +0x30
    long GetType() const { return Type; }
    long GetDeviceType() const { return DeviceType; }
    long GetDeviceNumber() const { return DeviceNumber; }
    long GetKey() const { return *(const long*)_data; }
    unsigned short GetChar() const { return *(const unsigned short*)_data; }
    long GetJoystickButton() const { return _data[8]; }
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
    virtual void v00() {}
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

static CInputEvent Ev(long deviceType, long type, long device, long key, unsigned char button)
{
    CInputEvent e; memset(&e, 0, sizeof(e));
    e.DeviceType = deviceType; e.Type = type; e.DeviceNumber = device;
    memcpy(e._data, &key, 4); e._data[8] = button;
    return e;
}
int main()
{
    static CInputEvent store[6];
    store[0] = Ev(1, 0x13, 2, 0, 5); store[1] = Ev(1, 0x11, 2, 0, 1); store[2] = Ev(2, 0x0f, 1, 'a', 0);
    store[3] = Ev(2, 0x07, 1, 0x41, 0); store[4] = Ev(3, 0x22, 0, 0, 0); store[5] = Ev(4, 0x22, 0, 0, 0);
    CInputManager m;
    m.ProcessedEvents.m_first = store; m.ProcessedEvents.m_last = store + 6; m.ProcessedEvents.m_end = store + 6;
    m.EventStore.m_first = 0; m.NoStoredEvents = 0;
    int bad = 0;
    // joystick button kind: the button must agree
    if (!m.IsEventProcessed(Ev(1, 0x13, 2, 0, 5))) bad = 1;
    if (m.IsEventProcessed(Ev(1, 0x13, 2, 0, 6))) bad = 2;
    if (!m.IsEventProcessed(Ev(1, 0x11, 2, 0, 9))) bad = 3;      // other joystick kinds match on type/device
    if (!m.IsEventProcessed(Ev(2, 0x0f, 1, 'a', 0))) bad = 4;
    if (m.IsEventProcessed(Ev(2, 0x0f, 1, 'b', 0))) bad = 5;
    if (!m.IsEventProcessed(Ev(2, 0x07, 1, 0x41, 0))) bad = 6;
    if (m.IsEventProcessed(Ev(2, 0x07, 1, 0x42, 0))) bad = 7;
    if (!m.IsEventProcessed(Ev(3, 0x22, 0, 0, 0))) bad = 8;
    if (m.IsEventProcessed(Ev(4, 0x22, 0, 0, 0))) bad = 9;         // unknown device type
    if (m.IsEventProcessed(Ev(3, 0x23, 0, 0, 0))) bad = 10;        // no stored event of that type
    if (bad) { std::printf("009f4b10_TEST FAIL\n"); return 1; }
    std::printf("009f4b10_TEST PASS\n");
    return 0;
}
