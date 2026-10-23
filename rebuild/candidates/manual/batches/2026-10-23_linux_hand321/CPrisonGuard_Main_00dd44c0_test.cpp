#include <cstdio>
#include <string>
#include <vector>
// behaviour test: CPrisonGuard::Main -- waits for the prison cutscene, then (unless the hero stole the key, which removes the
// guard) walks to GuardPatrolExit and back home, announces the race in three lines, waits out the conversation and sets
// RaceStarting
static int g_frames = 0, g_endAt = 100, g_cutsceneAt = 3, g_convEndsAt = 0;
static std::vector<std::string> g_log;
struct C3DVector { float X, Y, Z; };
static float g_guardX = 0.0f;                          // the guard walks one unit along X per frame while moving
static float g_targetX = 0.0f;
static bool g_moving = false;
struct CScriptThing {
    C3DVector pos, home;
    const C3DVector& GetPos() const { return pos; }
    C3DVector GetHomePos() const { return home; }
};
struct CHandle {
    void MoveToPosition(const C3DVector& p, float, long, bool, bool) { g_targetX = p.X; g_moving = true; g_log.push_back("move"); }
    bool IsPerformingScriptTask() const { return g_moving; }
    CScriptThing GetScriptThing() const { CScriptThing t; t.pos.X = g_guardX; t.pos.Y = t.pos.Z = 0; t.home = t.pos; return t; }
};
static bool IsDistanceFromThingToPositionOver(const CScriptThing& t, const C3DVector& p, float d)
{ float dx = t.GetPos().X - p.X; if (dx < 0) dx = -dx; return dx > d; }
struct CQ_PrisonEscapeRescueMotherScript { bool PrisonCutsceneFinished, RaceStarting; };
struct CQ_SunnyvaleMasterData { bool PrisonKeyStolenByHero; };
static CQ_PrisonEscapeRescueMotherScript* g_quest;
struct CGameScriptInterface {
    CScriptThing hero, exitPoint;
    void NewScriptFrame() {
        ++g_frames;
        if (g_frames >= g_cutsceneAt) g_quest->PrisonCutsceneFinished = true;
        if (g_moving) { if (g_guardX < g_targetX) g_guardX += 1.0f; else if (g_guardX > g_targetX) g_guardX -= 1.0f; if (g_guardX == g_targetX) g_moving = false; }
    }
    bool StartScriptingEntity(const CScriptThing&, CHandle&) { return true; }
    CScriptThing& GetHero() { return hero; }
    CScriptThing GetThingWithScriptName(const char*) const { return exitPoint; }
    void RemoveThing(const CScriptThing&, bool, bool) { g_log.push_back("removed"); }
    void EntitySetCutsceneBehaviour(const CScriptThing&, long) {}
    long AddNewConversation(const CScriptThing&, bool, bool) { g_convEndsAt = g_frames + 2; return 5; }
    void AddPersonToConversation(long, const CScriptThing&) {}
    void AddLineToConversation(long, const char* text, bool, const CScriptThing&, const CScriptThing&) { g_log.push_back(text); }
    bool IsConversationActive(long) const { return g_frames < g_convEndsAt; }
    void Pause(float) {}
};
struct CPrisonGuard {
    CGameScriptInterface* Game; CScriptThing Me; CQ_PrisonEscapeRescueMotherScript* ParentClass; CQ_SunnyvaleMasterData* PMasterData;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void Main();
};
void CPrisonGuard::Main()
{
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CHandle guard;
    ParentClass->PrisonCutsceneFinished = false;
    while (!ParentClass->PrisonCutsceneFinished) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    Game->StartScriptingEntity(Me, guard);
    if (PMasterData->PrisonKeyStolenByHero) { Game->RemoveThing(Me, false, true); return; }
    Game->EntitySetCutsceneBehaviour(Me, 1);
    {
        C3DVector exit = Game->GetThingWithScriptName("GuardPatrolExit").GetPos();
        while (IsDistanceFromThingToPositionOver(guard.GetScriptThing(), exit, 2.0f)) {
            Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;
            guard.MoveToPosition(exit, 0.0f, 0, false, true);
            while (guard.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        }
    }
    {
        C3DVector home = Me.GetHomePos();
        while (IsDistanceFromThingToPositionOver(guard.GetScriptThing(), home, 2.0f)) {
            Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;
            guard.MoveToPosition(home, 0.0f, 0, false, true);
            while (guard.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        }
    }
    Game->Pause(2.0f);
    long c = Game->AddNewConversation(Me, false, false);
    Game->AddPersonToConversation(c, Game->GetHero());
    Game->AddLineToConversation(c, "TEXT_QST_055_PRISON_START_RACE_01", false, Me, Game->GetHero());
    Game->AddLineToConversation(c, "TEXT_QST_055_PRISON_START_RACE_02", false, Me, Game->GetHero());
    Game->AddLineToConversation(c, "TEXT_QST_055_PRISON_START_RACE_03", false, Me, Game->GetHero());
    while (Game->IsConversationActive(c)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    Game->Pause(2.0f);
    ParentClass->RaceStarting = true;
    do { Game->NewScriptFrame(); } while (!IsActiveThreadTerminating());
}
int main()
{
    CQ_PrisonEscapeRescueMotherScript quest; quest.PrisonCutsceneFinished = true; quest.RaceStarting = false; g_quest = &quest;
    CQ_SunnyvaleMasterData data; data.PrisonKeyStolenByHero = false;
    CGameScriptInterface game; game.exitPoint.pos.X = 10.0f; game.exitPoint.pos.Y = game.exitPoint.pos.Z = 0;
    CPrisonGuard g; g.Game = &game; g.ParentClass = &quest; g.PMasterData = &data;
    g.Me.home.X = 0; g.Me.home.Y = g.Me.home.Z = 0;
    g.Main();
    // moves out (one move, walked to within 2 of x=10), then home, then the three lines
    bool ok = quest.RaceStarting && g_log.size() == 5 && g_log[0] == "move" && g_log[1] == "move" && g_log[4] == "TEXT_QST_055_PRISON_START_RACE_03"
        && g_frames == 100;
    g_log.clear(); g_frames = 0; data.PrisonKeyStolenByHero = true; quest.RaceStarting = false;
    g.Main();
    ok = ok && g_log.size() == 1 && g_log[0] == "removed" && !quest.RaceStarting;
    if (!ok) { std::printf("00dd44c0_TEST FAIL\n"); return 1; }
    std::printf("00dd44c0_TEST PASS\n");
    return 0;
}
