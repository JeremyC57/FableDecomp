// Behaviour test for CAIStateGroup_IceTrollIcicleAttack::Update (0x009621F0): model copy with scripted stubs.
#include <cstdio>

static bool g_alive, g_hasStealth, g_hasModes, g_knockedDown, g_stopping;
static int g_riseResult, g_lookResult, g_freeScript[8], g_fi, g_posted, g_yields, g_busy, g_doNothing, g_doNothingResult;
static int g_dangerSet, g_rose, g_boasts, g_term, g_stopAfterYields;
struct Group {
    void Terminate() { ++g_term; }
    bool IsStopping() { return g_stopping || (g_stopAfterYields > 0 && g_yields >= g_stopAfterYields); }
    void Update()
    {
        if (g_alive) {
            if (g_hasStealth) ++g_dangerSet;
            if (g_hasModes && g_knockedDown) {
                ++g_rose;
                if (g_riseResult == 1) { Terminate(); return; }
            }
            if (g_lookResult == 2 && g_alive && !IsStopping()) {
                int remaining = 2;
                do {
                    bool freeNow = g_freeScript[g_fi] != 0; if (g_fi < 7) ++g_fi;
                    if (freeNow) { ++g_posted; --remaining; }
                    ++g_yields;
                    if (IsStopping()) { Terminate(); return; }
                } while (remaining > 0);
                while (g_busy > 0) {
                    --g_busy; ++g_doNothing;
                    if (g_doNothingResult == 1) { Terminate(); return; }
                }
                ++g_boasts;
            }
        }
        Terminate();
    }
};
static int fail(int n) { std::printf("009621f0_TEST FAIL %d\n", n); return 1; }
static void reset()
{
    g_alive = g_hasStealth = g_hasModes = true; g_knockedDown = g_stopping = false; g_riseResult = 2; g_lookResult = 2;
    for (int i = 0; i < 8; ++i) g_freeScript[i] = 1;
    g_fi = g_posted = g_yields = g_busy = g_doNothing = g_dangerSet = g_rose = g_boasts = g_term = g_stopAfterYields = 0;
    g_doNothingResult = 2;
}
int main()
{
    Group g;
    reset(); g.Update();
    if (g_dangerSet != 1 || g_posted != 2 || g_yields != 2 || g_boasts != 1 || g_term != 1) return fail(1);
    reset(); g_freeScript[0] = 0; g_freeScript[1] = 0; g_busy = 2; g.Update();
    if (g_yields != 4 || g_posted != 2 || g_doNothing != 2 || g_boasts != 1) return fail(2);
    reset(); g_knockedDown = true; g_riseResult = 1; g.Update();
    if (g_rose != 1 || g_yields != 0 || g_term != 1) return fail(3);
    reset(); g_lookResult = 3; g.Update();
    if (g_yields != 0 || g_boasts != 0 || g_term != 1) return fail(4);
    reset(); g_stopAfterYields = 1; g.Update();
    if (g_yields != 1 || g_boasts != 0 || g_term != 1) return fail(5);
    reset(); g_alive = false; g.Update();
    if (g_dangerSet != 0 || g_term != 1) return fail(6);
    reset(); g_busy = 1; g_doNothingResult = 1; g.Update();
    if (g_boasts != 0 || g_term != 1) return fail(7);
    std::printf("009621f0_TEST PASS\n");
    return 0;
}