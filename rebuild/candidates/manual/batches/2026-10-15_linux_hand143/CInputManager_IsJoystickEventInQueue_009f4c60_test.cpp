// Behaviour test for 0x009F4C60 (CInputManager::IsJoystickEventInQueue): first match by type/device (and button for kinds 0x13..0x15) is copied out.
#include <cstdio>
struct Event { unsigned char data[0x20]; int DeviceType, DeviceNumber, Type; float CurrentTime, StartTime;
    int GetType() const { return Type; } int GetDeviceNumber() const { return DeviceNumber; } int GetJoystickButton() const { return data[8]; } };
struct Manager {
    Event store[8]; int NoStoredEvents;
    bool IsJoystickEventInQueue(int type, int button, Event* event, int device)
    {
        for (int i = 0; i < NoStoredEvents; ++i)
            if (store[i].GetType() == type && store[i].GetDeviceNumber() == device)
                if ((type != 0x13 && type != 0x14 && type != 0x15) || store[i].GetJoystickButton() == button) { *event = store[i]; return true; }
        return false;
    }
};
static void Make(Event& e, int type, int device, int button, float t) { for (int i = 0; i < 0x20; ++i) e.data[i] = 0; e.Type = type; e.DeviceNumber = device; e.DeviceType = 2; e.data[8] = (unsigned char)button; e.CurrentTime = t; e.StartTime = 0; }
int main()
{
    Manager m; m.NoStoredEvents = 4;
    Make(m.store[0], 0x11, 1, 0, 1.0f); Make(m.store[1], 0x11, 0, 0, 2.0f); Make(m.store[2], 0x13, 0, 5, 3.0f); Make(m.store[3], 0x13, 0, 7, 4.0f);
    Event out; int ok = 1;
    ok = ok && m.IsJoystickEventInQueue(0x11, 0, &out, 0) && out.CurrentTime == 2.0f;
    ok = ok && m.IsJoystickEventInQueue(0x13, 7, &out, 0) && out.CurrentTime == 4.0f;
    ok = ok && !m.IsJoystickEventInQueue(0x13, 9, &out, 0) && !m.IsJoystickEventInQueue(0x12, 0, &out, 0);
    m.NoStoredEvents = 1; ok = ok && !m.IsJoystickEventInQueue(0x11, 0, &out, 0);
    if (!ok) { std::printf("009f4c60_TEST FAIL\n"); return 1; }
    std::printf("009f4c60_TEST PASS\n");
    return 0;
}
