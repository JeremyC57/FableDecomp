#include <cstdio>
long g_frame = 100;
static long GFGetCurrentFrame() { return g_frame; }
struct CGroup { long FrameToSitDown; bool baseRunnable; int baseCalls; CGroup() : FrameToSitDown(100), baseRunnable(true), baseCalls(0) { }
    bool BaseIsRunnable() { ++baseCalls; return baseRunnable; }
    bool IsRunnable() { return GFGetCurrentFrame() > FrameToSitDown && BaseIsRunnable(); } };
int main()
{
    CGroup g; bool ok = !g.IsRunnable() && g.baseCalls == 0;   // not past the frame: base not asked
    g_frame = 101; ok = ok && g.IsRunnable() && g.baseCalls == 1;
    g.baseRunnable = false; ok = ok && !g.IsRunnable() && g.baseCalls == 2;
    if (!ok) { std::printf("009104a0_TEST FAIL\n"); return 1; }
    std::printf("009104a0_TEST PASS\n"); return 0;
}
