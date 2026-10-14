#include <cstdio>
// Model of CAIStateGroup_SkillPractice::IsRunnable @ 0x0092B360: once a minute, runnable when a dummy exists.
static long g_frame, g_fps = 30; static bool g_dummy;
long GFGetFrameCount() { return g_frame; }
struct Group {
    long FrameToCheckForDummy;
    bool GetDummy() { return g_dummy; }
    bool IsRunnable()
    {
        if (FrameToCheckForDummy < GFGetFrameCount()) {
            if (GetDummy()) return true;
            long fps = g_fps;
            FrameToCheckForDummy = GFGetFrameCount() + fps * 60;
        }
        return false;
    }
};
int main()
{
    Group g; g.FrameToCheckForDummy = 100; g_frame = 100; g_dummy = true;
    if (g.IsRunnable()) { std::printf("0092b360_TEST FAIL\n"); return 1; }           // not yet due
    g_frame = 101;
    if (!g.IsRunnable() || g.FrameToCheckForDummy != 100) { std::printf("0092b360_TEST FAIL\n"); return 1; }
    g_dummy = false;
    if (g.IsRunnable() || g.FrameToCheckForDummy != 101 + 30 * 60) { std::printf("0092b360_TEST FAIL\n"); return 1; }
    std::printf("0092b360_TEST PASS\n"); return 0;
}
