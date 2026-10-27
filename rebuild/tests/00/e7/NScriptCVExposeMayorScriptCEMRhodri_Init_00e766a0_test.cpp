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
    void SetThingHasInformation(const Thing&, bool a, bool b, bool c) { LogI("info", a * 100 + b * 10 + c); }
    void SetIsPushableByHero(Thing, bool p) { LogI("push", p); }
    void SetIsThingForcePushable(const Thing&, bool p) { LogI("force", p); }
    void EntitySetAsKillable(const Thing&, bool k, bool b) { LogI("killable", k * 10 + b); }
    void EntitySetAsToAddToComboMultiplierWhenHit(const Thing&, bool c) { LogI("combo", c); }
    void EntitySetAsDamageable(const Thing&, bool d) { LogI("damageable", d); }
};
struct Rhodri { Game* G; Thing Me; bool ExplainedPlight;
    void Init()
    {
        G->SetThingHasInformation(Me, false, false, false);
        G->SetIsPushableByHero(Me, false);
        G->SetIsThingForcePushable(Me, false);
        G->EntitySetAsKillable(Me, false, false);
        G->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
        G->EntitySetAsDamageable(Me, false);
        ExplainedPlight = false;
    }
};
int main()
{
    bool ok = true;
    Game g; Rhodri r; r.G = &g; r.ExplainedPlight = true;
    Reset(1000); r.Init();
    ok = std::strcmp(g_log, "info0;push0;force0;killable0;combo0;damageable0;") == 0 && !r.ExplainedPlight;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00e766a0_TEST FAIL\n"); return 1; }
    std::printf("00e766a0_TEST PASS\n");
    return 0;
}