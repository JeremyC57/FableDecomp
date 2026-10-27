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
struct DefString { const char* s; operator const char*() const { return s; } };
struct Def { DefString IngredientObject; float KilledSomeoneForMushroom; };
static Def g_def = { { "OBJECT_MUSHROOM" }, -20.0f };
static Def* pGlobalScriptDef = &g_def;
struct Thing { bool killed; bool MsgIsKilledBy(const char* w) const { return w[0] == 0 && killed; } };
struct Game {
    void GiveHeroObject(const char* name, int n, bool b) { LogS("give", name); LogI("n", n); LogI("b", b); }
    void GiveHeroMorality(float m) { LogI("morality", (int)m); }
};
struct Man { Thing Me; Game* G; bool GotReward;
    void OnPredicateFail()
    {
        if (!GotReward && Me.MsgIsKilledBy("")) {
            G->GiveHeroObject(pGlobalScriptDef->IngredientObject, -1, false);
            G->GiveHeroMorality(pGlobalScriptDef->KilledSomeoneForMushroom);
        }
    }
};
int main()
{
    bool ok = true;
    Game g; Man m; m.G = &g;
    Reset(0); m.GotReward = false; m.Me.killed = true; m.OnPredicateFail();
    ok = ok && std::strcmp(g_log, "give OBJECT_MUSHROOM;n-1;b0;morality-20;") == 0;
    Reset(0); m.GotReward = true; m.OnPredicateFail(); ok = ok && g_log[0] == 0;
    Reset(0); m.GotReward = false; m.Me.killed = false; m.OnPredicateFail(); ok = ok && g_log[0] == 0;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00ec8230_TEST FAIL\n"); return 1; }
    std::printf("00ec8230_TEST PASS\n");
    return 0;
}