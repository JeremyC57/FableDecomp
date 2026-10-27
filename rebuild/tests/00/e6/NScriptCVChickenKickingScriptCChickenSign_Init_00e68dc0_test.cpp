#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (interface calls are logged, not executed).
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static void LogI(const char* s, int v) { char b[128]; std::sprintf(b, "%s%d", s, v); Log(b); }
static void LogS(const char* s, const char* v) { char b[256]; std::sprintf(b, "%s %s", s, v); Log(b); }
static int g_termAfter = 1000;   // IsActiveThreadTerminating() turns true after this many calls
static int g_termCalls = 0;
static bool Term() { return ++g_termCalls > g_termAfter; }
static void Reset(int termAfter) { g_log[0] = 0; g_termCalls = 0; g_termAfter = termAfter; }
struct WStr { char t[48]; WStr() { t[0] = 0; } WStr& operator=(const char* s) { std::strcpy(t, s); return *this; } };
struct Vec { WStr items[16]; unsigned n;
    void resize(unsigned m, const WStr& x) { for (unsigned i = n; i < m; ++i) items[i] = x; n = m; }
    void resize(unsigned m) { resize(m, WStr()); }
    WStr& operator[](unsigned i) { return items[i]; } };
struct Sign { Vec TextKeys;
    void Init()
    {
        TextKeys.resize(8);
        TextKeys[0] = "TEXT_QST_B17_SIGN_WON_NOTHING";
        TextKeys[1] = "TEXT_QST_B17_SIGN_WON_EXPR";
        TextKeys[2] = "TEXT_QST_B17_SIGN_WON_KEY";
        TextKeys[3] = "TEXT_QST_B17_SIGN_WON_KEY_EXPR";
        TextKeys[4] = "TEXT_QST_B17_SIGN_WON_HAT";
        TextKeys[5] = "TEXT_QST_B17_SIGN_WON_HAT_EXPR";
        TextKeys[6] = "TEXT_QST_B17_SIGN_WON_HAT_KEY";
        TextKeys[7] = "TEXT_QST_B17_SIGN_WON_HAT_KEY_EXPR";
    }
};
int main()
{
    bool ok = true;
    Sign s; s.TextKeys.n = 0;
    s.Init();
    ok = s.TextKeys.n == 8 && std::strcmp(s.TextKeys[0].t, "TEXT_QST_B17_SIGN_WON_NOTHING") == 0 && std::strcmp(s.TextKeys[7].t, "TEXT_QST_B17_SIGN_WON_HAT_KEY_EXPR") == 0;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00e68dc0_TEST FAIL\n"); return 1; }
    std::printf("00e68dc0_TEST PASS\n");
    return 0;
}