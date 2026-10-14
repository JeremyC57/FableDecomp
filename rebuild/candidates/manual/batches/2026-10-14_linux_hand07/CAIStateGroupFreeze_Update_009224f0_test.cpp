#include <cstdio>
// Behaviour model of CAIStateGroup_Freeze::Update @ 0x009224F0: loops while DoNothing finishes and the group
// is runnable, then terminates.
static int g_doNothing = 0, g_runnable = 0, g_terminated = 0;
static long g_doNothingResults[8] = { 4, 4, 4, 4, 4, 4, 4, 4 }; static bool g_runnableResults[8] = { true, true, false, true, true, true, true, true };
struct CAIStateGroupBase {
    virtual bool IsRunnable() { return g_runnableResults[g_runnable++]; }
    long AIAction_DoNothing() { return g_doNothingResults[g_doNothing++]; }
    void Terminate() { g_terminated++; }
};
struct CAIStateGroup_Freeze : CAIStateGroupBase { void Update() { while (AIAction_DoNothing() == 4 && IsRunnable()) ; Terminate(); } };
int main()
{
    CAIStateGroup_Freeze f;
    f.Update();
    bool ok = g_doNothing == 3 && g_runnable == 3 && g_terminated == 1;
    g_doNothing = g_runnable = 0; g_doNothingResults[0] = 1;
    f.Update();
    ok = ok && g_doNothing == 1 && g_runnable == 0 && g_terminated == 2;
    if (!ok) { std::printf("009224f0_TEST FAIL\n"); return 1; }
    std::printf("009224f0_TEST PASS\n");
    return 0;
}
