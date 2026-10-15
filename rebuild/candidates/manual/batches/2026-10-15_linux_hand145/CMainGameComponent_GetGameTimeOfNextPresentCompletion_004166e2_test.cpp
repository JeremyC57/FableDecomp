// Behaviour test for 0x004166E2 (CMainGameComponent::GetGameTimeOfNextPresentCompletion): predicted present time clamped to now, as game time.
#include <cstdio>
struct Info { long VsyncCount; double LastVsyncTime; long PresentedVsyncCount; };
static Info g_info; static float g_period = 0.5f; static double g_now; static bool g_immediate;
struct Component {
    double m_gameTimeBase;
    double Get()
    {
        Info info = g_info;
        double t = (info.VsyncCount - info.PresentedVsyncCount + 1) * g_period + info.LastVsyncTime;
        double now = g_now;
        if (t < now) t = now;
        if (g_immediate) t = now;
        return t - m_gameTimeBase;
    }
};
int main()
{
    Component c; c.m_gameTimeBase = 100.0;
    g_info.VsyncCount = 10; g_info.PresentedVsyncCount = 8; g_info.LastVsyncTime = 200.0; g_now = 150.0;
    int ok = c.Get() == 101.5;                 // 200 + 3 * 0.5 - 100
    g_now = 300.0; ok &= c.Get() == 200.0;     // clamped to now
    g_now = 150.0; g_immediate = true; ok &= c.Get() == 50.0;
    if (!ok) { std::printf("004166e2_TEST FAIL\n"); return 1; }
    std::printf("004166e2_TEST PASS\n");
    return 0;
}
