// Behaviour test for 0x008530B0 (camera module ctor over CScriptModuleSelfTerminateBase): duration, end frame
// and the two tracked things.
#include <cstdio>
static long g_frame = 50;
long GFGetCurrentFrame() { return g_frame; }
struct CThing { int id; };
struct IP { const CThing* item; IP(const CThing* t) : item(t) {} };
struct SelfTerminate {
    long WFDuration; long EndFrame;
    SelfTerminate(long duration) : WFDuration(duration) { EndFrame = GFGetCurrentFrame() + WFDuration; }
};
struct Module : SelfTerminate {
    IP PCreature; IP PTarget;
    Module(const CThing* creature, const CThing* target, long duration) : SelfTerminate(duration), PCreature(creature), PTarget(target) {}
};
int main()
{
    CThing a, b;
    Module m(&a, &b, 30);
    if (m.WFDuration != 30 || m.EndFrame != 80 || m.PCreature.item != &a || m.PTarget.item != &b) { std::printf("008530b0_TEST FAIL\n"); return 1; }
    std::printf("008530b0_TEST PASS\n");
    return 0;
}
