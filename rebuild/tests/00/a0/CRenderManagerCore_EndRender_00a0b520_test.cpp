#include <cstdio>

static double g_now = 0.0;
double GFGetTime() { return g_now; }

struct Core {
    float LastRenderEndTime; long NoTrisPerSecond; bool Rendering; unsigned long NoRenderStateChanges; long NoTrisDrawn;
    void EndRender()
    {
        if (Rendering == true)
        {
            double elapsed = GFGetTime() - LastRenderEndTime;
            NoRenderStateChanges = 0;
            NoTrisPerSecond = (long)(NoTrisDrawn / elapsed);
            Rendering = false;
        }
    }
};

int main()
{
    Core c; c.LastRenderEndTime = 0.5f; c.NoTrisPerSecond = -1; c.Rendering = true; c.NoRenderStateChanges = 77; c.NoTrisDrawn = 1000;
    g_now = 2.5;
    c.EndRender();
    if (c.NoTrisPerSecond != 500 || c.Rendering || c.NoRenderStateChanges != 0) { std::printf("00a0b520_TEST FAIL\n"); return 1; }
    c.NoRenderStateChanges = 5; c.NoTrisDrawn = 9; c.EndRender();   // not rendering: untouched
    if (c.NoTrisPerSecond != 500 || c.NoRenderStateChanges != 5) { std::printf("00a0b520_TEST FAIL\n"); return 1; }
    std::printf("00a0b520_TEST PASS\n");
    return 0;
}