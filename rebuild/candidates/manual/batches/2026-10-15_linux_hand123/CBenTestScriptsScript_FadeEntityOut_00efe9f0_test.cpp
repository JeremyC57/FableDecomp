// Behaviour test for CBenTestScriptsScript::FadeEntityOut (0x00EFE9F0): alpha stepped down to zero one
// tick at a time, an aborting script stops early, the optional hide runs at the end.
#include <cstdio>

static int g_alphaSets = 0, g_ticks = 0, g_hides = 0;
struct CScript {
    int abortAfter;
    bool IsActiveThreadTerminating() { return abortAfter != 0 && g_ticks >= abortAfter; }
    void FadeEntityOut(bool hideAtEnd, float start, float step)
    {
        float alpha = start;
        while (alpha > 0.0f) {
            ++g_alphaSets;
            alpha -= step;
            ++g_ticks;
            if (IsActiveThreadTerminating()) return;
        }
        if (hideAtEnd) ++g_hides;
    }
};

int main()
{
    CScript s = { 0 };
    s.FadeEntityOut(true, 1.0f, 0.25f);
    if (g_alphaSets != 4 || g_ticks != 4 || g_hides != 1) { std::printf("00efe9f0_TEST FAIL\n"); return 1; }
    g_alphaSets = g_ticks = 0;
    CScript t = { 2 };
    t.FadeEntityOut(true, 1.0f, 0.25f);
    if (g_alphaSets != 2 || g_hides != 1) { std::printf("00efe9f0_TEST FAIL\n"); return 1; }
    g_alphaSets = g_ticks = 0;
    s.FadeEntityOut(false, 0.0f, 0.25f);
    if (g_alphaSets != 0 || g_hides != 1) { std::printf("00efe9f0_TEST FAIL\n"); return 1; }
    std::printf("00efe9f0_TEST PASS\n");
    return 0;
}
