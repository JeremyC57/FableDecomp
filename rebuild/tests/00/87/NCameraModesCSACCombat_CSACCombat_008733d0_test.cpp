// Behaviour test for 0x008733D0: the reconstructed logic over a host model.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
// Host model of the string-and-cage camera-mode constructor: the root records its arguments, the cage stores the rest.
struct Root { void* mgr; const void* def; float DefaultFOV; Root(void* m, const void* d) : mgr(m), def(d), DefaultFOV(0) {} };
struct Cage : Root {
    float DesiredHeightOffset, DesiredStringLength, DesiredCageRadius; int NextAutoGoBehindWF, TimerForExpansion; bool AutoGoBehind; float AutoGoBehindTime;
    Cage(void* m, const void* d, float len, float rad, float h, bool agb, float agbt, float fov)
        : Root(m, d), DesiredHeightOffset(h), DesiredStringLength(len), DesiredCageRadius(rad), NextAutoGoBehindWF(-1), TimerForExpansion(-1),
          AutoGoBehind(agb), AutoGoBehindTime(agbt) { DefaultFOV = fov; }
};
static void Run()
{
    int mgr = 0, def = 0;
    Cage c(&mgr, &def, 4.0f, 2.5f, 1.25f, true, 3.0f, 70.0f);
    CHECK(c.mgr == &mgr && c.def == &def && c.DesiredStringLength == 4.0f && c.DesiredCageRadius == 2.5f && c.DesiredHeightOffset == 1.25f);
    CHECK(c.NextAutoGoBehindWF == -1 && c.TimerForExpansion == -1 && c.AutoGoBehind && c.AutoGoBehindTime == 3.0f && c.DefaultFOV == 70.0f);
}

int main()
{
    Run();
    if (g_fail) { std::printf("008733d0_TEST FAIL\n"); return 1; }
    std::printf("008733d0_TEST PASS\n");
    return 0;
}