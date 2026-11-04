// Behaviour test for 0x00D73C70 CHTG_AmbushPointBackup::Main: wait for the tracked thing within 5, then once a second swap the two ambush points while the first is on screen; once off screen spawn four enemies at it (with its data string) and remove both points.
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
struct CHTG_AmbushPointBackup {
    void Main()
    {
        bool done = false;
        while (!Ans("tracked<5")) { Frame(); if (Term()) return; }
        if (Term()) return;
        int point = 1, other = 2;          // nearest cond point, and the one nearest to it
        Log("nearest:Me"); Log("nearest:point");
        int swap = 0;
        do {
            Frame();
            if (Term()) return;
            char buf[32];
            std::sprintf(buf, "onscreen%d?", point);
            if (Ans(buf)) {
                if (Term()) return;
                swap = point; point = other; other = swap;
            } else {
                if (Term()) return;
                std::sprintf(buf, "spawn4@%d", point); Log(buf);
                done = true;
                std::sprintf(buf, "remove%d", point); Log(buf);
                std::sprintf(buf, "remove%d", other); Log(buf);
            }
            Log("pause1");
        } while (!done);
        Term();
    }
};
int main()
{
    int bad = 0; CHTG_AmbushPointBackup s;
    Reset("", 1); s.Main(); bad += Check("tracked<5;frame;");
    Reset("nyyyn", 0); s.Main(); bad += Check("tracked<5;frame;tracked<5;nearest:Me;nearest:point;frame;onscreen1?;pause1;frame;onscreen2?;pause1;frame;onscreen1?;spawn4@1;remove1;remove2;pause1;");
    Reset("yn", 0); s.Main(); bad += Check("tracked<5;nearest:Me;nearest:point;frame;onscreen1?;spawn4@1;remove1;remove2;pause1;");
    if (bad) { std::printf("00d73c70_TEST FAIL\n"); return 1; }
    std::printf("00d73c70_TEST PASS\n"); return 0;
}