// Behaviour test for 0x00E5F2B0 CHenchmanComment::Main: find the CREATURE_BODY_GUARD among the body guards, then each frame let another henchman within 10 comment once more than 6 seconds passed; the timer restarts while none is near.
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
#include <string>
static void Frame() { Log("frame"); }
static const char* g_defs[4];
static int g_seconds;
struct CHenchmanComment {
    unsigned int count;
    void Main()
    {
        Log("getAll");
        if (count == 0) { Term(); return; }
        unsigned int bodyGuard = (unsigned int)-1;
        unsigned int i;
        for (i = 0; i < count; ++i) {
            if (Term()) return;
            if (std::string(g_defs[i]) == "CREATURE_BODY_GUARD") { if (Term()) return; bodyGuard = i; }
        }
        if (Term()) return;
        {
            g_seconds = 0;
            while (!Term()) {
                bool anyNear = false;
                for (i = 0; i < count; ++i) {
                    if (Term()) return;
                    char b[32]; std::sprintf(b, "h%u<10", i);
                    if (bodyGuard != i && Ans(b)) {
                        if (Term()) return;
                        anyNear = true;
                        if (g_seconds > 6 && Ans("comment")) { Term(); return; }
                    }
                }
                if (Term()) return;
                if (!anyNear) { if (Term()) return; g_seconds = 0; Log("reset"); }
                Frame();
                g_seconds += 4;
            }
        }
    }
};
int main()
{
    int bad = 0; CHenchmanComment s;
    s.count = 0; Reset("", 0); s.Main(); bad += Check("getAll;");
    g_defs[0] = "CREATURE_HENCHMAN"; g_defs[1] = "CREATURE_BODY_GUARD";
    s.count = 2; Reset("n" "y" "y" "y", 0); s.Main(); bad += Check("getAll;h0<10;reset;frame;h0<10;frame;h0<10;comment;");
    if (bad) { std::printf("00e5f2b0_TEST FAIL\n"); return 1; }
    std::printf("00e5f2b0_TEST PASS\n"); return 0;
}