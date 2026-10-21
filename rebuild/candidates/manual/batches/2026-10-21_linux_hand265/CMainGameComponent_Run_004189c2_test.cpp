#include <cstdio>
// Behaviour test for CMainGameComponent::Run (0x004189C2): the start-up branch (event-package load / save, presentation
// interval or the region pointer touch), the frame loop on the system update result (2 quits, 1 runs a frame, forced
// or regular), the frame-time record and the hand-back of the next component.
struct CRefCounted { long RefCount; int Deleted; void AddRef() { ++RefCount; } void Release() { if (--RefCount == 0) Deleted++; } };
static int g_loads = 0, g_saves = 0, g_intervals = 0, g_forced = 0, g_regular = 0, g_frames = 0, g_shutdown = 0;
static int g_updateIndex = 0;
static long g_updates[] = { 1, 0, 1, 2 };
static bool g_Load = false, g_Save = true, g_Interval = false;
static CRefCounted g_regionObject = { 1, 0 };
static double g_clock = 0.0;
static void* g_last = 0;
struct CMain {
    bool Quit, Running, ForceUpdateTick;
    double CurrentFrameStartGameTime, GameStartTime, LastRenderFrameStartTime, TotalFrameTime;
    CMain* NextComponentToRun;
    double GetCurrentGameTime() { g_clock += 1.0; return g_clock; }
    void RecordFrameTime(double d) { TotalFrameTime += d; g_frames++; }
    bool Run(CMain** next)
    {
        GameStartTime = 5.0;
        Running = true;
        Quit = false;
        LastRenderFrameStartTime = GetCurrentGameTime();
        if (g_Load) g_loads++;
        else if (g_Save) g_saves++;
        if (g_Interval) g_intervals++;
        else {
            CRefCounted* p = &g_regionObject;
            if (p) { p->AddRef(); p->Release(); }
        }
        while (!Quit) {
            CurrentFrameStartGameTime = GetCurrentGameTime();
            long result = g_updates[g_updateIndex++];
            if (result == 2) Quit = true;
            else if (result == 1) {
                if (ForceUpdateTick) g_forced++;
                else g_regular++;
            }
            double frameStart = CurrentFrameStartGameTime;
            RecordFrameTime(GetCurrentGameTime() - frameStart);
        }
        Running = false;
        *next = NextComponentToRun;
        g_shutdown++;
        g_last = this;
        return *next != 0;
    }
};
static int Fail() { std::printf("004189c2_TEST FAIL\n"); return 1; }
int main()
{
    CMain m = { true, false, false, 0, 0, 0, 0, 0 };
    CMain other;
    m.NextComponentToRun = &other;
    CMain* next = 0;
    if (!m.Run(&next) || next != &other) return Fail();
    if (g_saves != 1 || g_loads != 0 || g_intervals != 0) return Fail();
    if (g_regionObject.RefCount != 1 || g_regionObject.Deleted) return Fail();
    if (g_frames != 4 || g_regular != 2 || g_forced != 0 || m.TotalFrameTime != 4.0) return Fail();
    if (m.Running || !m.Quit || g_shutdown != 1 || g_last != &m) return Fail();
    m.NextComponentToRun = 0; m.ForceUpdateTick = true; g_updateIndex = 0; g_Interval = true;
    if (m.Run(&next) || next != 0) return Fail();
    if (g_forced != 2 || g_intervals != 1) return Fail();
    std::printf("004189c2_TEST PASS\n");
    return 0;
}
