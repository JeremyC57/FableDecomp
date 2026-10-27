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
#include <cstdlib>
struct Thing { int Id; const char* data; const char* GetDataString() const { return data; } };
struct Str { char t[16]; Str& operator=(const char* s) { std::strcpy(t, s); return *this; } };
static long GFCharStringToInt(const char* s) { return std::atol(s); }
struct Game { Thing Hero;
    void SetIsThingForcePushable(const Thing&, bool p) { LogI("force", p); }
    void SetIsPushableByHero(Thing, bool p) { LogI("push", p); }
    void EntitySetAsUseMovementInActions(const Thing&, bool m) { LogI("move", m); }
    void EntitySetAsKillable(const Thing&, bool k, bool b) { LogI("killable", k * 10 + b); }
    void EntitySetAsDamageable(const Thing&, bool d) { LogI("damageable", d); }
    Thing& GetHero() { return Hero; }
    void EntityForceToLookAtThing(const Thing& a, const Thing& b) { LogI("look", a.Id * 10 + b.Id); }
};
struct Guard { Game* G; Thing Me; unsigned NextLine; unsigned ID; Str Name;
    void Init()
    {
        G->SetIsThingForcePushable(Me, false);
        G->SetIsPushableByHero(Me, false);
        G->EntitySetAsUseMovementInActions(Me, false);
        G->EntitySetAsKillable(Me, false, true);
        G->EntitySetAsDamageable(Me, false);
        G->EntityForceToLookAtThing(Me, G->GetHero());
        ID = GFCharStringToInt(Me.GetDataString());
        Name = ID == 0 ? "_WEAK" : "_TOUGH";
        NextLine = 1;
    }
};
int main()
{
    bool ok = true;
    Game g; g.Hero.Id = 1; Guard s; s.G = &g; s.Me.Id = 2; s.Me.data = "3"; s.NextLine = 0;
    Reset(1000); s.Init();
    ok = std::strcmp(g_log, "force0;push0;move0;killable1;damageable0;look21;") == 0 && s.ID == 3 && std::strcmp(s.Name.t, "_TOUGH") == 0 && s.NextLine == 1;
    s.Me.data = "0"; s.Init();
    ok = ok && s.ID == 0 && std::strcmp(s.Name.t, "_WEAK") == 0;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00eb2070_TEST FAIL\n"); return 1; }
    std::printf("00eb2070_TEST PASS\n");
    return 0;
}