#include <cstdio>
#include <string>
// Behaviour model of CAT_Phase1Guard::Init: hero/guard enmity both ways, then each living bandit both ways
// (an early return when the thread is done), then one spawned WatchForEnemyDeath thread.
static std::string g_log;
struct CScriptThing { int id; bool alive; };
struct CGame {
    CScriptThing hero;
    const CScriptThing& GetHero() const { return hero; }
    void EntitySetThingAsEnemyOfThing(const CScriptThing& a, const CScriptThing& b) const
    { char x[32]; std::sprintf(x, "%d>%d;", a.id, b.id); g_log += x; }
};
struct CParent {
    CScriptThing* first; CScriptThing* last; int spawned;
    unsigned long size() const { return (unsigned long)(last - first); }
};
struct CGuard {
    CGame* Game; CScriptThing Me; CParent* ParentClass; int doneAfter; int doneCalls;
    bool IsDone() { return ++doneCalls > doneAfter; }
    void Init()
    {
        Game->EntitySetThingAsEnemyOfThing(Me, Game->GetHero());
        Game->EntitySetThingAsEnemyOfThing(Game->GetHero(), Me);
        for (unsigned long i = 0; i < ParentClass->size(); ++i) {
            if (ParentClass->first[i].alive) {
                if (IsDone())
                    return;
                Game->EntitySetThingAsEnemyOfThing(Me, ParentClass->first[i]);
                Game->EntitySetThingAsEnemyOfThing(ParentClass->first[i], Me);
            }
        }
        ++ParentClass->spawned;
    }
};
int main()
{
    CScriptThing bandits[4] = { { 10, true }, { 11, false }, { 12, true }, { 13, true } };
    CGame g; g.hero.id = 1; g.hero.alive = true;
    CParent p; p.first = bandits; p.last = bandits + 4; p.spawned = 0;
    CGuard guard; guard.Game = &g; guard.Me.id = 5; guard.Me.alive = true; guard.ParentClass = &p;
    guard.doneAfter = 100; guard.doneCalls = 0;
    guard.Init();
    if (g_log != "5>1;1>5;5>10;10>5;5>12;12>5;5>13;13>5;" || p.spawned != 1 || guard.doneCalls != 3) {
        std::printf("00f083a0_TEST FAIL\n"); return 1;
    }
    g_log = ""; guard.doneAfter = 1; guard.doneCalls = 0;
    guard.Init();   // done on the second living bandit: no spawn
    if (g_log != "5>1;1>5;5>10;10>5;" || p.spawned != 1) { std::printf("00f083a0_TEST FAIL\n"); return 1; }
    std::printf("00f083a0_TEST PASS\n");
    return 0;
}