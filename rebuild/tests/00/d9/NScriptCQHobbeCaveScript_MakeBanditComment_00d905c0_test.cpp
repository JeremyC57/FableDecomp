// Behaviour test for CQ_HobbeCaveScript::MakeBanditComment (0x00D905C0).
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
static bool g_true = true;
struct CGameScriptInterface {
    long Timers[4];
    CScriptThing Hero;
    long GetTimer(long i) { return Timers[i]; }
    void SetTimer(long i, long t) { Timers[i] = t; }
    CScriptThing& GetHero() { return Hero; }
    long AddNewConversation(const CScriptThing& who, bool a, bool b) { char t[64]; std::sprintf(t, "New(%s,%d,%d)", who.Name, a, b); Log(t); return 7; }
    void AddPersonToConversation(long c, const CScriptThing& who) { char t[64]; std::sprintf(t, "Person(%ld,%s)", c, who.Name); Log(t); }
    void AddLineToConversation(long c, const CCharString& line, bool a, const CScriptThing& from, const CScriptThing& to)
    { char t[96]; std::sprintf(t, "Line(%ld,%s,%d,%s,%s)", c, line.Text, a, from.Name, to.Name); Log(t); }
};
static CGameScriptInterface g_gsi;
static CGameScriptInterface* g_pGameScriptInterface = &g_gsi;
struct CScriptDef { long HC_BanditCommentTime; };
static CScriptDef g_def;
static CScriptDef* g_pScriptDef = &g_def;
struct CTimer {
    long TimerIndex;
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
};
struct CQuestScriptBase : CScriptBase { CGameScriptInterface* GSI; };
struct CQ_HobbeCaveScript : CQuestScriptBase {
    CTimer BanditComment;
    bool MakeBanditComment(const CCharString& line, const CScriptThing& bandit);
};
bool CQ_HobbeCaveScript::MakeBanditComment(const CCharString& line, const CScriptThing& bandit)
{
    if (BanditComment.GetTime() > 0) {
        if (IsActiveThreadTerminating())
            return false;
        return false;
    }
    BanditComment.SetTime(g_pScriptDef->HC_BanditCommentTime);
    long conversation = GSI->AddNewConversation(bandit, false, false);
    GSI->AddPersonToConversation(conversation, GSI->GetHero());
    GSI->AddLineToConversation(conversation, line, false, bandit, GSI->GetHero());
    return true;
}
int main()
{
    int bad = 0;
    CQ_HobbeCaveScript q; q.GSI = &g_gsi; q.BanditComment.TimerIndex = 2; g_gsi.Hero = CScriptThing("HERO", &g_true);
    g_def.HC_BanditCommentTime = 90; g_gsi.Timers[2] = 0;
    CScriptThing bandit("BANDIT", &g_true);
    ResetLog();
    if (!q.MakeBanditComment(CCharString("HC_LINE"), bandit)) bad |= 1;
    if (std::strcmp(g_log, "New(BANDIT,0,0);Person(7,HERO);Line(7,HC_LINE,0,BANDIT,HERO);") || g_gsi.Timers[2] != 90) bad |= 2;
    ResetLog();
    if (q.MakeBanditComment(CCharString("HC_LINE"), bandit) || g_log[0] || q.TerminateCalls != 1 || g_gsi.Timers[2] != 90) bad |= 4;
    if (bad) { std::printf("00d905c0_TEST FAIL\n"); return 1; }
    std::printf("00d905c0_TEST PASS\n");
    return 0;
}