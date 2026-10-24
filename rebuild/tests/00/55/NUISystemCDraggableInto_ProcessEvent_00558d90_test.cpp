// Behaviour test for 0x00558D90 NUISystem::CDraggableInto::ProcessEvent
#include <cstdio>
struct CFrontEnd { void* PDraggedComponent; };
static CFrontEnd g_frontEnd;
struct CDraggableInto {
    bool WasHovered; int* PActions; int executed; int baseEvents; int lastEvent;
    void ExecuteActions(int* actions) { executed += *actions; }
    void BaseProcessEvent(int event) { ++baseEvents; lastEvent = event; }
    void ProcessEvent(int event)
    {
        if (event == 0x1e && WasHovered) {
            ExecuteActions(PActions);
            g_frontEnd.PDraggedComponent = 0;
        }
        BaseProcessEvent(event);
    }
};
int main()
{
    int actions = 3; int dummy = 0;
    CDraggableInto d; d.WasHovered = true; d.PActions = &actions; d.executed = 0; d.baseEvents = 0; d.lastEvent = 0;
    g_frontEnd.PDraggedComponent = &dummy;
    d.ProcessEvent(0x19);
    int ok = d.executed == 0 && d.baseEvents == 1 && g_frontEnd.PDraggedComponent == &dummy;
    d.ProcessEvent(0x1e);
    ok = ok && d.executed == 3 && d.baseEvents == 2 && d.lastEvent == 0x1e && g_frontEnd.PDraggedComponent == 0;
    g_frontEnd.PDraggedComponent = &dummy; d.WasHovered = false;
    d.ProcessEvent(0x1e);
    ok = ok && d.executed == 3 && d.baseEvents == 3 && g_frontEnd.PDraggedComponent == &dummy;
    if (!ok) { std::printf("00558d90_TEST FAIL\n"); return 1; }
    std::printf("00558d90_TEST PASS\n");
    return 0;
}