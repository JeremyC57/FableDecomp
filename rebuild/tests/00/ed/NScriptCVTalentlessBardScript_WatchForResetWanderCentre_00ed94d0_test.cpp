// Behaviour test for 0x00ED94D0 NScript::CV_TalentlessBardScript::WatchForResetWanderCentre: the row's logic over stub classes (C++03).
#include <stdio.h>
#include <stdlib.h>
#include <string>
static void check(bool v, int line) { if (!v) { printf("00ed94d0_TEST FAIL line %d\n", line); exit(1); } }
static std::string trace;
class C3DVector { public: float X, Y, Z; };
class CScriptThing { public:
    bool Valid, Alive; C3DVector Pos;
    bool IsAlive() const { return Valid && Alive; }
    const C3DVector& GetPos() const { static C3DVector null_coord = { 0, 0, 0 }; return Valid ? Pos : null_coord; }
};
enum EScriptingStateGroups { ESSG_NONE = 0, ESSG_WANDER_NEAR = 4 };
static long g_timer = 0;
static int g_calls = 0, g_terminate_after = 1000;
class CGame { public:
    CScriptThing Hero; C3DVector Centre; float Min, Max; EScriptingStateGroups Group;
    void NewScriptFrame() { trace += '.'; if (g_timer > 0) --g_timer; }
    CScriptThing& GetHero() { return Hero; }
    void SetWanderCentrePoint(CScriptThing, C3DVector c) { Centre = c; trace += 'C'; }
    void SetWanderMinDistance(CScriptThing, float d) { Min = d; }
    void SetWanderMaxDistance(CScriptThing, float d) { Max = d; }
    void SetScriptingStateGroup(CScriptThing, EScriptingStateGroups g) { Group = g; }
};
class CScriptDef { public: float BardWanderDistanceMin, BardWanderDistanceMax; long BardWanderResetTime; };
static CScriptDef g_def = { 2.0f, 8.0f, 3 };
static CScriptDef* pGlobalScriptDef = &g_def;
class CTimer { public:
    long GetTime() const { return g_timer; }
    CTimer& operator=(long t) { g_timer = t; return *this; }
};
struct CV_TalentlessBardScript {
    CGame& Game; bool BardFollowing; CTimer BardWanderTime;
    CV_TalentlessBardScript(CGame& g) : Game(g), BardFollowing(false) { }
    bool IsActiveThreadTerminating() { return ++g_calls > g_terminate_after; }
    void WatchForResetWanderCentre(CScriptThing bard);
};
// the row's logic (src/00ed94d0.cpp; the alive predicate registration has no observable effect here)
void CV_TalentlessBardScript::WatchForResetWanderCentre(CScriptThing bard)
{
    Game.NewScriptFrame();
    do { if (IsActiveThreadTerminating()) return; } while (0);
    for (;;) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        while (BardWanderTime.GetTime() != 0) {
            Game.NewScriptFrame();
            do { if (IsActiveThreadTerminating()) return; } while (0);
        }
        if (IsActiveThreadTerminating())
            return;
        if (bard.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            if (BardFollowing) {
                if (IsActiveThreadTerminating())
                    return;
                Game.SetWanderCentrePoint(bard, Game.GetHero().GetPos());
            } else {
                if (IsActiveThreadTerminating())
                    return;
                Game.SetWanderCentrePoint(bard, bard.GetPos());
            }
            Game.SetWanderMinDistance(bard, pGlobalScriptDef->BardWanderDistanceMin);
            Game.SetWanderMaxDistance(bard, pGlobalScriptDef->BardWanderDistanceMax);
            Game.SetScriptingStateGroup(bard, ESSG_WANDER_NEAR);
            BardWanderTime = pGlobalScriptDef->BardWanderResetTime;
        }
        Game.NewScriptFrame();
    }
}
int main()
{
    CScriptThing bard = { true, true, { 1.0f, 2.0f, 3.0f } };
    CGame game;
    game.Hero.Valid = true; game.Hero.Alive = true; game.Hero.Pos.X = 10.0f; game.Hero.Pos.Y = 20.0f; game.Hero.Pos.Z = 30.0f;
    game.Centre.X = game.Centre.Y = game.Centre.Z = -1.0f; game.Min = game.Max = 0; game.Group = ESSG_NONE;
    // not following: centre on the bard, distances and state from the def, timer restarted; reset again once it runs out
    CV_TalentlessBardScript script(game);
    g_timer = 0; g_calls = 0; g_terminate_after = 12;
    script.WatchForResetWanderCentre(bard);
    check(game.Centre.X == 1.0f && game.Centre.Z == 3.0f, __LINE__);
    check(game.Min == 2.0f && game.Max == 8.0f && game.Group == ESSG_WANDER_NEAR, __LINE__);
    check(trace == ".C...C..", __LINE__);
    // following: centre on the hero
    trace = ""; script.BardFollowing = true; g_timer = 0; g_calls = 0; g_terminate_after = 6;
    script.WatchForResetWanderCentre(bard);
    check(game.Centre.X == 10.0f && game.Centre.Y == 20.0f && g_timer == 1, __LINE__);
    // dead bard: never reset, just yields
    trace = ""; bard.Alive = false; game.Centre.X = -1.0f; g_timer = 0; g_calls = 0; g_terminate_after = 7;
    script.WatchForResetWanderCentre(bard);
    check(game.Centre.X == -1.0f && trace == "....", __LINE__);
    printf("00ed94d0_TEST PASS\n");
    return 0;
}