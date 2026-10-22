// Behaviour test for 0x00D72200 CHTG_Audience::Main: take control, look up the executioner / watch point / prisoner, wait for the hero or prisoner or tracked thing to come near, set the two opinion sources, release, idle until terminated.
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
struct CHTG_Audience {
    void Main()
    {
        Log("pred"); Frame();
        if (Term()) return;
        if (Ans("scripting?")) Log("release");
        while (!Ans("start4")) { Frame(); if (Term()) return; }
        if (Term()) return;
        Log("get:HTG_Executioner"); Log("get:HTG_AudienceWatchPoint"); Log("get:HTG_Prisoner");
        Log("ally");
        while (!Ans("hero<15") && !Ans("prisoner<10") && !Ans("tracked<10")) { Frame(); if (Term()) return; }
        if (Term()) return;
        Log("opinion:EVIL_HIGH"); Log("opinion:GOOD_HIGH");
        if (Ans("scripting?")) Log("release");
        do { Frame(); } while (!Term());
    }
};
int main()
{
    int bad = 0; CHTG_Audience s;
    Reset("", 1); s.Main(); bad += Check("pred;frame;");
    Reset("nynnny", 5); s.Main(); bad += Check("pred;frame;scripting?;start4;get:HTG_Executioner;get:HTG_AudienceWatchPoint;get:HTG_Prisoner;ally;hero<15;prisoner<10;tracked<10;frame;hero<15;opinion:EVIL_HIGH;opinion:GOOD_HIGH;scripting?;frame;");
    Reset("yynny", 4); s.Main(); bad += Check("pred;frame;scripting?;release;start4;get:HTG_Executioner;get:HTG_AudienceWatchPoint;get:HTG_Prisoner;ally;hero<15;prisoner<10;tracked<10;opinion:EVIL_HIGH;opinion:GOOD_HIGH;scripting?;frame;");
    if (bad) { std::printf("00d72200_TEST FAIL\n"); return 1; }
    std::printf("00d72200_TEST PASS\n"); return 0;
}