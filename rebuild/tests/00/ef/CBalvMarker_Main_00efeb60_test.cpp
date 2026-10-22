#include <cstdio>
#include <cstring>
static char g_log[8192];
static int g_frame = 0;
static int g_termAt = 1000;
static void Log(const char* s) { char b[32]; std::sprintf(b, "@%d;", g_frame); std::strcat(g_log, s); std::strcat(g_log, b); }
static void Reset(int termAt) { g_log[0] = 0; g_frame = 0; g_termAt = termAt; }
static int Fail(const char* addr, const char* what) { std::printf("%s_TEST FAIL %s [%s]\n", addr, what, g_log); return 1; }
#define CHECK(addr, cond) do { if (!(cond)) return Fail(addr, #cond); } while (0)
// behaviour test: CBalvMarker::Main spawns a balverine each round, waits for the marker on screen, plays its leap animation, removes it, waits for the marker off screen
struct C3DVector { float X, Y, Z; };
enum EScriptAIPriority { P0 };
enum EScriptEntityMoveType { MT0 };
static int g_onScreenFrom = 3, g_onScreenTo = 6, g_taskEnds = 0, g_created = 0, g_alive = 0;
struct CScriptThing {
    int Id;
    explicit CScriptThing(int id = 0) : Id(id) { ++g_alive; }
    CScriptThing(const CScriptThing& o) : Id(o.Id) { ++g_alive; }
    ~CScriptThing() { --g_alive; }
    C3DVector GetPos() const { C3DVector v; v.X = 1.0f; v.Y = 2.0f; v.Z = 3.0f; return v; }
};
struct CScripted {
    CScripted() { ++g_alive; }
    ~CScripted() { --g_alive; }
    void PlayCombatAnimation(const char* a, bool, bool, bool, bool, bool, bool) { Log(a); g_taskEnds = g_frame + 2; }
    bool IsPerformingScriptTask() const { return g_frame < g_taskEnds; }
};
struct CGame {
    void NewScriptFrame() const { ++g_frame; }
    bool StartScriptingEntity(const CScriptThing& t, CScripted&, EScriptAIPriority p) { char x[32]; std::sprintf(x, "start %d %d", t.Id, (int)p); Log(x); return false; }
    CScriptThing CreateCreature(const char* def, const C3DVector& pos, const char* name, bool) const
    { char x[96]; std::sprintf(x, "create %s %s %g", def, name, pos.X); Log(x); return CScriptThing(++g_created); }
    void RemoveThing(const CScriptThing& t, bool, bool) const { char x[32]; std::sprintf(x, "remove %d", t.Id); Log(x); }
    bool IsCameraPosOnScreen(const C3DVector&) const { int f = g_frame % 10; return f >= g_onScreenFrom && f < g_onScreenTo; }
};
struct CBalvMarker {
    CGame* Game; CScriptThing Me;
    bool IsActiveThreadTerminating() const { return g_frame >= g_termAt; }
    void Main();
};
void CBalvMarker::Main()
{
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        {
            CScriptThing balv = Game->CreateCreature("CREATURE_BALVERINE_01", Me.GetPos(), "SpookyBalv", false);
            CScripted scripted;
            Game->StartScriptingEntity(balv, scripted, (EScriptAIPriority)4);
            while (!Game->IsCameraPosOnScreen(Me.GetPos())) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            scripted.PlayCombatAnimation("ATTACK_THEN_JUMP_INTO_SKY", true, false, false, true, false, false);
            while (scripted.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->RemoveThing(balv, false, true);
            while (Game->IsCameraPosOnScreen(Me.GetPos())) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        Game->NewScriptFrame();
    }
}
int main()
{
    CGame game; CBalvMarker b; b.Game = &game;
    Reset(15); g_created = 0;
    b.Main();
    CHECK("00efeb60", std::strcmp(g_log, "create CREATURE_BALVERINE_01 SpookyBalv 1@1;start 1 4@1;ATTACK_THEN_JUMP_INTO_SKY@3;remove 1@5;"
        "create CREATURE_BALVERINE_01 SpookyBalv 1@7;start 2 4@7;ATTACK_THEN_JUMP_INTO_SKY@13;") == 0 && g_frame == 15 && g_alive == 1);
    Reset(1); g_created = 0;
    b.Main();
    CHECK("00efeb60", g_log[0] == 0 && g_alive == 1);
    std::printf("00efeb60_TEST PASS\n");
    return 0;
}