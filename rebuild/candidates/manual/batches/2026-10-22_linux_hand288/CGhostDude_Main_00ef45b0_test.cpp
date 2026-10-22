// Behaviour test for 0x00EF45B0 CGhostDude::Main: half transparent and unkillable; forever waits to be set on fire and to go out again, showing a message each time.
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
struct CGhostDude {
    void Main()
    {
        Log("pred"); Frame();
        if (Term()) return;
        Log("alpha0.5"); Log("unkillable");
        if (Ans("scripting?")) Log("release");
        while (!Ans("start4")) { Frame(); if (Term()) return; }
        if (Term()) return;
        while (!Term()) {
            while (!Ans("onfire?")) { Frame(); if (Term()) return; }
            if (Term()) return;
            Log("msg:on fire");
            while (Ans("onfire?")) { Frame(); if (Term()) return; }
            if (Term()) return;
            Log("msg:gone out");
            Frame();
        }
    }
};
int main()
{
    int bad = 0; CGhostDude s;
    Reset("", 1); s.Main(); bad += Check("pred;frame;");
    Reset("nynyyn" "yn", 12); s.Main(); bad += Check("pred;frame;alpha0.5;unkillable;scripting?;start4;onfire?;frame;onfire?;msg:on fire;onfire?;frame;onfire?;msg:gone out;frame;onfire?;msg:on fire;onfire?;msg:gone out;frame;onfire?;frame;");
    if (bad) { std::printf("00ef45b0_TEST FAIL\n"); return 1; }
    std::printf("00ef45b0_TEST PASS\n"); return 0;
}
