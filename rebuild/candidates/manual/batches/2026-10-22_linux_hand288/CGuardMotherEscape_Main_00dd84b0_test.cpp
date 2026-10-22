// Behaviour test for 0x00DD84B0 CGuardMotherEscape::Main: hostile to the hero; until the hero sees or hears him, moans now and then (in range and rand()%500 == 0); then is surprised, attacks the hero and releases control.
#include <cstdio>
#include <cstring>
static char g_log[8192];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int g_termAt, g_termCalls;
static bool Term() { ++g_termCalls; return g_termCalls == g_termAt; }
static const char* g_ans;
static int g_ansIdx;
static bool Ans(const char* what)
{
    Log(what);
    char c = g_ans[g_ansIdx];
    if (c) ++g_ansIdx;
    return c == 'y';
}
static void Reset(const char* answers, int termAt)
{
    g_log[0] = 0; g_ans = answers; g_ansIdx = 0; g_termAt = termAt; g_termCalls = 0;
}
static int Check(const char* expect)
{
    if (std::strcmp(g_log, expect) != 0) { std::printf("got %s\nexp %s\n", g_log, expect); return 1; }
    return 0;
}
static void Frame() { Log("frame"); }
static int g_rand[8]; static int g_randIdx;
static int Rand() { return g_rand[g_randIdx++]; }
struct CGuardMotherEscape {
    void Main()
    {
        Log("pred"); Frame();
        if (Term()) return;
        if (Ans("scripting?")) Log("release");
        while (!Ans("start3")) { Frame(); if (Term()) return; }
        if (Term()) return;
        Log("get:MotherTorture"); Log("enemyOfHero");
        while (!Ans("seen?") && !Ans("heard?")) {
            Frame();
            if (Term()) return;
            if (Ans("hero<range") && Rand() % 500 == 0) {
                if (Term()) return;
                Log("conv"); Log("line:MOANING");
            }
        }
        if (Term()) return;
        Log("conv"); Log("line:SURPRISED"); Log("attackHero");
        if (Ans("scripting?")) Log("release");
    }
};
int main()
{
    int bad = 0; CGuardMotherEscape s;
    Reset("", 1); s.Main(); bad += Check("pred;frame;");
    g_rand[0] = 1000; g_rand[1] = 7; g_randIdx = 0;
    Reset("ny" "nn" "y" "nn" "y" "nn" "n" "y" "y", 0); s.Main(); bad += Check("pred;frame;scripting?;start3;get:MotherTorture;enemyOfHero;seen?;heard?;frame;hero<range;conv;line:MOANING;seen?;heard?;frame;hero<range;seen?;heard?;frame;hero<range;seen?;conv;line:SURPRISED;attackHero;scripting?;release;");
    if (bad) { std::printf("00dd84b0_TEST FAIL\n"); return 1; }
    std::printf("00dd84b0_TEST PASS\n"); return 0;
}
