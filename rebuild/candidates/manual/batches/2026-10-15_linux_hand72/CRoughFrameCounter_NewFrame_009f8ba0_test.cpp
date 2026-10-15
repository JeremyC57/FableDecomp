#include <cstdio>
// Behaviour model of 0x009F8BA0: FPS from the elapsed milliseconds (timeGetTime stubbed).
// (unsigned int stands for the 32-bit unsigned long of the retail build so the wrap-around case also holds on a 64-bit host.)
static unsigned int g_now = 0;
static unsigned int FakeTimeGetTime() { return g_now; }
struct CRoughFrameCounter {
    unsigned int FrameStart; float FPS;
    void NewFrame()
    {
        unsigned int now = FakeTimeGetTime();
        unsigned int elapsed = now - FrameStart;
        if (elapsed != 0)
            FPS = (float)(1000.0 / (float)elapsed);
        else
            FPS = 0.0f;
        FrameStart = now;
    }
};
int main()
{
    CRoughFrameCounter c; c.FrameStart = 1000; c.FPS = 99.0f;
    g_now = 1040; c.NewFrame();
    if (c.FrameStart != 1040 || c.FPS != 25.0f) { std::printf("009f8ba0_TEST FAIL\n"); return 1; }
    c.NewFrame();                              // no time passed
    if (c.FrameStart != 1040 || c.FPS != 0.0f) { std::printf("009f8ba0_TEST FAIL\n"); return 1; }
    c.FrameStart = 0xFFFFFFF0U; g_now = 0x10;  // wrap-around: 32 ms
    c.NewFrame();
    if (c.FrameStart != 0x10 || c.FPS != 31.25f) { std::printf("009f8ba0_TEST FAIL\n"); return 1; }
    std::printf("009f8ba0_TEST PASS\n");
    return 0;
}
