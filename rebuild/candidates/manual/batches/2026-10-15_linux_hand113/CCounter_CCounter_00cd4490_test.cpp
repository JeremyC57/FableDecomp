#include <cstdio>
// Behaviour test for CCounter::CCounter (0x00CD4490): the counter starts at the current world frame.
struct CThingManager { long frame; virtual long GetWorldFrame() { return frame; } };
static CThingManager g_mgr;
static CThingManager* g_pThingManager = &g_mgr;
struct CCounter { long RelevantWorldFrame; CCounter() : RelevantWorldFrame(g_pThingManager->GetWorldFrame()) { } };
int main()
{
    g_mgr.frame = 1234;
    CCounter c;
    g_mgr.frame = 9;
    CCounter d;
    if (c.RelevantWorldFrame != 1234 || d.RelevantWorldFrame != 9) { std::printf("00cd4490_TEST FAIL\n"); return 1; }
    std::printf("00cd4490_TEST PASS\n");
    return 0;
}
