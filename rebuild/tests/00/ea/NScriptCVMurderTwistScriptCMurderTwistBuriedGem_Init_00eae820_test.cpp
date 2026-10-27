#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (calls are recorded, not executed).
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_str = "";
struct Thing { int Id; };
struct Game {
    int Calls;
    void NewScriptFrame() { ++Calls; Log("frame"); }
    void SetTimer(int timer, int time) { char b[64]; std::sprintf(b, "timer%d=%d", timer, time); Log(b); }
    void RemoveThing(const Thing&, bool, bool) { Log("remove"); }
    void GiveHeroMorality(float m) { char b[64]; std::sprintf(b, "morality%g", m); Log(b); }
    void GiveHeroRenownPoints(int r) { char b[64]; std::sprintf(b, "renown%d", r); Log(b); }
    void DeactivateQuestLater(const char* q, unsigned int d) { char b[96]; std::sprintf(b, "deactivate %s %u", q, d); Log(b); }
    void MiniMapAddMarker(const Thing&, const char* m) { char b[96]; std::sprintf(b, "marker %s", m); Log(b); }
    void SetThingHasInformation(const Thing&, bool a, bool b2, bool c) { char b[64]; std::sprintf(b, "info%d%d%d", a, b2, c); Log(b); }
    void EntityAttachToScript(const Thing&, const char* s) { char b[96]; std::sprintf(b, "attach %s", s); Log(b); }
    void EntitySetTargetingValidTargetWithoutLOS(const Thing&, bool v) { Log(v ? "los1" : "los0"); }
    void SetThingPersistent(const Thing&, bool p) { Log(p ? "persist1" : "persist0"); }
    void EntitySetAsKillable(const Thing&, bool k, bool b2) { char b[32]; std::sprintf(b, "killable%d%d", k, b2); Log(b); }
    void EntitySetAsToAddToComboMultiplierWhenHit(const Thing&, bool a) { Log(a ? "combo1" : "combo0"); }
    void SetCreatureGeneratorsEnabled(const char* n, bool e) { char b[96]; std::sprintf(b, "gens %s %d", n, e); Log(b); }
    void EntitySetDeedReactionsEnabled(const Thing&, bool e) { Log(e ? "deeds1" : "deeds0"); }
    void EntitySetInFaction(const Thing&, const char* f) { char b[96]; std::sprintf(b, "faction %s", f); Log(b); }
    const char* GetActiveQuestName() { return "Q"; }
    void SetCategoryActivity(const char* c, bool a) { char b[96]; std::sprintf(b, "category %s %d", c, a); Log(b); }
    void SetIsPushableByHero(Thing, bool p) { Log(p ? "push1" : "push0"); }
};
static Game g_game;
class CTimer { public: int TimerIndex; CTimer& operator=(int t) { g_game.SetTimer(TimerIndex, t); return *this; } };
struct Vec { Thing buf[4]; int n; void push_back(const Thing& t) { buf[n++] = t; } };
struct Parent { int MurderTwistEvent; Vec MagicRuneRumbleList; };
struct E { Game* G; Thing Me; Parent* ParentClass;
    void Init()
    {
        if (ParentClass->MurderTwistEvent != 7)
            G->RemoveThing(Me, false, false);
        ParentClass->MagicRuneRumbleList.push_back(Me);
    }
};
int main()
{
    g_log[0] = 0;
    bool ok = true;
    Parent p; p.MurderTwistEvent = 7; p.MagicRuneRumbleList.n = 0; E e; e.G = &g_game; e.ParentClass = &p; e.Me.Id = 5;
    e.Init(); ok = g_log[0] == 0 && p.MagicRuneRumbleList.n == 1 && p.MagicRuneRumbleList.buf[0].Id == 5;
    p.MurderTwistEvent = 3; e.Init();
    ok = ok && std::strcmp(g_log, "remove;") == 0 && (p.MagicRuneRumbleList.n == 2);
    if (!ok) std::printf("log: %s\n", g_log);
    if (!ok) { std::printf("00eae820_TEST FAIL\n"); return 1; }
    std::printf("00eae820_TEST PASS\n");
    return 0;
}