// Behaviour test for CAT_BanditContact::Init (0x00E6AAC0): two named callbacks registered, the timer started from the quest globals, then the thing's flags cleared and the thing made a target.
#include <cstdio>
#include <cstring>
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CParent {
    int adds;
    char names[2][64];
    void Add(const char* prefix, const char* name) { std::strcpy(names[adds], prefix); std::strcat(names[adds], name); ++adds; Log("Add"); }
};
struct CTimers { long index, time; void SetTimer(long i, long t) { index = i; time = t; Log("SetTimer"); } };
struct CGlobals { long acolyteTime; long GetAcolyteTimerTime() const { return acolyteTime; } };
struct CGSI {
    long flagsThing, targetThing;
    void SetThingFlags(long thing, long a, long b, long c) { flagsThing = thing + a + b + c; Log("Flags"); }
    void SetThingAsTarget(long thing, long b) { targetThing = thing + b; Log("Target"); }
};
struct CScript {
    CParent* ParentClass; CTimers* Timers; CGlobals* Globals; CGSI* GSI; long Thing; long TimerIndex;
    void Init()
    {
        ParentClass->Add("ParentClass.", "WatchForAcolyteHit");
        ParentClass->Add("ParentClass.", "WatchForAcolyteDeath");
        Timers->SetTimer(TimerIndex, Globals->GetAcolyteTimerTime());
        GSI->SetThingFlags(Thing, 0, 0, 0);
        GSI->SetThingAsTarget(Thing, 0);
    }
};
int main()
{
    CParent parent = { 0 }; CTimers timers = { 0, 0 }; CGlobals globals = { 77 }; CGSI gsi = { 0, 0 };
    CScript s; s.ParentClass = &parent; s.Timers = &timers; s.Globals = &globals; s.GSI = &gsi; s.Thing = 5; s.TimerIndex = 3;
    s.Init();
    bool ok = parent.adds == 2 && std::strcmp(parent.names[0], "ParentClass.WatchForAcolyteHit") == 0
        && std::strcmp(parent.names[1], "ParentClass.WatchForAcolyteDeath") == 0 && timers.index == 3 && timers.time == 77
        && gsi.flagsThing == 5 && gsi.targetThing == 5 && std::strcmp(g_log, "Add;Add;SetTimer;Flags;Target;") == 0;
    if (!ok) { std::printf("00e6aac0_TEST FAIL\n"); return 1; }
    std::printf("00e6aac0_TEST PASS\n");
    return 0;
}