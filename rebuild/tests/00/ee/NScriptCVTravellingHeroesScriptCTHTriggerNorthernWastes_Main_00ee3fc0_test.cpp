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
static int g_dist;
static bool IsDistanceBetweenThingsUnder(const Thing& a, const Thing& b, float d) { LogI("dist", (int)d); return --g_dist <= 0; }
struct Pred { Thing T; Pred(const Thing& t) : T(t) { } };
struct Parent { void PlayCutscene(const char* name) { LogS("cutscene", name); } };
struct Game { Thing Hero;
    void NewScriptFrame() { Log("frame"); }
    void SetThingPersistent(const Thing& t, bool p) { LogI("persist", p); }
    Thing& GetHero() { return Hero; }
    void RemoveThing(const Thing& t, bool a, bool b) { LogI("remove", a * 10 + b); }
};
struct Trigger { Game* G; Thing Me; Parent* ParentClass;
    void SetPredicate(const Pred& p) { LogI("predicate", p.T.Id); }
    bool IsActiveThreadTerminating() { return Term(); }
    void Main()
    {
        SetPredicate(Pred(Me));
        G->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        G->SetThingPersistent(Me, true);
        while (!IsDistanceBetweenThingsUnder(Me, G->GetHero(), 5.0f)) {
            G->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        ParentClass->PlayCutscene("CS_TRAVHERO_SNOWROSE");
        G->RemoveThing(Me, false, true);
    }
};
int main()
{
    bool ok = true;
    Game g; Parent p; Trigger t; t.G = &g; t.ParentClass = &p; t.Me.Id = 9;
    Reset(1000); g_dist = 2; t.Main();
    ok = ok && std::strcmp(g_log, "predicate9;frame;persist1;dist5;frame;dist5;cutscene CS_TRAVHERO_SNOWROSE;remove1;") == 0;
    Reset(1); g_dist = 5; t.Main();
    ok = ok && std::strcmp(g_log, "predicate9;frame;persist1;dist5;frame;") == 0;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00ee3fc0_TEST FAIL\n"); return 1; }
    std::printf("00ee3fc0_TEST PASS\n");
    return 0;
}