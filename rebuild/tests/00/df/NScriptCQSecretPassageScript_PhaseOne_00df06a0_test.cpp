// Behaviour test for CQ_SecretPassageScript::PhaseOne (0x00DF06A0).
#include <cstdio>
#include <cstring>
// Test doubles: strings keep their literal, a script thing is a named handle whose liveness is shared with the
// test, the game script interface records calls in g_log, and the thread terminates after a set number of tests.
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static void ResetLog() { g_log[0] = 0; }
class CCharString { public:
    const char* Text;
    CCharString(const char* text, long = -1) : Text(text) {}
};
class CWideString { public:
    const void* Text;
    CWideString(const void* text) : Text(text) {}
};
class CScriptThing { public:
    const char* Name;
    const bool* AliveFlag;
    CScriptThing() : Name(""), AliveFlag(0) {}
    CScriptThing(const char* n, const bool* a) : Name(n), AliveFlag(a) {}
    bool IsAlive() const { return AliveFlag && *AliveFlag; }
};
class CScriptBase { public:
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
class CScriptGameResourceObjectScriptedThingBase { public:
    CScriptGameResourceObjectScriptedThingBase() { Log("Handle+"); }
    ~CScriptGameResourceObjectScriptedThingBase() { Log("Handle-"); }
};
class CGameScriptInterface { public: CFrameEvents Ev; void NewScriptFrame() { Ev.Tick(); Log("Frame"); } };
struct CQuestScriptBase : CScriptBase { CGameScriptInterface* GSI; };
struct CQ_SecretPassageScript : CQuestScriptBase {
    bool MissionSucceeded, MissionFailed, MissionOver, MotherFound;
    void MotherTortureThread() { Log("Torture"); }
    void PhaseOne();
};
void CQ_SecretPassageScript::PhaseOne()
{
    CScriptGameResourceObjectScriptedThingBase mother;
    MotherTortureThread();
    while (!MotherFound) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
}
int main()
{
    int bad = 0;
    {   CGameScriptInterface g; CQ_SecretPassageScript q; q.GSI = &g; q.MotherFound = false; g.Ev.Schedule(0, &q.MotherFound, 2, true);
        ResetLog(); q.PhaseOne();
        if (std::strcmp(g_log, "Handle+;Torture;Frame;Frame;Handle-;") || q.TerminateCalls != 3) bad |= 1; }
    {   CGameScriptInterface g; CQ_SecretPassageScript q; q.GSI = &g; q.MotherFound = false; q.TerminateAfter = 0;
        ResetLog(); q.PhaseOne();
        if (std::strcmp(g_log, "Handle+;Torture;Frame;Handle-;") || q.TerminateCalls != 1) bad |= 2; }
    if (bad) { std::printf("00df06a0_TEST FAIL\n"); return 1; }
    std::printf("00df06a0_TEST PASS\n");
    return 0;
}