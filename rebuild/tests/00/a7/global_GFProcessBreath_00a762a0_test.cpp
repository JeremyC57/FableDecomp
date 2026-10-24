// Behaviour test for 0x00A762A0 GFProcessBreath: the slot-4 handler runs (and the stored time is replaced by the
// current game time) only when stored - now >= 0.005; otherwise nothing changes.
#include <cstdio>
static double g_now = 0.0;
static double GFGetTime() { return g_now; }
class CProcess {
public:
    int calls;
    CProcess() : calls(0) { }
    virtual ~CProcess() { }
    virtual void SuspendableProcess() { ++calls; }
};
static void GFProcessBreath(CProcess& process, double& lastBreathTime)
{
    double now = GFGetTime();
    if (lastBreathTime - now >= 0.005) {
        process.SuspendableProcess();
        lastBreathTime = now;
    }
}
int main()
{
    CProcess p;
    double t = 10.0;
    g_now = 10.001;
    GFProcessBreath(p, t);
    if (p.calls != 0 || t != 10.0) { std::printf("00a762a0_TEST FAIL\n"); return 1; }
    g_now = 9.99;
    GFProcessBreath(p, t);
    if (p.calls != 1 || t != 9.99) { std::printf("00a762a0_TEST FAIL\n"); return 1; }
    g_now = 9.987;
    GFProcessBreath(p, t);
    if (p.calls != 1 || t != 9.99) { std::printf("00a762a0_TEST FAIL\n"); return 1; }
    std::printf("00a762a0_TEST PASS\n");
    return 0;
}