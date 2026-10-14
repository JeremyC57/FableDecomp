#include <cstdio>
long g_FramesPerSecond = 15; long g_frame = 1000;
static long GFGetCurrentFrame() { return g_frame; }
struct CGroup { int baseCalls; long FrameToSitDown; CGroup() : baseCalls(0), FrameToSitDown(0) { }
    void BaseOnReplace() { ++baseCalls; }
    void OnReplace() { BaseOnReplace(); long fps = g_FramesPerSecond; FrameToSitDown = GFGetCurrentFrame() + fps * 80; } };
int main()
{
    CGroup g; g.OnReplace(); bool ok = g.baseCalls == 1 && g.FrameToSitDown == 1000 + 1200;
    g_FramesPerSecond = 30; g_frame = 7; g.OnReplace(); ok = ok && g.FrameToSitDown == 7 + 2400;
    if (!ok) { std::printf("0090f710_TEST FAIL\n"); return 1; }
    std::printf("0090f710_TEST PASS\n"); return 0;
}