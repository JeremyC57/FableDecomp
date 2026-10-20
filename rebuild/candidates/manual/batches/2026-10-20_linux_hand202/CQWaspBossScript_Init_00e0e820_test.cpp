// Behaviour test for CQ_WaspBossScript::Init (0x00E0E820).
#include <cstdio>
#include <cstring>
// Test doubles: strings keep their literal, a script thing is a named handle whose liveness is shared with the
// test, the game script interface records calls in g_log, and the thread terminates after a set number of tests.
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static void ResetLog() { g_log[0] = 0; }
struct CCharString {
    const char* Text;
    CCharString(const char* text, long = -1) : Text(text) {}
};
struct CWideString {
    const void* Text;
    CWideString(const void* text) : Text(text) {}
};
struct CScriptThing {
    const char* Name;
    const bool* AliveFlag;
    CScriptThing() : Name(""), AliveFlag(0) {}
    CScriptThing(const char* n, const bool* a) : Name(n), AliveFlag(a) {}
    bool IsAlive() const { return AliveFlag && *AliveFlag; }
};
struct CScriptBase {
    long TerminateAfter;                                 // IsActiveThreadTerminating() turns true after this many calls
    long TerminateCalls;
    CScriptBase() : TerminateAfter(1000), TerminateCalls(0) {}
    bool IsActiveThreadTerminating() { return ++TerminateCalls > TerminateAfter; }
};
// Frame-driven world: NewScriptFrame advances the frame counter and fires one scheduled flag change.
struct CFrameEvents {
    long Frames;
    bool* Flag[4]; long At[4]; bool Value[4];
    CFrameEvents() : Frames(0) { for (int i = 0; i < 4; ++i) { Flag[i] = 0; At[i] = 0; Value[i] = false; } }
    void Schedule(int i, bool* f, long at, bool v) { Flag[i] = f; At[i] = at; Value[i] = v; }
    void Tick() { ++Frames; for (int i = 0; i < 4; ++i) if (Flag[i] && At[i] == Frames) *Flag[i] = Value[i]; }
};
struct C2DCoordI { long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) {} };
struct CGameScriptInterface {
    long Timers[4];
    void SetTimer(long i, long t) { Timers[i] = t; }
    void AddQuestRegion(const CCharString& q, const CCharString& r) { char t[96]; std::sprintf(t, "%s>%s", q.Text, r.Text); Log(t); }
    void SetQuestWorldMapOffset(const CCharString& q, const C2DCoordI& c) { char t[96]; std::sprintf(t, "%s@%ld,%ld", q.Text, c.X, c.Y); Log(t); }
};
static CGameScriptInterface g_global;
static CGameScriptInterface* g_pGameScriptInterface = &g_global;
struct CTimer {
    long TimerIndex;
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
};
struct CQuestScriptBase : CScriptBase { CGameScriptInterface* GSI; };
struct CQ_WaspBossScript : CQuestScriptBase {
    bool MissionSucceeded, MissionFailed, DronesCreated, QueenHornetAttacks, CutsceneFinished, SaidLineOnce, PanickedVillagersScene, StartChase;
    long SavedVillagerCount;
    bool QuestStartScreened;
    CTimer ReachedWaspHelper;
    void Init();
};
void CQ_WaspBossScript::Init()
{
    GSI->AddQuestRegion("Q_WaspBoss", "PicnicArea");
    GSI->SetQuestWorldMapOffset("Q_WaspBoss", C2DCoordI(10, 10));
    PanickedVillagersScene = false;
    MissionSucceeded = false;
    MissionFailed = false;
    DronesCreated = false;
    SaidLineOnce = false;
    QueenHornetAttacks = false;
    CutsceneFinished = false;
    StartChase = false;
    QuestStartScreened = false;
    SavedVillagerCount = 0;
    ReachedWaspHelper.SetTime(0);
}
int main()
{
    CGameScriptInterface g; CQ_WaspBossScript q; q.GSI = &g;
    std::memset(&q.MissionSucceeded, 1, 8); q.SavedVillagerCount = 6; q.QuestStartScreened = true; q.ReachedWaspHelper.TimerIndex = 1; g_global.Timers[1] = 77;
    ResetLog(); q.Init();
    int bad = std::strcmp(g_log, "Q_WaspBoss>PicnicArea;Q_WaspBoss@10,10;") != 0 || g_global.Timers[1] != 0 || q.SavedVillagerCount != 0 || q.QuestStartScreened;
    const unsigned char* f = (const unsigned char*)&q.MissionSucceeded;
    for (int i = 0; i < 8; ++i) if (f[i] != 0) bad = 1;
    if (bad) { std::printf("00e0e820_TEST FAIL\n"); return 1; }
    std::printf("00e0e820_TEST PASS\n");
    return 0;
}
