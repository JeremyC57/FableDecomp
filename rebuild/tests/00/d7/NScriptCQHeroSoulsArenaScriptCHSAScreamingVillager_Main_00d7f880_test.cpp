// Behaviour test for 0x00D7F880 CHSA_ScreamingVillager::Main: wait for the hero within 50, turn scared, run (re-issuing the move whenever idle) to the RunAwayMarker until within 2, then fade out and die.
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
struct CHSA_ScreamingVillager {
    void Main()
    {
        Log("pred"); Frame();
        if (Term()) return;
        if (Ans("scripting?")) Log("release");
        while (!Ans("start4")) { Frame(); if (Term()) return; }
        if (Term()) return;
        while (!Ans("hero<50")) { Frame(); if (Term()) return; }
        if (Term()) return;
        Log("get:RunAwayMarker"); Log("scared");
        if (!Term()) {
            for (;;) {
                if (!Ans("busy?")) { if (Term()) return; Log("move"); }
                if (Ans("marker<2")) break;
                Frame();
                if (Term()) return;
            }
            if (Term()) return;
            Log("fade");
        }
    }
};
int main()
{
    int bad = 0; CHSA_ScreamingVillager s;
    Reset("", 1); s.Main(); bad += Check("pred;frame;");
    Reset("nynynnyy", 0); s.Main(); bad += Check("pred;frame;scripting?;start4;hero<50;frame;hero<50;get:RunAwayMarker;scared;busy?;move;marker<2;frame;busy?;marker<2;fade;");
    Reset("nyynn", 6); s.Main(); bad += Check("pred;frame;scripting?;start4;hero<50;get:RunAwayMarker;scared;busy?;move;marker<2;frame;");
    if (bad) { std::printf("00d7f880_TEST FAIL\n"); return 1; }
    std::printf("00d7f880_TEST PASS\n"); return 0;
}