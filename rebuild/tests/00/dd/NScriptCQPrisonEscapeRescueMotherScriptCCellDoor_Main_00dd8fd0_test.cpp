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
struct Thing { int usedAfter; int polls;
    bool MsgIsUsedBy(const char* who) { ++polls; return std::strcmp(who, "SCRIPT_NAME_HERO") == 0 && polls > usedAfter; } };
struct MasterData { bool PrisonKeyStolenByHero; };
static MasterData* g_md;
struct Game {
    void NewScriptFrame() { Log("frame"); if (g_md) g_md->PrisonKeyStolenByHero = true; }
    void EntitySetTargetable(const Thing&, bool t) { LogI("targetable", t); }
    void OpenDoor(const Thing&) { Log("open"); }
};
struct CellDoor { Game* G; Thing Me; MasterData* PMasterData;
    bool IsActiveThreadTerminating() { return Term(); }
    void Main()
    {
        G->EntitySetTargetable(Me, false);
        while (!PMasterData->PrisonKeyStolenByHero) {
            G->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        G->EntitySetTargetable(Me, true);
        if (IsActiveThreadTerminating())
            return;
        do {
            if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating())
                    return;
                G->OpenDoor(Me);
                return;
            }
            G->NewScriptFrame();
        } while (!IsActiveThreadTerminating());
    }
};
int main()
{
    bool ok = true;
    Game g; MasterData md; md.PrisonKeyStolenByHero = false; g_md = &md;
    CellDoor d; d.G = &g; d.PMasterData = &md; d.Me.usedAfter = 2; d.Me.polls = 0;
    Reset(1000); d.Main();
    ok = std::strcmp(g_log, "targetable0;frame;targetable1;frame;frame;open;") == 0;
    md.PrisonKeyStolenByHero = false; d.Me.polls = 0;
    Reset(0); d.Main();
    ok = ok && std::strcmp(g_log, "targetable0;frame;") == 0;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00dd8fd0_TEST FAIL\n"); return 1; }
    std::printf("00dd8fd0_TEST PASS\n");
    return 0;
}