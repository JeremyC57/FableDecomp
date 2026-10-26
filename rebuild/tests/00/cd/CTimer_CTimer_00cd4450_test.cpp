#include <cstdio>
// Behaviour test for CTimer::CTimer (0x00CD4450): each timer takes a fresh index from the manager.
class CThingManager { public: long next; virtual long CreateTimer() { return next++; } };
static CThingManager g_mgr;
static CThingManager* g_pThingManager = &g_mgr;
class CTimer { public: long TimerIndex; CTimer() : TimerIndex(g_pThingManager->CreateTimer()) { } };
int main()
{
    g_mgr.next = 40;
    CTimer a, b;
    if (a.TimerIndex != 40 || b.TimerIndex != 41 || g_mgr.next != 42) { std::printf("00cd4450_TEST FAIL\n"); return 1; }
    std::printf("00cd4450_TEST PASS\n");
    return 0;
}