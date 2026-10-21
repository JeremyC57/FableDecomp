#include <cstdio>
#include <cstring>
// Behaviour model of CChasingScreamer::Main.
struct CScriptThing { int id; };
static int g_frame, g_terminateAt, g_startOkAt, g_heroNearAt, g_targetLog[4], g_targets, g_follow, g_releases, g_scripting;
static float g_followDist; static bool g_followRun; static char g_name[32];
struct CHandle {
    bool IsScripting() { return g_scripting != 0; }
    void Release() { ++g_releases; g_scripting = 0; }
    void FollowThing(const CScriptThing& t, float d, bool run) { g_follow = t.id; g_followDist = d; g_followRun = run; }
};
struct CGameScriptInterface {
    CScriptThing hero;
    void NewScriptFrame() const { ++g_frame; }
    bool StartScriptingEntity(const CScriptThing&, CHandle&, int priority) { if (priority == 4 && g_frame >= g_startOkAt) { g_scripting = 1; return true; } return false; }
    const CScriptThing& GetHero() const { return hero; }
    CScriptThing GetThingWithScriptName(const char* name) const { std::strcpy(g_name, name); CScriptThing t; t.id = 77; return t; }
    void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing& target) const { g_targetLog[g_targets++] = target.id; }
};
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing&, float d) { return a.id == 1 && d == 4.0f && g_frame >= g_heroNearAt; }
struct CChasingScreamer {
    CGameScriptInterface* Game; CScriptThing Me;
    bool IsActiveThreadTerminating() const { return g_frame >= g_terminateAt; }
    void Main()
    {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        CHandle scripted;
        if (scripted.IsScripting()) scripted.Release();
        while (!Game->StartScriptingEntity(Me, scripted, 4)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        CScriptThing chasedMan = Game->GetThingWithScriptName("HKC_ChasedMan");
        Game->GiveThingBestEnemyTarget(Me, chasedMan);
        scripted.FollowThing(chasedMan, 1.0f, true);
        while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 4.0f)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
        if (scripted.IsScripting()) scripted.Release();
    }
};
int main()
{
    CGameScriptInterface game; game.hero.id = 1;
    CChasingScreamer s; s.Game = &game; s.Me.id = 5;
    g_terminateAt = 100; g_startOkAt = 3; g_heroNearAt = 8;
    s.Main();
    if (std::strcmp(g_name, "HKC_ChasedMan") != 0 || g_targets != 2 || g_targetLog[0] != 77 || g_targetLog[1] != 1 || g_follow != 77
        || g_followDist != 1.0f || !g_followRun || g_releases != 1 || g_frame != 8) { std::printf("00e1af30_TEST FAIL\n"); return 1; }
    g_frame = 0; g_targets = 0; g_releases = 0; g_terminateAt = 5; g_heroNearAt = 50;   // terminates while waiting for the hero
    s.Main();
    if (g_targets != 1 || g_releases != 0 || g_frame != 5) { std::printf("00e1af30_TEST FAIL\n"); return 1; }
    std::printf("00e1af30_TEST PASS\n");
    return 0;
}
