// Behaviour test for 0x00E5F8F0 CHenchmanRegionExitComment::Main: with no body guards return at once; else wait until none is within 30, then each frame ask for a comment from henchmen within 10 until one is made.
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
struct CHenchmanRegionExitComment {
    unsigned int count;
    bool Under(unsigned int i, int d) { char b[32]; std::sprintf(b, "h%u<%d", i, d); return Ans(b); }
    void Main()
    {
        Log("getAll");
        if (count == 0) { Term(); return; }
        bool allAway;
        do {
            Frame();
            if (Term()) return;
            allAway = true;
            for (unsigned int i = 0; i < count; ++i) {
                if (Term()) return;
                if (Under(i, 30)) { if (Term()) return; allAway = false; }
            }
            if (Term()) return;
        } while (!allAway);
        if (Term()) return;
        while (!Term()) {
            for (unsigned int i = 0; i < count; ++i) {
                if (Term()) return;
                if (Under(i, 10)) {
                    if (Term()) return;
                    if (Ans("comment")) { Term(); return; }
                }
            }
            if (Term()) return;
            Frame();
        }
    }
};
int main()
{
    int bad = 0; CHenchmanRegionExitComment s;
    s.count = 0; Reset("", 0); s.Main(); bad += Check("getAll;");
    s.count = 2; Reset("ny" "nn" "yn" "n" "yy", 0); s.Main(); bad += Check("getAll;frame;h0<30;h1<30;frame;h0<30;h1<30;h0<10;comment;h1<10;frame;h0<10;comment;");
    if (bad) { std::printf("00e5f8f0_TEST FAIL\n"); return 1; }
    std::printf("00e5f8f0_TEST PASS\n"); return 0;
}