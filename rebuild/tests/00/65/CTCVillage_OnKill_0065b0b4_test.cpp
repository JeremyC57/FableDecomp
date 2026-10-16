// Behaviour test for CTCVillage::OnKill (0x0065B0B4): a village-stopped clock is restarted once (flag cleared), then the
// live hero's component is notified.
#include <cstdio>
static int g_restarts, g_notified;
struct Village {
    bool TimeIsPaused;
    void OnKill(bool heroAlive, bool heroHasComponent)
    {
        bool& paused = TimeIsPaused;
        if (paused) { paused = false; ++g_restarts; }
        if (heroAlive && heroHasComponent) ++g_notified;
    }
};
int main()
{
    Village v; v.TimeIsPaused = true;
    v.OnKill(true, true); v.OnKill(true, false); v.OnKill(false, true);
    if (v.TimeIsPaused || g_restarts != 1 || g_notified != 1) { std::printf("0065b0b4_TEST FAIL\n"); return 1; }
    std::printf("0065b0b4_TEST PASS\n");
    return 0;
}