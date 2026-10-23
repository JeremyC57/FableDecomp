// behaviour test for 00e49980 CBlockingBandit::Main (the same control flow over a mocked engine)
#include <cstdio>
#include <cstring>
struct C3DVector { float X, Y, Z; };
struct CScriptThing { int id; C3DVector pos; bool alive; const C3DVector& GetPos() const { return pos; } bool IsAlive() const { return alive; } };
struct CGame;
static CGame* g_game;
struct CGame {
    int frame, terminateAt, fades, follows, factionSet, released;
    float lastSpeed; CScriptThing hero, escape, camp, self;
    bool flee, angered, paid;
    void NewScriptFrame() { ++frame; if (frame == 4) flee = false; }
    const CScriptThing& GetHero() const { return hero; }
    CScriptThing GetThingWithScriptName(const char* n) const { return std::strcmp(n, "M_BanditEscapePoint") == 0 ? escape : camp; }
    bool StartScriptingEntity() { return true; }
    void EntitySetInFaction(const CScriptThing&, const char* f) { if (std::strcmp(f, "FACTION_BANDITS") == 0) ++factionSet; }
    bool IsThingAwareOfOtherThingInAnyWay(const CScriptThing&, const CScriptThing&) const { return true; }
};
struct CScripted {
    bool scripting; int busy; C3DVector at;
    bool IsScripting() { return scripting; }
    void Release() { scripting = false; ++g_game->released; }
    void MoveToPosition(const C3DVector& p, float s, int, bool, bool) { at = p; busy = 2; g_game->lastSpeed = s; }
    void FollowThing(const CScriptThing&, float, bool) { ++g_game->follows; }
    bool IsPerformingScriptTask() { return busy-- > 0; }
    CScriptThing GetScriptThing() const { CScriptThing t = g_game->self; t.pos = at; return t; }
};
static bool IsDistanceFromThingToPositionOver(const CScriptThing& t, const C3DVector& p, float d) { float dx = t.pos.X - p.X; if (dx < 0) dx = -dx; return dx > d; }
static void FadeOutAndKillEntity(const CScriptThing&, bool, float, bool) { ++g_game->fades; }
struct CQ { bool BanditsAngered, TollPaid, BanditsFlee; };
struct CBlockingBandit {
    CGame* Game; CScriptThing Me; CQ* ParentClass; float closeToExit, fadeTime;
    bool IsActiveThreadTerminating() { return Game->frame >= Game->terminateAt; }
    void Main();
};
void CBlockingBandit::Main()
{
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScripted scripted; scripted.scripting = false; scripted.busy = 0; scripted.at = Me.pos;
    if (scripted.IsScripting()) scripted.Release();
    while (!Game->StartScriptingEntity()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    bool returnedToCamp = false;
    CScriptThing escapePoint = Game->GetThingWithScriptName("M_BanditEscapePoint");
    while (!IsActiveThreadTerminating()) {
        if (ParentClass->BanditsFlee) {
            if (IsActiveThreadTerminating()) return;
            float close = closeToExit;
            C3DVector exitPos = escapePoint.GetPos();
            while (IsDistanceFromThingToPositionOver(scripted.GetScriptThing(), exitPos, close + 2.0f)) {
                Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;
                scripted.MoveToPosition(exitPos, close, 1, false, true);
                while (scripted.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            FadeOutAndKillEntity(Me, true, fadeTime, true);
            ParentClass->BanditsFlee = false;
        }
        if (ParentClass->BanditsAngered && Me.IsAlive() && Game->IsThingAwareOfOtherThingInAnyWay(Game->GetHero(), Me)) {
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetInFaction(Me, "FACTION_BANDITS");
            scripted.FollowThing(Game->GetHero(), 1.0f, true);
        }
        if (ParentClass->TollPaid && !returnedToCamp) {
            if (IsActiveThreadTerminating()) return;
            CScriptThing campCentre = Game->GetThingWithScriptName("M_BanditCampCentrePos");
            C3DVector campPos = campCentre.GetPos();
            while (IsDistanceFromThingToPositionOver(scripted.GetScriptThing(), campPos, 4.0f)) {
                Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;
                scripted.MoveToPosition(campPos, 2.0f, 0, false, true);
                while (scripted.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            scripted.Release();
            returnedToCamp = true;
        }
        Game->NewScriptFrame();
    }
}
int main()
{
    int ok = 1;
    CGame g; std::memset(&g, 0, sizeof(g)); g_game = &g; g.terminateAt = 30; g.escape.pos.X = 50.0f; g.camp.pos.X = 20.0f; g.self.alive = true;
    CQ q; q.BanditsAngered = false; q.TollPaid = true; q.BanditsFlee = false;
    CBlockingBandit b; b.Game = &g; b.Me = g.self; b.ParentClass = &q; b.closeToExit = 3.0f; b.fadeTime = 1.5f;
    b.Main();
    if (g.released != 1 || g.lastSpeed != 2.0f || g.fades != 0 || g.follows != 0) ok = 0;   // walked to camp once
    std::memset(&g, 0, sizeof(g)); g.terminateAt = 10; g.escape.pos.X = 50.0f; g.self.alive = true;
    q.TollPaid = false; q.BanditsFlee = true; q.BanditsAngered = true;
    b.Main();
    if (g.fades != 1 || g.lastSpeed != 3.0f || g.factionSet == 0 || g.follows == 0) ok = 0;
    if (!ok) { std::printf("00e49980_TEST FAIL\n"); return 1; }
    std::printf("00e49980_TEST PASS\n");
    return 0;
}