// Behaviour test for CInputManager::IsEventInQueue (0x009F4D40): matching per device type.
#include <cstdio>
#include <string.h>
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
    m.EventStore.m_first = store; m.EventStore.m_last = store + 6; m.EventStore.m_end = store + 6; m.NoStoredEvents = 6;
    m.ProcessedEvents.m_first = m.ProcessedEvents.m_last = 0;
    int bad = 0;
    if (!m.IsEventInQueue(Ev(1, 0x13, 2, 0, 6))) bad = 1;          // joystick: type/device only
    if (m.IsEventInQueue(Ev(1, 0x13, 3, 0, 5))) bad = 2;
    if (!m.IsEventInQueue(Ev(2, 0x0f, 1, 'a', 0))) bad = 4;
    if (m.IsEventInQueue(Ev(2, 0x0f, 1, 'b', 0))) bad = 5;
    if (!m.IsEventInQueue(Ev(2, 0x07, 1, 0x41, 0))) bad = 6;
    if (m.IsEventInQueue(Ev(2, 0x07, 1, 0x42, 0))) bad = 7;
    if (!m.IsEventInQueue(Ev(3, 0x22, 0, 0, 0))) bad = 8;
    if (m.IsEventInQueue(Ev(4, 0x22, 0, 0, 0))) bad = 9;
    m.NoStoredEvents = 2;
    if (m.IsEventInQueue(Ev(3, 0x22, 0, 0, 0))) bad = 10;          // only the first two are searched
    if (bad) { std::printf("009f4d40_TEST FAIL\n"); return 1; }
    std::printf("009f4d40_TEST PASS\n");
    return 0;
}
