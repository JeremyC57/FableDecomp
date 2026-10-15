// Behaviour test for 0x009143E0: ground-wander runnable when in the mode and idle long enough.
#include <cstdio>
static long g_frame = 0;
static long g_fps = 30;
static long g_mode = 5;
struct CModes { long mode; long GetCurrentModeType() { return mode; } };
struct CBird {
    CModes* modes; unsigned long FrameLastFinishedWalking;
    bool IsRunnable()
    {
        if (modes->GetCurrentModeType() == g_mode) {
            if (FrameLastFinishedWalking == 0) return true;
            long framesPerSecond = g_fps;
            if (g_frame - FrameLastFinishedWalking > (unsigned long)(framesPerSecond * 2)) return true;
        }
        return false;
    }
};
int main()
{
    CModes m = { 5 };
    CBird b = { &m, 0 };
    int bad = 0;
    g_frame = 10; if (!b.IsRunnable()) bad = 1;
    b.FrameLastFinishedWalking = 5; g_frame = 65; if (b.IsRunnable()) bad = 2;    // 60 elapsed: not > 60
    g_frame = 66; if (!b.IsRunnable()) bad = 3;
    m.mode = 4; if (b.IsRunnable()) bad = 4;
    if (bad) { std::printf("009143e0_TEST FAIL\n"); return 1; }
    std::printf("009143e0_TEST PASS\n");
    return 0;
}
