#include <cstdio>
#include <cstring>
static char g_log[8192];
static int g_frame = 0;
static int g_termAt = 1000;
static void Log(const char* s) { char b[32]; std::sprintf(b, "@%d;", g_frame); std::strcat(g_log, s); std::strcat(g_log, b); }
static void Reset(int termAt) { g_log[0] = 0; g_frame = 0; g_termAt = termAt; }
static int Fail(const char* addr, const char* what) { std::printf("%s_TEST FAIL %s [%s]\n", addr, what, g_log); return 1; }
#define CHECK(addr, cond) do { if (!(cond)) return Fail(addr, #cond); } while (0)
// behaviour test: CBSGateGuard::Main walks the guard home when he strayed more than 1 unit, then faces the hero, then idles
#include <cmath>
struct C3DVector { float X, Y, Z; };
static float g_pos = 0.0f;
static int g_taskEnds = 0;
enum EScriptAIPriority { P0 };
enum EScriptEntityMoveType { MT0 };
struct CScriptThing {
    const char* Name; float X;
    CScriptThing(const char* n = "", float x = 0.0f) : Name(n), X(x) { }
    C3DVector GetHomePos() const { C3DVector v; v.X = 10.0f; v.Y = v.Z = 0.0f; return v; }
};
static bool IsDistanceFromThingToPositionOver(const CScriptThing& t, const C3DVector& p, float d) { return std::fabs(t.X - p.X) > d; }
struct CScripted {
    bool Scripting;
    bool IsScripting() const { return Scripting; }
    void Release() { Log("release"); Scripting = false; }
    CScriptThing GetScriptThing() const { return CScriptThing("ME", g_pos); }
    void MoveToPosition(const C3DVector& p, float s, EScriptEntityMoveType t, bool a, bool b)
    { char x[64]; std::sprintf(x, "move %g %g %d%d%d", p.X, s, (int)t, (int)a, (int)b); Log(x); g_pos += 5.0f; g_taskEnds = g_frame + 1; }
    bool IsPerformingScriptTask() const { return g_frame < g_taskEnds; }
};
static CScriptThing g_hero("HERO");
struct CGame {
    void NewScriptFrame() const { ++g_frame; }
    bool StartScriptingEntity(const CScriptThing& t, CScripted& h, EScriptAIPriority p)
    { if (g_frame < 1) return false; char x[64]; std::sprintf(x, "start %s %d", t.Name, (int)p); Log(x); h.Scripting = true; return true; }
    CScriptThing& GetHero() { return g_hero; }
    void EntitySetFacingAngleTowardsThing(const CScriptThing& a, const CScriptThing& b, bool i) const
    { char x[64]; std::sprintf(x, "face %s %s %d", a.Name, b.Name, (int)i); Log(x); }
};
static bool g_scriptedInit = true;
struct CBSGateGuard {
    CGame* Game; CScriptThing Me;
    bool IsActiveThreadTerminating() const { return g_frame >= g_termAt; }
    void Main();
};
void CBSGateGuard::Main()
{
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScripted scripted; scripted.Scripting = g_scriptedInit;
    if (scripted.IsScripting())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)1)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (IsDistanceFromThingToPositionOver(Me, Me.GetHomePos(), 1.0f)) {
        if (IsActiveThreadTerminating())
            return;
        C3DVector home = Me.GetHomePos();
        while (IsDistanceFromThingToPositionOver(scripted.GetScriptThing(), home, 2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            scripted.MoveToPosition(home, 0.0f, (EScriptEntityMoveType)0, false, true);
            while (scripted.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), true);
    }
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
int main()
{
    CGame game; CBSGateGuard g; g.Game = &game;
    Reset(20); g_pos = 0.0f; g.Me = CScriptThing("ME", 0.0f);
    g.Main();
    CHECK("00cdfb00", std::strcmp(g_log, "release@1;start ME 1@1;move 10 0 001@2;move 10 0 001@4;face ME HERO 1@5;") == 0 && g_frame == 20);
    Reset(7); g_scriptedInit = false; g.Me = CScriptThing("ME", 10.5f);
    g.Main();
    CHECK("00cdfb00", std::strcmp(g_log, "start ME 1@1;") == 0 && g_frame == 7);
    Reset(3); g_pos = 0.0f; g.Me = CScriptThing("ME", 0.0f);
    g.Main();
    CHECK("00cdfb00", std::strcmp(g_log, "start ME 1@1;move 10 0 001@2;") == 0 && g_frame == 3);
    std::printf("00cdfb00_TEST PASS\n");
    return 0;
}
