// Behaviour test for CQ_HangingTreeGoodScript::ReassignLeader (0x00D74F80).
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
static bool g_true = true, g_false = false;
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T Store[8];
    vector() { _M_start = _M_finish = Store; _M_end_of_storage = Store + 8; }
    bool empty() const { return _M_start == _M_finish; }
    T& front() { return *_M_start; }
    void pop_back() { --_M_finish; _M_finish->~T(); }
    void push(const T& v) { *_M_finish++ = v; }
};
struct CGameScriptInterface {
    bool FirstGuardAlive;
    CScriptThing Hero;
    CScriptThing& GetHero() { Log("GetHero"); return Hero; }
    long GetAllThingsWithScriptName(const CCharString& name, vector<CScriptThing>& out)
    {
        Log(name.Text);
        out.push(CScriptThing("GUARD1", FirstGuardAlive ? &g_true : &g_false));
        out.push(CScriptThing("GUARD2", &g_true));
        return 2;
    }
};
struct CQuestScriptBase : CScriptBase { CGameScriptInterface* GSI; };
struct CQ_HangingTreeGoodScript : CQuestScriptBase {
    bool LeaderSpokenTo, PartySuccessfullySetUp, MissionSucceeded, MissionFailed, SwitchingRegions, ReassignLeaderNow;
    CScriptThing Leader;
    vector<CScriptThing> CurrentGuardVector;
    void ReassignLeader();
};
void CQ_HangingTreeGoodScript::ReassignLeader()
{
    if (MissionSucceeded || MissionFailed || IsActiveThreadTerminating())
        return;
    vector<CScriptThing>& guards = CurrentGuardVector;
    while (!guards.empty())
        guards.pop_back();
    {
        CCharString name("HTG_Guard");
        GSI->GetAllThingsWithScriptName(name, guards);
    }
    if (guards.front().IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        Leader = guards.front();
        ReassignLeaderNow = false;
        PartySuccessfullySetUp = true;
    } else {
        if (IsActiveThreadTerminating())
            return;
        Leader = GSI->GetHero();
        PartySuccessfullySetUp = true;
    }
}
static void Setup(CQ_HangingTreeGoodScript& q, CGameScriptInterface& g, bool alive)
{
    q.GSI = &g; g.FirstGuardAlive = alive; g.Hero = CScriptThing("HERO", &g_true);
    q.MissionSucceeded = q.MissionFailed = false; q.PartySuccessfullySetUp = false; q.ReassignLeaderNow = true;
    q.Leader = CScriptThing("OLD", &g_true);
    q.CurrentGuardVector.push(CScriptThing("STALE", &g_true)); q.CurrentGuardVector.push(CScriptThing("STALE", &g_true));
    ResetLog();
}
int main()
{
    int bad = 0;
    {   CGameScriptInterface g; CQ_HangingTreeGoodScript q; Setup(q, g, true); q.ReassignLeader();
        if (std::strcmp(q.Leader.Name, "GUARD1") || q.ReassignLeaderNow || !q.PartySuccessfullySetUp
            || q.CurrentGuardVector._M_finish - q.CurrentGuardVector._M_start != 2 || std::strcmp(g_log, "HTG_Guard;")) bad |= 1; }
    {   CGameScriptInterface g; CQ_HangingTreeGoodScript q; Setup(q, g, false); q.ReassignLeader();
        if (std::strcmp(q.Leader.Name, "HERO") || !q.ReassignLeaderNow || !q.PartySuccessfullySetUp) bad |= 2; }
    {   CGameScriptInterface g; CQ_HangingTreeGoodScript q; Setup(q, g, true); q.MissionFailed = true; q.ReassignLeader();
        if (std::strcmp(q.Leader.Name, "OLD") || q.CurrentGuardVector._M_finish - q.CurrentGuardVector._M_start != 2 || g_log[0]) bad |= 4; }
    {   CGameScriptInterface g; CQ_HangingTreeGoodScript q; Setup(q, g, true); q.TerminateAfter = 1; q.ReassignLeader();
        if (std::strcmp(q.Leader.Name, "OLD") || q.PartySuccessfullySetUp) bad |= 8; }
    if (bad) { std::printf("00d74f80_TEST FAIL\n"); return 1; }
    std::printf("00d74f80_TEST PASS\n");
    return 0;
}