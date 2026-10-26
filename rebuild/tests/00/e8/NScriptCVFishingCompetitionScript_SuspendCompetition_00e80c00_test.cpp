// Behaviour test for CV_FishingCompetitionScript::SuspendCompetition (0x00E80C00).
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
class CGameScriptInterface { public:
    CFrameEvents Ev;
    bool FishermanLoaded, GreatwoodLoaded;
    void NewScriptFrame() { Ev.Tick(); }
    bool IsRegionLoaded(const CCharString& r)
    { Log(r.Text); return std::strcmp(r.Text, "Fisherman") == 0 ? FishermanLoaded : std::strcmp(r.Text, "GreatwoodEntrance") == 0 ? GreatwoodLoaded : false; }
};
struct CQuestScriptBase : CScriptBase { CGameScriptInterface* GSI; };
struct CV_FishingCompetitionScript : CQuestScriptBase {
    bool BountyHuntActive, OrganiserRanAway, OrganiserKilled, CompetitionRunning;
    long AttackCounter, AttackDayCount, HitCount;
    void SuspendCompetition();
};
void CV_FishingCompetitionScript::SuspendCompetition()
{
    OrganiserRanAway = true;
    CompetitionRunning = false;
    AttackCounter += 5;
    HitCount = 0;
    while (GSI->IsRegionLoaded("Fisherman") || GSI->IsRegionLoaded("GreatwoodEntrance")) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (!IsActiveThreadTerminating())
        CompetitionRunning = true;
}
int main()
{
    int bad = 0;
    {   // Fisherman loaded until frame 1, Greatwood until frame 2: two frames, then running again
        CGameScriptInterface g; CV_FishingCompetitionScript q; q.GSI = &g; g.FishermanLoaded = true; g.GreatwoodLoaded = true;
        g.Ev.Schedule(0, &g.FishermanLoaded, 1, false); g.Ev.Schedule(1, &g.GreatwoodLoaded, 2, false);
        q.OrganiserRanAway = false; q.CompetitionRunning = true; q.AttackCounter = 3; q.HitCount = 9; ResetLog();
        q.SuspendCompetition();
        if (!q.OrganiserRanAway || !q.CompetitionRunning || q.AttackCounter != 8 || q.HitCount != 0 || g.Ev.Frames != 2
            || std::strcmp(g_log, "Fisherman;Fisherman;GreatwoodEntrance;Fisherman;GreatwoodEntrance;")) bad |= 1;
    }
    {   // terminating while a region is loaded: the competition stays stopped
        CGameScriptInterface g; CV_FishingCompetitionScript q; q.GSI = &g; g.FishermanLoaded = false; g.GreatwoodLoaded = true;
        q.CompetitionRunning = true; q.AttackCounter = 0; q.TerminateAfter = 0;
        q.SuspendCompetition();
        if (q.CompetitionRunning || q.AttackCounter != 5 || g.Ev.Frames != 1) bad |= 2;
    }
    if (bad) { std::printf("00e80c00_TEST FAIL\n"); return 1; }
    std::printf("00e80c00_TEST PASS\n");
    return 0;
}