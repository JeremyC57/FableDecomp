#include <cstdio>
#include <cstring>
// behaviour test: the arena CCham::Main sets Me up (no opinion source, hero faction, unkillable, no combo), waits for
// ChamLeaving, takes Me under scripted control, walks to the door marker and removes Me after the walk
static char g_log[4096];
static int g_frame = 0, g_termAt = 1000, g_leaveAt = 3, g_scriptAt = 5, g_walkFrames = 2, g_walkEnd = -1, g_released = 0;
static void Log(const char* s) { char b[16]; std::sprintf(b, "@%d;", g_frame); std::strcat(g_log, s); std::strcat(g_log, b); }
static int Fail(const char* what) { std::printf("00f14770_TEST FAIL %s [%s]\n", what, g_log); return 1; }
#define CHECK(cond) do { if (!(cond)) return Fail(#cond); } while (0)
struct C3DVector { float X, Y, Z; };
struct CScriptThing {
    const char* Name; C3DVector Pos;
    CScriptThing(const char* n = "") : Name(n) { Pos.X = 7.0f; Pos.Y = Pos.Z = 0.0f; }
    const C3DVector& GetPos() const { return Pos; }
};
struct CHandle {
    bool Scripting;
    CHandle() : Scripting(false) { }
    ~CHandle() { ++g_released; }
    bool IsScripting() { return Scripting; }
    void Release() { Scripting = false; }
    void MoveToPosition(const C3DVector& p, float speed, int type, bool, bool) { char x[64]; std::sprintf(x, "move %d %d %d", (int)p.X, (int)speed, type); Log(x); g_walkEnd = g_frame + g_walkFrames; }
    bool IsPerformingScriptTask() const { return g_frame < g_walkEnd; }
};
struct CArena { bool ChamLeaving; };
static CArena g_arena;
struct CGame {
    void NewScriptFrame() const { ++g_frame; g_arena.ChamLeaving = g_frame >= g_leaveAt; }
    bool StartScriptingEntity(const CScriptThing&, CHandle& h, int prio) { if (g_frame < g_scriptAt) return false; h.Scripting = true; char x[32]; std::sprintf(x, "script %d", prio); Log(x); return true; }
    CScriptThing GetThingWithScriptName(const char* n) const { return CScriptThing(n); }
    void RemoveThing(const CScriptThing& t, bool, bool b) const { char x[64]; std::sprintf(x, "remove %s %d", t.Name, (int)b); Log(x); }
    void EntitySetAsKillable(const CScriptThing&, bool k, bool) const { char x[32]; std::sprintf(x, "killable %d", (int)k); Log(x); }
    void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&, bool a) const { char x[32]; std::sprintf(x, "combo %d", (int)a); Log(x); }
    void EntitySetInFaction(const CScriptThing&, const char* f) const { Log(f); }
    void EntityUnsetAsOpinionSource(const CScriptThing&, bool) const { Log("noopinion"); }
};
struct CCham {
    CGame* Game; CScriptThing Me; CArena* ParentClass;
    bool IsActiveThreadTerminating() const { return g_frame >= g_termAt; }
    void Main();
};
void CCham::Main()
{
    CHandle scripted;
    Game->EntityUnsetAsOpinionSource(Me, false);
    Game->EntitySetInFaction(Me, "FACTION_HERO");
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    while (!ParentClass->ChamLeaving) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (scripted.IsScripting())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, 4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    scripted.MoveToPosition(Game->GetThingWithScriptName("GuardingDoorMarkerLeft").GetPos(), 1.0f, 1, false, true);
    while (scripted.IsPerformingScriptTask()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(Me, false, true);
}
int main()
{
    CGame game; CCham c; c.Game = &game; c.Me = CScriptThing("CHAM"); c.ParentClass = &g_arena; g_arena.ChamLeaving = false;
    c.Main();
    CHECK(std::strcmp(g_log, "noopinion@0;FACTION_HERO@0;killable 0@0;combo 0@0;script 4@5;move 7 1 1@5;remove CHAM 1@7;") == 0 && g_released == 1);
    g_log[0] = 0; g_frame = 0; g_termAt = 2; g_arena.ChamLeaving = false;
    c.Main();
    CHECK(std::strcmp(g_log, "noopinion@0;FACTION_HERO@0;killable 0@0;combo 0@0;") == 0 && g_frame == 2 && g_released == 2);
    std::printf("00f14770_TEST PASS\n");
    return 0;
}
