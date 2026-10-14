#include <cstdio>
// Model of CAIStateGroup_WanderAroundShops::Cleanup @ 0x00946DD0: NextFrameBuy = (long)(frame + fps * delay).
static long g_fps = 30, g_frame = 1000; static float g_delay = 2.5f;
long GFGetFrameCount() { return g_frame; }
struct Group {
    long NextFrameBuy;
    void Cleanup() { long fps = g_fps; NextFrameBuy = (long)(GFGetFrameCount() + fps * g_delay); }
};
int main()
{
    Group g; g.NextFrameBuy = 0; g.Cleanup();
    if (g.NextFrameBuy != 1075) { std::printf("00946dd0_TEST FAIL\n"); return 1; }
    g_fps = 60; g_frame = 10; g_delay = 0.5f; g.Cleanup();
    if (g.NextFrameBuy != 40) { std::printf("00946dd0_TEST FAIL\n"); return 1; }
    std::printf("00946dd0_TEST PASS\n"); return 0;
}
