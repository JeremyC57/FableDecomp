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
struct Thing { int Id; bool killed; bool MsgIsKilledBy(const char* w) const { return w[0] == 0 && killed; } };
struct Game {
    void SetCreatureGeneratorsEnabled(const char* g, bool e) { LogS("gen", g); LogI("on", e); }
    void SetRegionEntranceAsActive(const Thing& t, bool a) { LogI("entrance", t.Id * 10 + a); }
    void SetRegionExitAsActive(const Thing& t, bool a) { LogI("exit", t.Id * 10 + a); }
};
struct Troll { Game* G; Thing Me; Thing Entrance; Thing Exit;
    void OnPredicateFail()
    {
        if (Me.MsgIsKilledBy("")) {
            G->SetCreatureGeneratorsEnabled("LostBay", true);
            G->SetRegionEntranceAsActive(Entrance, true);
            G->SetRegionExitAsActive(Exit, true);
        }
    }
};
int main()
{
    bool ok = true;
    Game g; Troll t; t.G = &g; t.Entrance.Id = 3; t.Exit.Id = 4;
    Reset(0); t.Me.killed = true; t.OnPredicateFail();
    ok = ok && std::strcmp(g_log, "gen LostBay;on1;entrance31;exit41;") == 0;
    Reset(0); t.Me.killed = false; t.OnPredicateFail(); ok = ok && g_log[0] == 0;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00ee0350_TEST FAIL\n"); return 1; }
    std::printf("00ee0350_TEST PASS\n");
    return 0;
}