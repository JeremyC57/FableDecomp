// Behaviour test for 0x00EE6A40 (RST): model plus the body verbatim from final.cpp, callees stubbed.
// Checks the TourGuideFollower collection, that leaving the guild complex stops the watch without the flag, that all
// followers dead (tour unfinished) sets AllFollowersDead, a finished tour ignores it, and that every exit destroys and frees the vector.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define _snprintf snprintf
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static int g_live = 0;                  // live CScriptThing copies (every exit must release the watched thing)
static int g_iatt = 0;                  // IsActiveThreadTerminating call count
static int g_termAt = 1 << 30;          // ... it returns true from this call number on
static int g_termFrame = 1 << 30;       // ... or once this many script frames have run
static int g_frames = 0;                // NewScriptFrame count
static bool g_badName = false;          // a callee saw an unexpected name / argument
static bool Term() { ++g_iatt; return g_iatt >= g_termAt || g_frames >= g_termFrame; }
static void Reset() { g_live = 0; g_iatt = 0; g_termAt = 1 << 30; g_termFrame = 1 << 30; g_frames = 0; g_badName = false; }

class CCharString { public:
    const char* Text;
    CCharString() : Text(0) { }
    CCharString(const char* text, long length = -1) : Text(text) { (void)length; }
    ~CCharString() { }
};
class CScriptThing { public:
    int Id;
    explicit CScriptThing(int id) : Id(id) { ++g_live; }
    CScriptThing(const CScriptThing& o) : Id(o.Id) { ++g_live; }
    ~CScriptThing() { --g_live; }
};
static int g_frees = 0;
static void TestFree(void* p) { ++g_frees; free(p); }
struct CScriptThingVector {
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    CScriptThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~CScriptThingVector()
    {
        CScriptThing* last = _M_finish;
        for (CScriptThing* p = _M_start; p != last; ++p)
            p->~CScriptThing();
        if (_M_start)
            TestFree(_M_start);
    }
};
static int g_deadFrom = 1 << 30, g_leaveAt = 1 << 30;
bool __fastcall AreAllThingsInVectorDead(const CScriptThingVector& things)
{
    if (things._M_finish - things._M_start != 3) g_badName = true;
    return g_frames >= g_deadFrom;
}
class CGameScriptInterface { public:
    void NewScriptFrame() const { ++g_frames; }
    bool MsgIsRegionUnloaded(const CCharString& region) const
    {
        if (strcmp(region.Text, "HeroGuildComplexInside") != 0) g_badName = true;
        return g_frames == g_leaveAt;
    }
    long GetAllThingsWithScriptName(const CCharString& name, CScriptThingVector& things) const
    {
        if (strcmp(name.Text, "TourGuideFollower") != 0) g_badName = true;
        CScriptThing* mem = (CScriptThing*)malloc(3 * sizeof(CScriptThing));
        for (int i = 0; i < 3; ++i) new (mem + i) CScriptThing(i + 1);
        things._M_start = mem; things._M_finish = mem + 3; things._M_end_of_storage = mem + 3;
        return 3;
    }
};
struct CV_TourGuideScript {
    CGameScriptInterface* Game;
    bool AllFollowersDead;
    bool TourFinished;
    bool IsActiveThreadTerminating() { return Term(); }
    void WatchForNoFollowers();
};

void CV_TourGuideScript::WatchForNoFollowers()
{
    CScriptThingVector followers;
    CCharString unused;
    Game->GetAllThingsWithScriptName("TourGuideFollower", followers);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (Game->MsgIsRegionUnloaded("HeroGuildComplexInside")) {
            if (IsActiveThreadTerminating())
                return;
            break;
        }
        if (AreAllThingsInVectorDead(followers) && !TourFinished) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            AllFollowersDead = true;
            break;
        }
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        return;
}

static void Run(int termAt, int leaveAt, int deadFrom, bool finished, CV_TourGuideScript& s)
{
    Reset(); g_termAt = termAt; g_leaveAt = leaveAt; g_deadFrom = deadFrom; g_frees = 0;
    static CGameScriptInterface game; s.Game = &game; s.AllFollowersDead = false; s.TourFinished = finished;
    s.WatchForNoFollowers();
}
int main()
{
    CV_TourGuideScript s;
    Run(1, 1 << 30, 1 << 30, false, s); CHECK(!s.AllFollowersDead && g_frames == 0 && g_live == 0 && g_frees == 1);
    Run(1 << 30, 3, 1 << 30, false, s); CHECK(!s.AllFollowersDead && g_frames == 3 && g_live == 0 && g_frees == 1 && !g_badName);
    Run(1 << 30, 5, 2, false, s); CHECK(s.AllFollowersDead && g_frames == 2 && g_live == 0 && g_frees == 1 && g_iatt == 5);   // tops 1-3, block 4, trailing 5
    Run(1 << 30, 5, 2, true, s); CHECK(!s.AllFollowersDead && g_frames == 5 && g_live == 0 && g_frees == 1);
    // frame 0: top(1); frame 1: top(2), all-dead block(3) terminates (the out-of-line vector dtor exit)
    Run(3, 1 << 30, 1, false, s); CHECK(!s.AllFollowersDead && g_frames == 1 && g_live == 0 && g_frees == 1 && g_iatt == 3);
    Run(3, 1, 1 << 30, false, s); CHECK(!s.AllFollowersDead && g_frames == 1 && g_live == 0 && g_frees == 1 && g_iatt == 3);
    char line[64];
    _snprintf(line, sizeof(line), "00ee6a40_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}