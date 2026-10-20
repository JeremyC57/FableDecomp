// Behaviour test for CQ_WizardBattleScript::WatchForLeaving (0x00E1C4D0).
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
struct CHero {
    bool Killed;
    bool MsgIsKilledBy(const CCharString& killer) const { Log(killer.Text[0] ? killer.Text : "<any>"); return Killed; }
};
struct CGameScriptInterface { CFrameEvents Ev; CHero Hero; void NewScriptFrame() { Ev.Tick(); } CHero& GetHero() { return Hero; } };
struct CQuestScriptBase : CScriptBase { CGameScriptInterface* GSI; };
struct CQ_WizardBattleScript : CQuestScriptBase {
    bool VillagersTurnOnHero, VillagersChased, FightersStart, MissionSucceeded, MissionFailed, MissionOver;
    void WatchForLeaving();
};
void CQ_WizardBattleScript::WatchForLeaving()
{
    while (!GSI->GetHero().MsgIsKilledBy("") && !MissionFailed && !MissionSucceeded) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (!IsActiveThreadTerminating() && !MissionSucceeded)
        MissionFailed = true;
}
int main()
{
    int bad = 0;
    {   // hero killed on frame 3: the mission fails
        CGameScriptInterface g; CQ_WizardBattleScript q; q.GSI = &g; g.Hero.Killed = false; q.MissionFailed = q.MissionSucceeded = false;
        g.Ev.Schedule(0, &g.Hero.Killed, 3, true); ResetLog(); q.WatchForLeaving();
        if (!q.MissionFailed || g.Ev.Frames != 3 || std::strcmp(g_log, "<any>;<any>;<any>;<any>;")) bad |= 1; }
    {   // mission won on frame 1: not failed
        CGameScriptInterface g; CQ_WizardBattleScript q; q.GSI = &g; g.Hero.Killed = false; q.MissionFailed = q.MissionSucceeded = false;
        g.Ev.Schedule(0, &q.MissionSucceeded, 1, true); q.WatchForLeaving();
        if (q.MissionFailed || g.Ev.Frames != 1) bad |= 2; }
    {   // terminating: nothing changes
        CGameScriptInterface g; CQ_WizardBattleScript q; q.GSI = &g; g.Hero.Killed = true; q.MissionFailed = q.MissionSucceeded = false; q.TerminateAfter = 0;
        q.WatchForLeaving();
        if (q.MissionFailed || g.Ev.Frames != 0) bad |= 4; }
    if (bad) { std::printf("00e1c4d0_TEST FAIL\n"); return 1; }
    std::printf("00e1c4d0_TEST PASS\n");
    return 0;
}
