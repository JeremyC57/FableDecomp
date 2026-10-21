#include <cstdio>
#include <cstring>
#include <string>
static std::string g_log;
static int g_frame = 0;
static int g_termAt = 1000;
static void (*g_hook)(int frame) = 0;
static void Log(const std::string& s) { char b[16]; std::sprintf(b, "@%d", g_frame); g_log += s; g_log += b; g_log += ";"; }
static void Reset(int termAt) { g_log = ""; g_frame = 0; g_termAt = termAt; g_hook = 0; }
struct CCharString {
    std::string S;
    CCharString(const char* text, long length = -1) : S(text) { (void)length; }
};
struct CScriptThing {
    std::string Name;
    CScriptThing() { }
    CScriptThing(const std::string& name) : Name(name) { }
};
struct CScriptBase {
    bool IsActiveThreadTerminating() { return g_frame >= g_termAt; }
};
static int Fail(const char* addr, const char* what) { std::printf("%s_TEST FAIL %s [%s]\n", addr, what, g_log.c_str()); return 1; }
#define CHECK(addr, cond) do { if (!(cond)) return Fail(addr, #cond); } while (0)
// behaviour test: CTollBandit::Should_I_Flee (when the bandits flee) takes over the bandit, runs it toward the escape
// marker until it is within the def's close-to-exit range plus 2, then fades it out and kills it
#include <cmath>
struct C3DVector { float X, Y, Z; };
struct CScriptDef { float TollBanditCloseToExit; float TollBanditTimeToFade; };
static CScriptDef g_def;
static CScriptDef* g_pScriptDef = &g_def;
static float g_pos = 0.0f;
static int g_taskEnds = 0;
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_4 = 4 };
enum EScriptEntityMoveType { MOVE_TYPE_WALK, MOVE_TYPE_RUN };
struct CScriptThingP : CScriptThing {
    C3DVector Pos;
    CScriptThingP() { }
    CScriptThingP(const std::string& n, float x) : CScriptThing(n) { Pos.X = x; Pos.Y = Pos.Z = 0.0f; }
    C3DVector GetPos() const { return Pos; }
};
struct CScriptGameResourceObjectScriptedThingBase {
    bool Scripting;
    bool IsScripting() const { return Scripting; }
    void Release() { Log("release"); Scripting = false; }
    CScriptThingP GetScriptThing() { return CScriptThingP("BANDIT", g_pos); }
    void MoveToPosition(const C3DVector& p, float closeTo, EScriptEntityMoveType type, bool a, bool b)
    { char t[64]; std::sprintf(t, "move %g %g %d", p.X, closeTo, (int)type); Log(t); g_pos += 10.0f; g_taskEnds = g_frame + 1; (void)a; (void)b; }
    bool IsPerformingScriptTask() { return g_frame < g_taskEnds; }
};
struct CGameScriptInterface {
    void NewScriptFrame() { ++g_frame; }
    bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& b, EScriptAIPriority p)
    { if (g_frame < 2) return false; Log("start " + thing.Name); b.Scripting = true; (void)p; return true; }
};
static bool IsDistanceFromThingToPositionOver(const CScriptThingP& thing, const C3DVector& pos, float distance)
{ return std::fabs(thing.Pos.X - pos.X) > distance; }
static void FadeOutAndKillEntity(const CScriptThing& thing, float time, bool kill, bool b)
{ char t[64]; std::sprintf(t, "fade %s %g %d", thing.Name.c_str(), time, (int)kill); Log(t); (void)b; }
struct CTollBanditScript { bool BanditsFlee; };
struct CTollBandit : CScriptBase {
    CGameScriptInterface* Game;
    CTollBanditScript* ParentClass;
    CScriptThing Me;
    CScriptThingP BanditEscapeMarker;
    void Should_I_Flee(CScriptGameResourceObjectScriptedThingBase& bandit);
};
void CTollBandit::Should_I_Flee(CScriptGameResourceObjectScriptedThingBase& bandit)
{
    if (!ParentClass->BanditsFlee)
        return;
    if (IsActiveThreadTerminating())
        return;
    if (bandit.IsScripting())
        bandit.Release();
    while (!Game->StartScriptingEntity(Me, bandit, SCRIPT_AI_PRIORITY_4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    float closeToExit = g_pScriptDef->TollBanditCloseToExit;
    C3DVector exitPos = BanditEscapeMarker.GetPos();
    float range = closeToExit + 2.0f;
    while (IsDistanceFromThingToPositionOver(bandit.GetScriptThing(), exitPos, range)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        bandit.MoveToPosition(exitPos, closeToExit, MOVE_TYPE_RUN, false, true);
        while (bandit.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    FadeOutAndKillEntity(Me, g_pScriptDef->TollBanditTimeToFade, true, true);
}
int main()
{
    CGameScriptInterface game;
    CTollBanditScript parent;
    CTollBandit script;
    script.Game = &game;
    script.ParentClass = &parent;
    script.Me = CScriptThing("ME");
    script.BanditEscapeMarker = CScriptThingP("EXIT", 25.0f);
    g_def.TollBanditCloseToExit = 3.0f;
    g_def.TollBanditTimeToFade = 1.5f;
    CScriptGameResourceObjectScriptedThingBase bandit;
    Reset(1000); parent.BanditsFlee = true; bandit.Scripting = true; g_pos = 0.0f;
    script.Should_I_Flee(bandit);
    CHECK("00e4df20", g_log == "release@0;start ME@2;move 25 3 1@3;move 25 3 1@5;fade ME 1.5 1@6;");
    Reset(1000); parent.BanditsFlee = false; bandit.Scripting = true;
    script.Should_I_Flee(bandit);
    CHECK("00e4df20", g_log == "" && bandit.Scripting);
    Reset(4); parent.BanditsFlee = true; bandit.Scripting = false; g_pos = 0.0f;
    script.Should_I_Flee(bandit);
    CHECK("00e4df20", g_log == "start ME@2;move 25 3 1@3;" && g_frame == 4);
    std::printf("00e4df20_TEST PASS\n");
    return 0;
}
