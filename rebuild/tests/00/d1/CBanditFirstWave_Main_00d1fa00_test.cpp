#include <cstdio>
#include <cstring>
static char g_log[8192];
static int g_frame = 0;
static int g_termAt = 1000;
static void Log(const char* s) { char b[32]; std::sprintf(b, "@%d;", g_frame); std::strcat(g_log, s); std::strcat(g_log, b); }
static void Reset(int termAt) { g_log[0] = 0; g_frame = 0; g_termAt = termAt; }
static int Fail(const char* addr, const char* what) { std::printf("%s_TEST FAIL %s [%s]\n", addr, what, g_log); return 1; }
#define CHECK(addr, cond) do { if (!(cond)) return Fail(addr, #cond); } while (0)
// behaviour test: CBanditFirstWave::Main takes Me over, runs to the leader until within 5 units (or the leader is freed), frees the leader and follows him
#include <cmath>
enum EScriptAIPriority { P0 };
enum EScriptEntityMoveType { MT0 };
static float g_pos = 0.0f;
static int g_taskEnds = 0;
class CScriptThing { public:
    const char* Name; float X;
    CScriptThing(const char* n = "", float x = 0.0f) : Name(n), X(x) { }
    struct InlineCopy { };
    CScriptThing(const CScriptThing& o, InlineCopy) : Name(o.Name), X(o.X) { }
};
struct CQ_BreakSiegeScript { bool BanditLeaderFree, StartSecondWave, HeroLetInTown; };
static CQ_BreakSiegeScript g_parent;
static int g_letInAt = 2, g_waveAt = 4;
class CScriptDef { public: float BanditAttackerBreakOffRange; };
static CScriptDef g_def;
static CScriptDef* g_pScriptDef = &g_def;
static bool IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float d) { (void)a; return std::fabs(g_pos - b.X) > d; }
struct CScripted {
    bool Scripting;
    CScripted() : Scripting(false) { }
    bool IsScripting() const { return Scripting; }
    void Release() { Log("release"); Scripting = false; }
    void MoveToThing(const CScriptThing& t, float d, EScriptEntityMoveType m, bool a, bool b, bool c, bool e)
    { char x[64]; std::sprintf(x, "move %s %g %d%d%d%d%d", t.Name, d, (int)m, (int)a, (int)b, (int)c, (int)e); Log(x); g_pos += 4.0f; g_taskEnds = g_frame + 2; }
    bool IsPerformingScriptTask() const { return g_frame < g_taskEnds; }
};
class CGame { public:
    void NewScriptFrame() const { ++g_frame; if (g_frame >= g_letInAt) g_parent.HeroLetInTown = true; if (g_frame >= g_waveAt) g_parent.StartSecondWave = true; }
    bool StartScriptingEntity(const CScriptThing& t, CScripted& h, EScriptAIPriority p)
    { char x[64]; std::sprintf(x, "start %s %d", t.Name, (int)p); Log(x); h.Scripting = true; return true; }
    CScriptThing GetThingWithScriptName(const char* n) const { return CScriptThing(n, 12.0f); }
    void SetCombatNearbyBreakOffRange(CScriptThing t, float r) { char x[64]; std::sprintf(x, "range %s %g", t.Name, r); Log(x); }
    void EntityFollowThing(const CScriptThing& a, const CScriptThing& b, float d, bool f) const
    { char x[64]; std::sprintf(x, "follow %s %s %g %d", a.Name, b.Name, d, (int)f); Log(x); }
};
struct CBanditFirstWave {
    CGame* Game; CScriptThing Me; CQ_BreakSiegeScript* ParentClass;
    bool IsActiveThreadTerminating() const { return g_frame >= g_termAt; }
    void Main();
};
void CBanditFirstWave::Main()
{
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScripted scripted;
    if (scripted.IsScripting())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)3)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetCombatNearbyBreakOffRange(CScriptThing(Me, CScriptThing::InlineCopy()), g_pScriptDef->BanditAttackerBreakOffRange);
    CScriptThing leader = Game->GetThingWithScriptName("ImprisonedBanditLeader");
    while (IsDistanceBetweenThingsOver(Me, leader, 5.0f) && !ParentClass->BanditLeaderFree) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!scripted.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating())
                return;
            scripted.MoveToThing(leader, 0.0f, (EScriptEntityMoveType)1, false, true, false, true);
        }
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->BanditLeaderFree = true;
    if (scripted.IsScripting())
        scripted.Release();
    Game->EntityFollowThing(Me, leader, 1.0f, true);
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
int main()
{
    CGame game; CBanditFirstWave b; b.Game = &game; b.Me = CScriptThing("ME"); b.ParentClass = &g_parent;
    g_def.BanditAttackerBreakOffRange = 7.5f;
    Reset(30); g_pos = 0.0f; g_taskEnds = 0;
    b.Main();
    CHECK("00d1fa00", std::strcmp(g_log, "start ME 3@1;range ME 7.5@1;move ImprisonedBanditLeader 0 10101@2;move ImprisonedBanditLeader 0 10101@4;release@4;follow ME ImprisonedBanditLeader 1 1@4;") == 0);
    CHECK("00d1fa00", g_parent.BanditLeaderFree && g_frame == 30);
    Reset(1); g_parent.BanditLeaderFree = false; g_pos = 0.0f; g_taskEnds = 0;
    b.Main();
    CHECK("00d1fa00", g_log[0] == 0 && g_frame == 1 && !g_parent.BanditLeaderFree);
    Reset(3); g_parent.BanditLeaderFree = false; g_pos = 0.0f; g_taskEnds = 0;
    b.Main();
    CHECK("00d1fa00", std::strcmp(g_log, "start ME 3@1;range ME 7.5@1;move ImprisonedBanditLeader 0 10101@2;") == 0 && !g_parent.BanditLeaderFree);
    std::printf("00d1fa00_TEST PASS\n");
    return 0;
}