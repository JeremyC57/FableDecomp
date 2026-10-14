#include <cstdio>
long g_fps = 15; long g_frame = 5000; float g_seekBackSeconds = 2.0f; int g_worldCalls = 0;
struct CWorld { static long GetFrameCount() { return g_frame; } };
static long GFSecondsToFrames(float s) { return (long)(g_fps * s); }
struct CGroup { CWorld world; long FrameLastCaptured; CWorld* GetPWorld() { ++g_worldCalls; return &world; }
    void OnActivate() { FrameLastCaptured = GetPWorld()->GetFrameCount() - GFSecondsToFrames(g_seekBackSeconds); } };
int main()
{
    CGroup g; g.FrameLastCaptured = 0; g.OnActivate();
    bool ok = g.FrameLastCaptured == 5000 - 30 && g_worldCalls == 1;
    g_seekBackSeconds = 0.5f; g_frame = 100; g.OnActivate(); ok = ok && g.FrameLastCaptured == 100 - 7;
    if (!ok) { std::printf("0091b2f0_TEST FAIL\n"); return 1; }
    std::printf("0091b2f0_TEST PASS\n"); return 0;
}