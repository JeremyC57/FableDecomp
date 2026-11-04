// Behaviour test for 0x00D72590 CHTG_Boss::Main: wait for the hero (25) / tracked thing (30) / quest state 7, then walk to the boss exit point until within 2 and remove itself, or just release control when there is no exit point.
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
struct CHTG_Boss {
    int State;
    void Main()
    {
        Log("pred"); Frame();
        if (Term()) return;
        if (Ans("scripting?")) Log("release");
        while (!Ans("start4")) { Frame(); if (Term()) return; }
        if (Term()) return;
        while (!Ans("hero<25") && !Ans("tracked<30") && State != 7) { Frame(); if (Term()) return; }
        if (Term()) return;
        Log("get:HTG_BossExitPoint");
        if (Ans("exitAlive?")) {
            if (Term()) return;
            Log("pos");
            while (Ans("me>2")) {
                Frame();
                if (Term()) return;
                Log("move");
                while (Ans("busy?")) { Frame(); if (Term()) return; }
                if (Term()) return;
            }
            if (Term()) return;
            Log("remove");
        } else {
            if (Term()) return;
            if (Ans("scripting?")) Log("release");
        }
    }
};
int main()
{
    int bad = 0; CHTG_Boss s; s.State = 0;
    Reset("", 1); s.Main(); bad += Check("pred;frame;");
    Reset("nynnnyyyynyn", 0); s.Main(); bad += Check("pred;frame;scripting?;start4;hero<25;tracked<30;frame;hero<25;tracked<30;get:HTG_BossExitPoint;exitAlive?;pos;me>2;frame;move;busy?;frame;busy?;me>2;frame;move;busy?;me>2;remove;");
    Reset("nyyny", 0); s.Main(); bad += Check("pred;frame;scripting?;start4;hero<25;get:HTG_BossExitPoint;exitAlive?;scripting?;release;");
    s.State = 7; Reset("nynn" "y" "n", 0); s.Main(); bad += Check("pred;frame;scripting?;start4;hero<25;tracked<30;get:HTG_BossExitPoint;exitAlive?;pos;me>2;remove;");
    if (bad) { std::printf("00d72590_TEST FAIL\n"); return 1; }
    std::printf("00d72590_TEST PASS\n"); return 0;
}