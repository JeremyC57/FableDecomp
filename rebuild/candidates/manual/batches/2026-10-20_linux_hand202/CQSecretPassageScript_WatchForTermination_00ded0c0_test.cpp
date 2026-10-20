// Behaviour test for CQ_SecretPassageScript::WatchForTermination (0x00DED0C0).
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
struct CGameScriptInterface {
    CFrameEvents Ev;
    void NewScriptFrame() { Ev.Tick(); }
    CCharString GetActiveQuestName() { return CCharString("Q_SecretPassage"); }
    void SetQuestAsFailed(const CCharString& name, bool a, const CWideString& msg, bool b)
    { char t[96]; std::sprintf(t, "Failed(%s,%d,%d,%d)", name.Text, a ? 1 : 0, *(const unsigned short*)msg.Text, b ? 1 : 0); Log(t); }
    void DeactivateQuestLater(const CCharString& name, unsigned long delay) { char t[96]; std::sprintf(t, "Deactivate(%s,%lu)", name.Text, delay); Log(t); }
};
struct CQuestScriptBase : CScriptBase { CGameScriptInterface* GSI; };
struct CQ_SecretPassageScript : CQuestScriptBase {
    bool MissionSucceeded; bool MissionFailed; bool MissionOver;
    void WatchForTermination();
};
void CQ_SecretPassageScript::WatchForTermination()
{
    while (!MissionFailed && !MissionSucceeded) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (MissionFailed) {
        if (IsActiveThreadTerminating())
            return;
        GSI->SetQuestAsFailed(GSI->GetActiveQuestName(), true, L"", true);
    }
    GSI->DeactivateQuestLater(GSI->GetActiveQuestName(), 0);
}
int main()
{
    int bad = 0;
    {   // fails on frame 2: marked failed then deactivated
        CGameScriptInterface gsi; CQ_SecretPassageScript q; q.GSI = &gsi; q.MissionSucceeded = false; q.MissionFailed = false;
        gsi.Ev.Schedule(0, &q.MissionFailed, 2, true);
        ResetLog(); q.WatchForTermination();
        if (std::strcmp(g_log, "Failed(Q_SecretPassage,1,0,1);Deactivate(Q_SecretPassage,0);") != 0 || gsi.Ev.Frames != 2) bad |= 1;
    }
    {   // succeeds on frame 4: deactivated only
        CGameScriptInterface gsi; CQ_SecretPassageScript q; q.GSI = &gsi; q.MissionSucceeded = false; q.MissionFailed = false;
        gsi.Ev.Schedule(0, &q.MissionSucceeded, 4, true);
        ResetLog(); q.WatchForTermination();
        if (std::strcmp(g_log, "Deactivate(Q_SecretPassage,0);") != 0 || gsi.Ev.Frames != 4) bad |= 2;
    }
    {   // failed but the thread terminates at the second test: nothing happens
        CGameScriptInterface gsi; CQ_SecretPassageScript q; q.GSI = &gsi; q.MissionSucceeded = false; q.MissionFailed = true; q.TerminateAfter = 1;
        ResetLog(); q.WatchForTermination();
        if (g_log[0] != 0 || q.TerminateCalls != 2) bad |= 4;
    }
    if (bad) { std::printf("00ded0c0_TEST FAIL\n"); return 1; }
    std::printf("00ded0c0_TEST PASS\n");
    return 0;
}
