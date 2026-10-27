// Behaviour test for 0x009054F0 NBalverineCombat::SetLungedThisFrame(): stores the current world frame as the last lunge frame.
#include <cstdio>

static long g_frame = 0;
static long g_lastLungeFrame = -1;
static long GetFrame() { return g_frame; }
static void SetLungedThisFrame() { g_lastLungeFrame = GetFrame(); }

int main()
{
    g_frame = 1234;
    SetLungedThisFrame();
    if (g_lastLungeFrame != 1234) { std::printf("009054f0_TEST FAIL\n"); return 1; }
    g_frame = 1240;
    SetLungedThisFrame();
    if (g_lastLungeFrame != 1240) { std::printf("009054f0_TEST FAIL\n"); return 1; }
    std::printf("009054f0_TEST PASS\n");
    return 0;
}