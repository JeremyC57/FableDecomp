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
struct Thing { int Id; };
struct Game {
    void SetIsPushableByHero(Thing, bool p) { LogI("push", p); }
    void SetThingHasInformation(const Thing&, bool a, bool b, bool c) { LogI("info", a * 100 + b * 10 + c); }
    void EntitySetAsScared(const Thing&, bool s) { LogI("scared", s); }
    void EntitySetInFaction(const Thing&, const char* f) { LogS("faction", f); }
    void EntitySetAsAllowedToFollowHero(const Thing&, bool f) { LogI("follow", f); }
};
struct Trader { Game* G; Thing Me; int BarIndex;
    void Init()
    {
        G->SetIsPushableByHero(Me, false);
        G->SetThingHasInformation(Me, false, false, false);
        G->EntitySetAsScared(Me, true);
        G->EntitySetInFaction(Me, "FACTION_NEUTRAL");
        G->EntitySetAsAllowedToFollowHero(Me, true);
        BarIndex = 0;
    }
};
int main()
{
    bool ok = true;
    Game g; Trader t; t.G = &g; t.BarIndex = 7;
    Reset(1000); t.Init();
    ok = std::strcmp(g_log, "push0;info0;scared1;faction FACTION_NEUTRAL;follow1;") == 0 && t.BarIndex == 0;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00dfb1b0_TEST FAIL\n"); return 1; }
    std::printf("00dfb1b0_TEST PASS\n");
    return 0;
}