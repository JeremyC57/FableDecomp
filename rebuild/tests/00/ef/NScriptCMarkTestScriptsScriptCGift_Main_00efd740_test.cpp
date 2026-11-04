// Behaviour test for 0x00EFD740 CGift::Main: once per approach of the hero (within 0.5) report the gift's kind; the approach re-arms beyond 2 units.
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
static std::string DefName() { return "OBJECT_ROSE"; }
static void AddScreenMessage(const std::string& s) { Log(s.c_str()); }
struct CGift {
    void Main()
    {
        bool shown = false;
        while (!Term()) {
            if (Ans("hero<0.5") && !shown) {
                if (Term()) return;
                shown = true;
                if (Ans("romantic?")) { if (Term()) return; AddScreenMessage(DefName() + std::string(" is a romantic gift")); }
                else if (Ans("friendly?")) { if (Term()) return; AddScreenMessage(DefName() + std::string(" is a friendly gift")); }
                else if (Ans("offensive?")) { if (Term()) return; AddScreenMessage(DefName() + std::string(" is an offensive gift")); }
            }
            if (Ans("hero>2")) { if (Term()) return; shown = false; }
            Frame();
        }
    }
};
int main()
{
    int bad = 0; CGift s;
    Reset("yyy" "ynyy" "ynnyn" "yn", 13); s.Main(); bad += Check("hero<0.5;romantic?;OBJECT_ROSE is a romantic gift;hero>2;frame;hero<0.5;romantic?;friendly?;OBJECT_ROSE is a friendly gift;hero>2;frame;hero<0.5;romantic?;friendly?;offensive?;OBJECT_ROSE is an offensive gift;hero>2;frame;hero<0.5;hero>2;frame;");
    if (bad) { std::printf("00efd740_TEST FAIL\n"); return 1; }
    std::printf("00efd740_TEST PASS\n"); return 0;
}