// Behaviour test for 0x009F54D0 (CInputManager::SetEventsFromEventPackage): the package's events become the store, positions go to the devices.
#include <cstdio>
#include <vector>
struct Vec2 { float x, y; };
struct Device { int calls; Vec2 last; Device() : calls(0) {} void SetRecordedPosition(const Vec2& p) { ++calls; last = p; } };
struct Package { std::vector<int> Events; Vec2 MousePos, JoystickPos; };
struct Manager {
    std::vector<int> ProcessedEvents, EventStore; int NoStoredEvents; Device* PMouse; std::vector<Device*> PJoysticks;
    void SetEventsFromEventPackage(const Package& p)
    {
        ProcessedEvents.clear(); EventStore = p.Events; NoStoredEvents = (int)EventStore.size();
        if (PMouse) PMouse->SetRecordedPosition(p.MousePos);
        if (PJoysticks[0]) PJoysticks[0]->SetRecordedPosition(p.JoystickPos);
    }
};
int main()
{
    Manager m; Device mouse, joy; m.PMouse = &mouse; m.PJoysticks.push_back(&joy); m.ProcessedEvents.push_back(9);
    Package p; p.Events.push_back(1); p.Events.push_back(2); p.Events.push_back(3); p.MousePos.x = 1; p.MousePos.y = 2; p.JoystickPos.x = 3; p.JoystickPos.y = 4;
    m.SetEventsFromEventPackage(p);
    int ok = m.ProcessedEvents.empty() && m.EventStore.size() == 3 && m.NoStoredEvents == 3 && mouse.last.y == 2 && joy.last.x == 3 && joy.calls == 1;
    m.PMouse = 0; m.PJoysticks[0] = 0; m.SetEventsFromEventPackage(p); ok = ok && mouse.calls == 1 && joy.calls == 1;
    if (!ok) { std::printf("009f54d0_TEST FAIL\n"); return 1; }
    std::printf("009f54d0_TEST PASS\n");
    return 0;
}
