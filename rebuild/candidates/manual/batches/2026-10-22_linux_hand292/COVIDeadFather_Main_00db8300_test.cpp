// Behaviour test for 0x00DB8300 COVI_DeadFather::Main: take control, teleport to and face the marker, loop the dead
// pose until the quest says the father was found, remove his minimap marker, idle until terminated.
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
struct COVI_DeadFather {
    void Main()
    {
        Log("pred"); Frame();
        if (Term()) return;
        if (Ans("scripting?")) Log("release");
        while (!Ans("start4")) { Frame(); if (Term()) return; }
        if (Term()) return;
        Log("teleport:MK_OVID_DAD"); Log("face:MK_OVID_DAD");
        Log("loop:CS_DEAD_DAD");
        while (!Ans("dadFound")) { Frame(); if (Term()) return; }
        if (Term()) return;
        Log("removeMarker");
        do { Frame(); } while (!Term());
    }
};
int main()
{
    int bad = 0; COVI_DeadFather s;
    Reset("", 1); s.Main(); bad += Check("pred;frame;");
    Reset("nnyny", 6); s.Main(); bad += Check("pred;frame;scripting?;start4;frame;start4;teleport:MK_OVID_DAD;face:MK_OVID_DAD;loop:CS_DEAD_DAD;dadFound;frame;dadFound;removeMarker;frame;");
    Reset("ynn", 4); s.Main(); bad += Check("pred;frame;scripting?;release;start4;frame;start4;frame;start4;frame;");
    if (bad) { std::printf("00db8300_TEST FAIL\n"); return 1; }
    std::printf("00db8300_TEST PASS\n"); return 0;
}
