// Behaviour test for CQS_GuardianTrophyDealerInfoScript::WaitForPieceOver (0x00E27AC0).
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
static bool g_mazeAlive = true;
struct CGameScriptInterface {
    CFrameEvents Ev;
    void NewScriptFrame() { Ev.Tick(); }
    CScriptThing GetThingWithScriptName(const CCharString& name) { Log(name.Text); return CScriptThing(name.Text, &g_mazeAlive); }
    CCharString GetActiveQuestName() { return CCharString("Q_GuardianTrophyDealer"); }
    void DeactivateQuestLater(const CCharString& name, unsigned long delay) { char b[96]; std::sprintf(b, "Deactivate(%s,%lu)", name.Text, delay); Log(b); }
};
struct CQuestScriptBase : CScriptBase { CGameScriptInterface* GSI; };
struct CQS_GuardianTrophyDealerInfoScript : CQuestScriptBase {
    bool PieceOver;
    void WaitForPieceOver();
};
void CQS_GuardianTrophyDealerInfoScript::WaitForPieceOver()
{
    while (!PieceOver) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing maze = GSI->GetThingWithScriptName("GTDI_Maze");
    while (maze.IsAlive()) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    GSI->DeactivateQuestLater(GSI->GetActiveQuestName(), 0);
}
int main()
{
    int bad = 0;
    {   // the piece ends on frame 3, the maze dies on frame 5: deactivated once, after five frames
        CGameScriptInterface gsi; CQS_GuardianTrophyDealerInfoScript q; q.GSI = &gsi; q.PieceOver = false; g_mazeAlive = true;
        gsi.Ev.Schedule(0, &q.PieceOver, 3, true); gsi.Ev.Schedule(1, &g_mazeAlive, 5, false);
        ResetLog(); q.WaitForPieceOver();
        if (std::strcmp(g_log, "GTDI_Maze;Deactivate(Q_GuardianTrophyDealer,0);") != 0 || gsi.Ev.Frames != 5) bad |= 1;
    }
    {   // thread terminates while waiting for the piece: nothing is looked up or deactivated
        CGameScriptInterface gsi; CQS_GuardianTrophyDealerInfoScript q; q.GSI = &gsi; q.PieceOver = false; q.TerminateAfter = 2;
        ResetLog(); q.WaitForPieceOver();
        if (g_log[0] != 0 || gsi.Ev.Frames != 3) bad |= 2;
    }
    {   // terminates while the maze is alive: no deactivation
        CGameScriptInterface gsi; CQS_GuardianTrophyDealerInfoScript q; q.GSI = &gsi; q.PieceOver = true; q.TerminateAfter = 3; g_mazeAlive = true;
        ResetLog(); q.WaitForPieceOver();
        if (std::strcmp(g_log, "GTDI_Maze;") != 0) bad |= 4;
    }
    if (bad) { std::printf("00e27ac0_TEST FAIL\n"); return 1; }
    std::printf("00e27ac0_TEST PASS\n");
    return 0;
}
