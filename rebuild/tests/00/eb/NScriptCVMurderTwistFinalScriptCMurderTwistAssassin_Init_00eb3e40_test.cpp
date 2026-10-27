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
struct Def { float AssassinThreatDistSq; };
static Def g_def = { 25.0f };
static Def* pGlobalScriptDef = &g_def;
struct Thing { int Id; void SetFriendsWithEverythingFlag(bool f) { LogI("friends", f); } };
struct Game { Thing Hero;
    void RemoveThing(const Thing&, bool a, bool b) { LogI("remove", a * 10 + b); }
    void SetIsThingForcePushable(const Thing&, bool p) { LogI("force", p); }
    void SetIsPushableByHero(Thing, bool p) { LogI("push", p); }
    void EntitySetAsUseMovementInActions(const Thing&, bool m) { LogI("move", m); }
    void EntitySetAsKillable(const Thing&, bool k, bool b) { LogI("killable", k * 10 + b); }
    void EntitySetAsDamageable(const Thing&, bool d) { LogI("damageable", d); }
    Thing& GetHero() { return Hero; }
    void EntityForceToLookAtThing(const Thing& a, const Thing& b) { LogI("look", a.Id * 10 + b.Id); }
    void SetThingHasInformation(const Thing&, bool a, bool b, bool c) { LogI("info", a * 100 + b * 10 + c); }
    void SetThingPersistent(const Thing&, bool p) { LogI("persist", p); }
};
struct Parent { int SheriffState; };
struct Assassin { Game* G; Thing Me; Parent* ParentClass; unsigned NextLine; bool HasBeenThreatened; float ThreatDistSq; bool HasGivenIntroToHero, PreemptiveAttackInProgress;
    void Init()
    {
        HasBeenThreatened = false;
        ThreatDistSq = pGlobalScriptDef->AssassinThreatDistSq;
        NextLine = 1;
        HasGivenIntroToHero = false;
        PreemptiveAttackInProgress = false;
        if (ParentClass->SheriffState == 0) {
            G->RemoveThing(Me, false, true);
            return;
        }
        G->SetIsThingForcePushable(Me, false);
        G->SetIsPushableByHero(Me, false);
        G->EntitySetAsUseMovementInActions(Me, false);
        G->EntitySetAsKillable(Me, false, true);
        G->EntitySetAsDamageable(Me, false);
        G->EntityForceToLookAtThing(Me, G->GetHero());
        G->SetThingHasInformation(Me, false, true, false);
        Me.SetFriendsWithEverythingFlag(true);
        G->SetThingPersistent(Me, true);
    }
};
int main()
{
    bool ok = true;
    Game g; g.Hero.Id = 1; Parent p; p.SheriffState = 0; Assassin a; a.G = &g; a.Me.Id = 2; a.ParentClass = &p; a.HasBeenThreatened = true;
    Reset(1000); a.Init();
    ok = std::strcmp(g_log, "remove1;") == 0 && !a.HasBeenThreatened && a.ThreatDistSq == 25.0f && a.NextLine == 1;
    p.SheriffState = 2; Reset(1000); a.Init();
    ok = ok && std::strcmp(g_log, "force0;push0;move0;killable1;damageable0;look21;info10;friends1;persist1;") == 0;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00eb3e40_TEST FAIL\n"); return 1; }
    std::printf("00eb3e40_TEST PASS\n");
    return 0;
}