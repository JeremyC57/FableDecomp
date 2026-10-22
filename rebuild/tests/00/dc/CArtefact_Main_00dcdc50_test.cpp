// Behaviour test for 0x00DCDC50 (RST): model plus the body verbatim from final.cpp, callees stubbed.
// Checks the alive filter over Me, the BanditTeamCrateDrop lookup, that a near crate not carried by the hero removes
// the artefact (count decremented, master flag set, RemoveThing(Me, false, true)), near-and-carried / far frames only track
// wasNear, termination at the near and far checks, and that the crate and filter copies are released on every exit.
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

struct CCharString {
    const char* Text;
    CCharString(const char* text, long length = -1) : Text(text) { (void)length; }
    ~CCharString() { }
};
static int g_nearFrom = 1 << 30, g_nearTo = 1 << 30, g_freeFrom = 1 << 30;
struct CScriptThing {
    int Id;
    explicit CScriptThing(int id) : Id(id) { ++g_live; }
    CScriptThing(const CScriptThing& o) : Id(o.Id) { ++g_live; }
    __forceinline ~CScriptThing() { --g_live; }
    bool IsBeingCarriedBy(const CCharString& name) const
    {
        if (strcmp(name.Text, "SCRIPT_NAME_HERO") != 0 || Id != 1) g_badName = true;
        return g_frames < g_freeFrom;
    }
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance)
{
    if (a.Id != 1 || b.Id != 9 || distance != 3.0f) g_badName = true;
    return g_frames >= g_nearFrom && g_frames < g_nearTo;
}
static int g_removed = 0;
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; }
    CScriptThing GetThingWithScriptName(const CCharString& name) const
    {
        if (strcmp(name.Text, "BanditTeamCrateDrop") != 0) g_badName = true;
        return CScriptThing(9);
    }
    void RemoveThing(const CScriptThing& thing, bool b1, bool b2) const { if (thing.Id != 1 || b1 || !b2) g_badName = true; ++g_removed; }
};
static int g_predicates = 0;
struct CIsThingAlive {
    CScriptThing m_thing;
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
struct SRaidMasterData { bool ArtefactTaken; };
struct CQ_OrchardFarmRaidScript {
    SRaidMasterData* MasterData;
    long             ArtefactsRemaining;
};
struct CArtefact {
    CGameScriptInterface*     Game;
    CScriptThing              Me;
    CQ_OrchardFarmRaidScript* ParentClass;
    CArtefact() : Me(1) { }
    void SetPredicate(CIsThingAlive& predicate) { if (predicate.m_thing.Id == 1) ++g_predicates; }
    bool IsActiveThreadTerminating() const { return Term(); }
    void Main();
};

void CArtefact::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    bool wasNear = false;
    CScriptThing crate = Game->GetThingWithScriptName("BanditTeamCrateDrop");
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsUnder(Me, crate, 3.0f)) {
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth(0)
            bool carried = Me.IsBeingCarriedBy("SCRIPT_NAME_HERO");
            if (!carried) {
                if (IsActiveThreadTerminating())
                    return;
                CQ_OrchardFarmRaidScript* parent = ParentClass;
                --parent->ArtefactsRemaining;
                parent->MasterData->ArtefactTaken = true;
                Game->RemoveThing(Me, false, true);
                return;
            }
            if (IsActiveThreadTerminating())
                return;
            if (!wasNear) {
                if (IsActiveThreadTerminating())
                    return;
                wasNear = true;
            }
#pragma inline_depth()
        } else {
            if (IsActiveThreadTerminating())
                return;
            wasNear = false;
        }
        Game->NewScriptFrame();
    }
}

static SRaidMasterData g_master; static CQ_OrchardFarmRaidScript g_raid;
static void Run(int termAt, int termFrame, int nearFrom, int nearTo, int freeFrom)
{
    Reset(); g_termAt = termAt; g_termFrame = termFrame; g_nearFrom = nearFrom; g_nearTo = nearTo; g_freeFrom = freeFrom;
    g_removed = 0; g_predicates = 0; g_master.ArtefactTaken = false; g_raid.MasterData = &g_master; g_raid.ArtefactsRemaining = 3;
    static CGameScriptInterface game;
    {
        CArtefact a; a.Game = &game; a.ParentClass = &g_raid;
        g_live = 0;   // count only the copies made by Main
        a.Main();
        CHECK(g_live == 0);
    }
}
int main()
{
    Run(1, 1 << 30, 1 << 30, 1 << 30, 1 << 30); CHECK(g_predicates == 1 && g_frames == 0 && g_removed == 0);
    // near from frame 2 (carried until frame 4), then left free: removed at frame 4
    Run(1 << 30, 1 << 30, 2, 1 << 30, 4); CHECK(g_removed == 1 && g_raid.ArtefactsRemaining == 2 && g_master.ArtefactTaken && g_frames == 4 && !g_badName);
    // near only while carried: no removal, stops at the frame limit
    Run(1 << 30, 6, 1, 3, 1 << 30); CHECK(g_removed == 0 && g_raid.ArtefactsRemaining == 3 && !g_master.ArtefactTaken && g_frames == 6);
    // frame 0: top(1), near(2) terminates
    Run(2, 1 << 30, 0, 1 << 30, 1 << 30); CHECK(g_removed == 0 && g_frames == 0);
    // frame 0: top(1), far(2) terminates
    Run(2, 1 << 30, 1 << 30, 1 << 30, 1 << 30); CHECK(g_removed == 0 && g_frames == 0);
    // near and free at frame 0: top(1), near(2), not-carried block(3) terminates before the removal
    Run(3, 1 << 30, 0, 1 << 30, 0); CHECK(g_removed == 0 && g_raid.ArtefactsRemaining == 3 && g_frames == 0);
    char line[64];
    _snprintf(line, sizeof(line), "00dcdc50_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}