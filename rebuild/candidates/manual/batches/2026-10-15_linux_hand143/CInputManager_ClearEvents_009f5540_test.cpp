// Behaviour test for 0x009F5540 (CInputManager::ClearEvents): both event vectors emptied, count zeroed, devices cleared.
#include <cstdio>
#include <vector>
struct Joystick { int resets, clears; Joystick() : resets(0), clears(0) {} void ResetState() { ++resets; } void Clear() { ++clears; } };
struct Device { int clears; Device() : clears(0) {} void Clear() { ++clears; } };
struct Manager {
    std::vector<int> ProcessedEvents, EventStore; int NoStoredEvents; Device* PMouse; Device* PKeyboard; std::vector<Joystick*> PJoysticks;
    void ClearEvents()
    {
        ProcessedEvents.clear(); EventStore.clear(); NoStoredEvents = 0;
        long count = (long)PJoysticks.size();
        for (long i = 0; i < count; ++i) if (PJoysticks[i]) { PJoysticks[i]->ResetState(); PJoysticks[i]->Clear(); }
        if (PMouse) PMouse->Clear();
        if (PKeyboard) PKeyboard->Clear();
    }
};
int main()
{
    Manager m; Joystick j; Device mouse; m.ProcessedEvents.push_back(1); m.EventStore.push_back(2); m.NoStoredEvents = 1;
    m.PMouse = &mouse; m.PKeyboard = 0; m.PJoysticks.push_back(&j); m.PJoysticks.push_back(0);
    m.ClearEvents();
    int ok = m.ProcessedEvents.empty() && m.EventStore.empty() && m.NoStoredEvents == 0 && j.resets == 1 && j.clears == 1 && mouse.clears == 1;
    if (!ok) { std::printf("009f5540_TEST FAIL\n"); return 1; }
    std::printf("009f5540_TEST PASS\n");
    return 0;
}
