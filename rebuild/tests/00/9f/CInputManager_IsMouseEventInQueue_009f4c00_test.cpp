#include <cstdio>
// Behaviour test for CInputManager::IsMouseEventInQueue (0x009F4C00): first stored event of the type is copied out.
enum EInputEventType { EIET_NONE, EIET_A = 3, EIET_B = 7 };
class CInputEvent {
public:
    unsigned char Data[0x20];
    int DeviceType, DeviceNumber;
    EInputEventType Type;
    float CurrentTime, StartTime;
    EInputEventType GetType() const { return Type; }
};
struct Vec { CInputEvent* _M_start; CInputEvent* _M_finish; CInputEvent* _M_eos;
    const CInputEvent& operator[](unsigned int n) const { return *(_M_start + n); } };
class CInputManager {
public:
    Vec ProcessedEvents, EventStore;
    int NoStoredEvents;
    bool IsMouseEventInQueue(EInputEventType type, CInputEvent& event) const
    {
        for (long i = 0; i < NoStoredEvents; ++i) {
            if (EventStore[i].GetType() == type) { event = EventStore[i]; return true; }
        }
        return false;
    }
};
int main()
{
    CInputEvent ev[3];
    for (int i = 0; i < 3; ++i) { ev[i].DeviceNumber = i; ev[i].Type = EIET_NONE; ev[i].CurrentTime = 0; ev[i].StartTime = 0; }
    ev[1].Type = EIET_A; ev[2].Type = EIET_A;
    CInputManager m; m.EventStore._M_start = ev; m.NoStoredEvents = 3;
    CInputEvent out; out.DeviceNumber = -1;
    bool ok = m.IsMouseEventInQueue(EIET_A, out) && out.DeviceNumber == 1;
    CInputEvent out2; out2.DeviceNumber = 99;
    ok = ok && !m.IsMouseEventInQueue(EIET_B, out2) && out2.DeviceNumber == 99;
    m.NoStoredEvents = 1;
    ok = ok && !m.IsMouseEventInQueue(EIET_A, out2);
    if (!ok) { std::printf("009f4c00_TEST FAIL\n"); return 1; }
    std::printf("009f4c00_TEST PASS\n");
    return 0;
}