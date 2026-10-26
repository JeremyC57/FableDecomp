// Behaviour test for 0x00E6B750 (RST): model plus the body verbatim from final.cpp, callees stubbed.
// Checks that only a DarkwoodChapel region load teleports the acolytes (with the local thing), other loads and
// idle frames do not, the frame-per-poll wait, termination in the hit block, and that the local thing is released on every exit.
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
    const char* PStringData;
    CCharString() : PStringData(0) { }
    ~CCharString() { }
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return strcmp(PStringData, text) == 0;
    }
};
class CScriptThing { public:
    int Id;
    CScriptThing() : Id(0) { ++g_live; }
    CScriptThing(const CScriptThing& o) : Id(o.Id) { ++g_live; }
    __forceinline ~CScriptThing() { --g_live; }
};
static const char* g_loads[16];          // region loaded at frame f (0 = none)
static int g_teleports = 0;
class CGameScriptInterface { public:
    void NewScriptFrame() const { ++g_frames; }
    bool MsgOnRegionLoaded(CCharString& region) const
    {
        if (g_frames >= 16 || !g_loads[g_frames]) return false;
        region.PStringData = g_loads[g_frames];
        return true;
    }
};
struct CV_ChapelOfEvilScript {
    CGameScriptInterface* Game;
    bool IsActiveThreadTerminating() { return Term(); }
    void TeleportAcolytes(const CScriptThing& thing) { if (thing.Id != 0) g_badName = true; ++g_teleports; }
    void WatchForRegionEntry();
};

void CV_ChapelOfEvilScript::WatchForRegionEntry()
{
    CCharString region;
    CScriptThing thing;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->MsgOnRegionLoaded(region)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (region == "DarkwoodChapel") {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            TeleportAcolytes(thing);
        }
        Game->NewScriptFrame();
    }
}

static void Run(int termAt, int termFrame)
{
    Reset(); g_termAt = termAt; g_termFrame = termFrame; g_teleports = 0;
    static CGameScriptInterface game; CV_ChapelOfEvilScript s; s.Game = &game;
    s.WatchForRegionEntry();
}
int main()
{
    memset(g_loads, 0, sizeof(g_loads));
    Run(1, 1 << 30); CHECK(g_teleports == 0 && g_frames == 0 && g_live == 0);
    g_loads[2] = "DarkwoodChapel"; g_loads[5] = "Bowerstone"; g_loads[7] = "DarkwoodChapel";
    Run(1 << 30, 9); CHECK(g_teleports == 2 && g_frames == 9 && g_live == 0 && !g_badName);
    Run(1 << 30, 6); CHECK(g_teleports == 1 && g_frames == 6 && g_live == 0);
    // frame 0: top(1); polls frames 0,1 fail with a check after each frame (2,3); frame 2 loads: post-wait (4), hit block (5)
    Run(5, 1 << 30); CHECK(g_teleports == 0 && g_frames == 2 && g_live == 0 && g_iatt == 5);
    char line[64];
    _snprintf(line, sizeof(line), "00e6b750_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}