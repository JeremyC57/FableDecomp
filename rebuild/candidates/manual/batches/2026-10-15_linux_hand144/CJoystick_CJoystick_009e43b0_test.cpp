// Behaviour test for 0x009e43b0 (CJoystick::CJoystick): the reconstructed logic run over a host model of its callees.
#include <cstdio>
#include <cstring>
#include <cmath>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct CInputEvent { long type; CInputEvent() : type(0) {} };
struct SNode { SNode* next; SNode* prev; unsigned char data[0x38]; };
struct CList { SNode* node; CList() : node(0) { SNode* n = new SNode; n->next = n; n->prev = n; node = n; } ~CList() { delete node; } };
struct CInputManager { int id; };
struct CJoystick {
    CInputEvent EventStore[64]; CInputManager* InputManager; bool ExclusiveMode; CList MaintainedEventsList; long NoStoredEvents; long NoEventScansRunning;
    CJoystick(CInputManager* m, bool e) : InputManager(m), NoStoredEvents(0), NoEventScansRunning(0) { ExclusiveMode = e; }
};
static void Run()
{
    CInputManager m; m.id = 1;
    CJoystick j(&m, true);
    CHECK(j.InputManager == &m && j.ExclusiveMode && j.NoStoredEvents == 0 && j.NoEventScansRunning == 0);
    CHECK(j.MaintainedEventsList.node != 0 && j.MaintainedEventsList.node->next == j.MaintainedEventsList.node && j.MaintainedEventsList.node->prev == j.MaintainedEventsList.node);
    CHECK(j.EventStore[63].type == 0);
    CJoystick k(&m, false); CHECK(!k.ExclusiveMode);
}

int main()
{
    Run();
    if (g_fail) { std::printf("009e43b0_TEST FAIL\n"); return 1; }
    std::printf("009e43b0_TEST PASS\n");
    return 0;
}
