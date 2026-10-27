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
static int g_alive = 0, g_talks = 0;
struct Thing {
    bool IsAlive() const { return g_alive-- > 0; }
    bool MsgIsTalkedToBy(const char* who) const { return std::strcmp(who, "SCRIPT_NAME_HERO") == 0 && (g_talks++ % 2) == 0; }
};
struct Game {
    void NewScriptFrame() { Log("frame"); }
    void SetBettingActive(bool b) { LogI("bet", b); }
    void SetCountBetMoneyDown(bool b) { LogI("count", b); }
};
struct Altar { Game* G; Thing Me;
    bool IsActiveThreadTerminating() { return Term(); }
    void Main()
    {
        while (Me.IsAlive()) {
            G->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating())
                    return;
                G->SetBettingActive(true);
                G->SetCountBetMoneyDown(true);
            }
        }
        if (IsActiveThreadTerminating())
            return;
    }
};
int main()
{
    bool ok = true;
    Game g; Altar a; a.G = &g;
    Reset(1000); g_alive = 2; g_talks = 0; a.Main();
    ok = ok && std::strcmp(g_log, "frame;bet1;count1;frame;") == 0 && g_termCalls == 4;
    Reset(1); g_alive = 5; g_talks = 1; a.Main();
    ok = ok && std::strcmp(g_log, "frame;frame;") == 0;
    Reset(1000); g_alive = 0; a.Main(); ok = ok && g_log[0] == 0 && g_termCalls == 1;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00edc390_TEST FAIL\n"); return 1; }
    std::printf("00edc390_TEST PASS\n");
    return 0;
}