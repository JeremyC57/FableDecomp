#include <cstdio>
static long g_frame = 0;
static long g_FramesPerSecond = 15;
static long GetWorldFrame() { return g_frame; }
struct CTCWife {
    long FrameGotMarriedToThePlayer;
    float GetNumHoursMarriedFor() const
    {
        float hours = 0.0f;
        if (FrameGotMarriedToThePlayer <= GetWorldFrame())
            hours = (float)(GetWorldFrame() - FrameGotMarriedToThePlayer) / (g_FramesPerSecond * 3600.0f);
        return hours;
    }
};
int main()
{
    CTCWife w; w.FrameGotMarriedToThePlayer = 1000; g_frame = 500;
    if (w.GetNumHoursMarriedFor() != 0.0f) { std::printf("006cec90_TEST FAIL\n"); return 1; }
    g_frame = 1000 + 15 * 3600 * 2;
    float h = w.GetNumHoursMarriedFor(); if (h < 1.999f || h > 2.001f) { std::printf("006cec90_TEST FAIL\n"); return 1; }
    std::printf("006cec90_TEST PASS\n");
    return 0;
}