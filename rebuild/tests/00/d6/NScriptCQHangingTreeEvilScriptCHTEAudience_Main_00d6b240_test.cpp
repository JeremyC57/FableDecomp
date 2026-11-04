// Behaviour test for 0x00D6B240 CHTE_Audience::Main: wait for quest state 6 and for the audience to leg it, retake control and walk to the party spawn point, removing itself once within 4 units.
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
struct CHTE_Audience {
    int State; bool Leggit; int stateAt, leggitAt, frames;
    void Frame2() { Frame(); ++frames; if (frames == stateAt) State = 6; if (frames == leggitAt) Leggit = true; }
    void Main()
    {
        Log("pred"); Frame2();
        if (Term()) return;
        if (Ans("scripting?")) Log("release");
        while (!Ans("start4")) { Frame2(); if (Term()) return; }
        if (Term()) return;
        Log("get:left"); Log("get:right");
        while (State != 6) { Frame2(); if (Term()) return; }
        if (Term()) return;
        if (Ans("scripting?")) Log("release");
        while (!Leggit) { Frame2(); if (Term()) return; }
        if (Term()) return;
        if (Ans("scripting?")) Log("release");
        while (!Ans("start4")) { Frame2(); if (Term()) return; }
        if (Term()) return;
        Log("get:HTE_PartySpawnPoint");
        while (!Term()) {
            if (!Ans("busy?")) { if (Term()) return; Log("move"); }
            if (Ans("spawn<4")) { if (Term()) return; Log("remove"); }
            Frame2();
        }
    }
};
static void Run(CHTE_Audience& s, int stateAt, int leggitAt) { s.State = 0; s.Leggit = false; s.stateAt = stateAt; s.leggitAt = leggitAt; s.frames = 0; s.Main(); }
int main()
{
    int bad = 0; CHTE_Audience s;
    Reset("", 1); Run(s, 99, 99); bad += Check("pred;frame;");
    Reset("nynyyny", 11); Run(s, 2, 3); bad += Check("pred;frame;scripting?;start4;get:left;get:right;frame;scripting?;frame;scripting?;release;start4;get:HTE_PartySpawnPoint;busy?;move;spawn<4;remove;frame;");
    if (bad) { std::printf("00d6b240_TEST FAIL\n"); return 1; }
    std::printf("00d6b240_TEST PASS\n"); return 0;
}