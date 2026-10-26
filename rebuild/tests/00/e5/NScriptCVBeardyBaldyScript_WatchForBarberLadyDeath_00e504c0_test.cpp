// Behaviour test for 0x00E504C0 (RST2): model plus the body verbatim from final.cpp, callees stubbed.
// Checks the barber lookup (nearest to the hero, by script name), the wait until she is alive, the wait until she is
// killed, BarberLadyKilled only when neither re-test is terminating, termination at every test, and the release of the
// barber handle on every exit.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#define _snprintf snprintf
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static int g_live = 0, g_iatt = 0, g_termAt = 1 << 30, g_frames = 0, g_stopFrame = 1 << 30, g_aliveFrame = 0, g_killFrame = 1 << 30;
static bool g_badArg = false;
static bool Term() { ++g_iatt; return g_iatt >= g_termAt || g_frames >= g_stopFrame; }
struct CCharString {
    const char* Text;
    CCharString(const char* text, long length = -1) : Text(text) { (void)length; }
    ~CCharString() { }
};
struct CScriptThing {
    int Id;
    explicit CScriptThing(int id) : Id(id) { if (Id != 1) ++g_live; }
    CScriptThing(const CScriptThing& o) : Id(o.Id) { if (Id != 1) ++g_live; }
    __forceinline ~CScriptThing() { if (Id != 1) --g_live; }
    bool MsgIsKilledBy(const CCharString& killer) const { if (strcmp(killer.Text, "") != 0 || Id != 9) g_badArg = true; return g_frames >= g_killFrame; }
    bool IsAlive() const { if (Id != 9) g_badArg = true; return g_frames >= g_aliveFrame; }
};
static CScriptThing g_hero(1);
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; }
    const CScriptThing& GetHero() { return g_hero; }
    CScriptThing GetNearestWithScriptName(const CScriptThing& thing, const CCharString& name) const
    {
        if (thing.Id != 1 || strcmp(name.Text, "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER") != 0) g_badArg = true;
        return CScriptThing(9);
    }
};
struct CScriptBase {
    bool IsActiveThreadTerminating() { return Term(); }
};
struct CV_BeardyBaldyScript : CScriptBase {
    CGameScriptInterface* GSI;
    bool BeardyBaldyKilled;
    bool BarberLadyKilled;
    void WatchForBarberLadyDeath();
};

void CV_BeardyBaldyScript::WatchForBarberLadyDeath()
{
    CScriptThing barber = GSI->GetNearestWithScriptName(GSI->GetHero(), "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER");
    while (!barber.IsAlive()) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
#pragma inline_depth(0)
    if (IsActiveThreadTerminating())
        return;
#pragma inline_depth()
    while (!IsActiveThreadTerminating()) {
        if (barber.MsgIsKilledBy("")) {
            if (!IsActiveThreadTerminating()) {
                if (!IsActiveThreadTerminating())
                    BarberLadyKilled = true;
            }
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
        GSI->NewScriptFrame();
    }
}

static void Run(CV_BeardyBaldyScript& s, int termAt, int stopFrame, int aliveFrame, int killFrame)
{
    static CGameScriptInterface gsi;
    g_live = 0; g_iatt = 0; g_frames = 0; g_badArg = false;
    g_termAt = termAt; g_stopFrame = stopFrame; g_aliveFrame = aliveFrame; g_killFrame = killFrame;
    s.GSI = &gsi; s.BarberLadyKilled = false; s.BeardyBaldyKilled = false;
    s.WatchForBarberLadyDeath();
}
int main()
{
    CV_BeardyBaldyScript s;
    Run(s, 1 << 30, 1 << 30, 3, 6); CHECK(g_frames == 6 && s.BarberLadyKilled && !s.BeardyBaldyKilled && g_live == 0 && !g_badArg);
    Run(s, 1 << 30, 1 << 30, 0, 0); CHECK(g_frames == 0 && s.BarberLadyKilled && g_iatt == 4 && g_live == 0);
    Run(s, 1 << 30, 2, 5, 1 << 30); CHECK(g_frames == 2 && !s.BarberLadyKilled && g_iatt == 2 && g_live == 0);   // dies in the alive wait
    Run(s, 1, 1 << 30, 0, 0);       CHECK(g_frames == 0 && !s.BarberLadyKilled && g_iatt == 1 && g_live == 0);   // the out-of-line exit
    Run(s, 2, 1 << 30, 0, 0);       CHECK(!s.BarberLadyKilled && g_iatt == 2 && g_live == 0);                    // second loop test
    Run(s, 3, 1 << 30, 0, 0);       CHECK(!s.BarberLadyKilled && g_iatt == 3 && g_live == 0);                    // first re-test
    Run(s, 4, 1 << 30, 0, 0);       CHECK(!s.BarberLadyKilled && g_iatt == 4 && g_live == 0);                    // second re-test
    Run(s, 1 << 30, 5, 0, 1 << 30); CHECK(g_frames == 5 && !s.BarberLadyKilled && g_live == 0);                  // never killed
    char line[64];
    _snprintf(line, sizeof(line), "00e504c0_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}