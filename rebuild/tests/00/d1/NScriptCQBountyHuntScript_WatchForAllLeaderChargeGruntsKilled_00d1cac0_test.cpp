// Behaviour test for 0x00D1CAC0 (RST2): model plus the body verbatim from final.cpp, callees stubbed.
// Checks the BH_GangGruntSpawn lookup, the wait until every grunt is dead, AllLeaderChargeGruntsKilled only when the
// final re-test is not terminating, termination in the wait, and that the vector's elements and buffer are released
// on every exit.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#define _snprintf snprintf
#define __fastcall
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static int g_live = 0, g_bufs = 0, g_iatt = 0, g_termAt = 1 << 30, g_frames = 0, g_stopFrame = 1 << 30, g_deadFrame = 0, g_count = 3;
static bool g_badArg = false;
static bool Term() { ++g_iatt; return g_iatt >= g_termAt || g_frames >= g_stopFrame; }
class CCharString { public:
    const char* Text;
    CCharString(const char* text, long length = -1) : Text(text) { (void)length; }
    ~CCharString() { }
};
class CScriptThing { public:
    int Id;
    explicit CScriptThing(int id) : Id(id) { ++g_live; }
    virtual ~CScriptThing() { --g_live; }
};
inline void Destroy(CScriptThing* first, CScriptThing* last)
{
    for (; first != last; ++first)
        first->~CScriptThing();
}
static void FreeBuffer(void* p) { --g_bufs; free(p); }
struct CScriptThingVector {
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    CScriptThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~CScriptThingVector()
    {
        Destroy(_M_start, _M_finish);
        if (_M_start)
            FreeBuffer(_M_start);
    }
};
static bool __fastcall AreAllThingsInVectorDead(const CScriptThingVector& things)
{
    if (things._M_finish - things._M_start != g_count) g_badArg = true;
    return g_frames >= g_deadFrame;
}
class CGameScriptInterface { public:
    void NewScriptFrame() const { ++g_frames; }
    long GetAllThingsWithScriptName(const CCharString& name, CScriptThingVector& things) const
    {
        if (strcmp(name.Text, "BH_GangGruntSpawn") != 0) g_badArg = true;
        if (g_count) {
            ++g_bufs;
            things._M_start = (CScriptThing*)malloc(sizeof(CScriptThing) * g_count);
            for (int k = 0; k < g_count; ++k)
                new (&things._M_start[k]) CScriptThing(k);
            things._M_finish = things._M_end_of_storage = things._M_start + g_count;
        }
        return g_count;
    }
};
class CScriptBase { public:
    bool IsActiveThreadTerminating() { return Term(); }
};
struct CQ_BountyHuntScript : CScriptBase {
    CGameScriptInterface* GSI;
    bool AllLeaderGruntsKilled;
    bool AllLeaderChargeGruntsKilled;
    void WatchForAllLeaderChargeGruntsKilled();
};

void CQ_BountyHuntScript::WatchForAllLeaderChargeGruntsKilled()
{
    CScriptThingVector grunts;
    GSI->GetAllThingsWithScriptName("BH_GangGruntSpawn", grunts);
    while (!AreAllThingsInVectorDead(grunts)) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
#pragma inline_depth(0)
    if (IsActiveThreadTerminating())
        return;
    AllLeaderChargeGruntsKilled = true;
}
#pragma inline_depth()

static void Run(CQ_BountyHuntScript& s, int termAt, int stopFrame, int deadFrame, int count)
{
    static CGameScriptInterface gsi;
    g_live = 0; g_bufs = 0; g_iatt = 0; g_frames = 0; g_badArg = false;
    g_termAt = termAt; g_stopFrame = stopFrame; g_deadFrame = deadFrame; g_count = count;
    s.GSI = &gsi; s.AllLeaderGruntsKilled = false; s.AllLeaderChargeGruntsKilled = false;
    s.WatchForAllLeaderChargeGruntsKilled();
}
int main()
{
    CQ_BountyHuntScript s;
    Run(s, 1 << 30, 1 << 30, 4, 3); CHECK(g_frames == 4 && s.AllLeaderChargeGruntsKilled && !s.AllLeaderGruntsKilled && g_live == 0 && g_bufs == 0 && !g_badArg);
    Run(s, 1 << 30, 1 << 30, 0, 0); CHECK(g_frames == 0 && s.AllLeaderChargeGruntsKilled && g_iatt == 1 && g_bufs == 0);
    Run(s, 1 << 30, 2, 9, 2);       CHECK(g_frames == 2 && !s.AllLeaderChargeGruntsKilled && g_live == 0 && g_bufs == 0);   // terminating in the wait
    Run(s, 1, 1 << 30, 0, 5);       CHECK(g_frames == 0 && !s.AllLeaderChargeGruntsKilled && g_iatt == 1 && g_live == 0 && g_bufs == 0);   // the final re-test
    char line[64];
    _snprintf(line, sizeof(line), "00d1cac0_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}