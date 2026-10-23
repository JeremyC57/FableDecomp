// behaviour test for 00d15510 CBH_GangGruntSpawn::Main (the same control flow over a mocked engine)
#include <cstdio>
#include <cstring>
#include <cstdlib>
struct C3DVector { float X, Y, Z; };
static C3DVector g_ZeroVector = { 0, 0, 0 };
struct CCharString {
    const char* text;
    CCharString() : text("") { }
    CCharString(const char* t, long length = -1) : text(t) { (void)length; }
    bool operator!=(const char* t) const { return std::strcmp(text, t) != 0; }
};
static long GFCharStringToInt(const CCharString& s) { return std::atol(s.text); }
enum EHeroAbility { HERO_ABILITY_14 = 14 };
struct CScriptThing {
    int id; bool alive; C3DVector pos; const char* data;
    int hitFrame;                                      // frame at which the hero hits it (-1 never)
    CCharString GetDataString() const { return CCharString(data); }
    const C3DVector& GetPos() const { return pos; }
    bool MsgIsHitBy(const CCharString&) const;
    bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&) const { return false; }
    bool MsgIsHitBySpecialAbilityFrom(EHeroAbility, const CCharString&) const { return false; }
    bool IsAlive() const { return alive; }
};
struct CGame {
    int frame; long timer[4]; int startTries; CScriptThing hero, exitPoint;
    int targetOf; int fades; C3DVector moveTarget; int terminateAt;
    bool questSucceeded;
    void NewScriptFrame() { ++frame; if (frame == 6) questSucceeded = true; }
    const CScriptThing& GetHero() const { return hero; }
    bool CanThingBeSeenByOtherThing(const CScriptThing&, const CScriptThing&) const { return false; }
    void SetTimer(long h, long t) { timer[h] = t; }
    long GetTimer(long h) { if (timer[h] > 0) --timer[h]; return timer[h]; }
    void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing& t) { targetOf = t.id; }
    CScriptThing GetNearestWithDefName(const CScriptThing&, const CCharString&) const { return exitPoint; }
    bool StartScriptingEntity() { return ++startTries >= 2; }
    bool MsgOnRegionLoaded(CCharString&) const { return false; }
};
static CGame* g_game;
bool CScriptThing::MsgIsHitBy(const CCharString&) const { return hitFrame >= 0 && g_game->frame >= hitFrame; }
struct CScripted {
    bool scripting;
    bool IsScripting() { return scripting; }
    void Release() { scripting = false; }
    void MoveToPosition(const C3DVector& p, float, int, bool, bool) { g_game->moveTarget = p; }
};
static bool IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float) { return g_game->frame >= 9; }
static void FadeOutAndKillEntity(const CScriptThing&, bool, float, bool) { ++g_game->fades; }
struct CTimer {
    long Handle;
    void SetTime(long t) { g_game->SetTimer(Handle, t); }
    long GetTime() const { return g_game->GetTimer(Handle); }
};
struct CBH_GangGruntSpawn {
    CGame* Game; CScriptThing Me; CTimer WaitForTimer;
    bool IsActiveThreadTerminating() { return Game->frame >= Game->terminateAt; }
    void Main();
};
void CBH_GangGruntSpawn::Main()
{
    CScripted scripted; scripted.scripting = true;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CCharString data = Me.GetDataString();
    if (data != "" && data != "NULL") {
        if (IsActiveThreadTerminating())
            return;
        WaitForTimer.SetTime(GFCharStringToInt(data));
    }
    while (!Me.MsgIsHitBy("SCRIPT_NAME_HERO")
           && !(Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_14, "SCRIPT_NAME_HERO"))
           && !Game->CanThingBeSeenByOtherThing(Game->GetHero(), Me)
           && WaitForTimer.GetTime() != 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    while (!Game->questSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing exitPoint = Game->GetNearestWithDefName(Me, "REGION_EXIT_POINT");
    CCharString region;
    if (scripted.IsScripting())
        scripted.Release();
    while (!Game->StartScriptingEntity()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!exitPoint.IsAlive())
        return;
    scripted.scripting = true;
    if (scripted.IsScripting()) {
        if (IsActiveThreadTerminating())
            return;
        scripted.MoveToPosition(exitPoint.GetPos(), 1.0f, 1, false, true);
    }
    while (!IsDistanceBetweenThingsUnder(Me, exitPoint, 2.0f) && !Game->MsgOnRegionLoaded(region)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    FadeOutAndKillEntity(Me, true, 1.0f, true);
}
static CGame make()
{
    CGame g; std::memset(&g, 0, sizeof(g));
    g.hero.id = 1; g.exitPoint.id = 2; g.exitPoint.alive = true; g.exitPoint.pos.X = 5.0f; g.terminateAt = 100;
    g.hero.hitFrame = -1; g.exitPoint.hitFrame = -1;
    return g;
}
int main()
{
    int ok = 1;
    // timer from the data string runs out: the grunt targets the hero, walks out and fades
    CGame g = make(); g_game = &g;
    CBH_GangGruntSpawn s; s.Game = &g; s.Me.id = 3; s.Me.data = "3"; s.Me.hitFrame = -1; s.WaitForTimer.Handle = 1;
    s.Main();
    if (g.targetOf != 1 || g.fades != 1 || g.moveTarget.X != 5.0f || g.timer[1] != 0) ok = 0;
    // terminated while waiting: no target, no fade
    CGame h = make(); h.terminateAt = 2; g_game = &h;
    CBH_GangGruntSpawn t; t.Game = &h; t.Me.id = 3; t.Me.data = "NULL"; t.Me.hitFrame = -1; t.WaitForTimer.Handle = 2; h.timer[2] = 50;
    t.Main();
    if (h.targetOf != 0 || h.fades != 0) ok = 0;
    if (!ok) { std::printf("00d15510_TEST FAIL\n"); return 1; }
    std::printf("00d15510_TEST PASS\n");
    return 0;
}