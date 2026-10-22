// Behaviour test for 0x00D97170 (RST): model plus the body verbatim from final.cpp, callees stubbed.
// Checks that the first hit that lowers James's health clears the quest flag once (a hit without damage does not),
// a kill fails the mission, termination in the hit block, and that the by-value James is released on every exit.
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
static int g_killedAt = 1 << 30;
static int g_hitFrames[8]; static int g_hitCount = 0;
static float g_healthAt[16];
struct CScriptThing {
    int Id;
    explicit CScriptThing(int id) : Id(id) { ++g_live; }
    CScriptThing(const CScriptThing& o) : Id(o.Id) { ++g_live; }
    __forceinline ~CScriptThing() { --g_live; }
    bool MsgIsKilledBy(const CCharString& k) const { if (strcmp(k.Text, "") != 0) g_badName = true; return g_frames >= g_killedAt; }
    bool MsgIsHitBy(const CCharString& a) const
    {
        if (strcmp(a.Text, "") != 0) g_badName = true;
        for (int i = 0; i < g_hitCount; ++i) if (g_hitFrames[i] == g_frames) return true;
        return false;
    }
};
struct CHobbeCaveData { bool Flag114; };
static CHobbeCaveData g_data; static int g_rearmAt = -1;
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; if (g_frames == g_rearmAt) g_data.Flag114 = true; }
    float GetHealth(const CScriptThing& t) const { if (t.Id != 7) g_badName = true; return g_healthAt[g_frames < 16 ? g_frames : 15]; }
};
struct CQ_HobbeCaveScript {
    CGameScriptInterface* Game;
    CHobbeCaveData*       Data;
    bool                  MissionFailed;
    bool IsActiveThreadTerminating() { return Term(); }
    void CheckJamesKilled(CScriptThing james);
};

void CQ_HobbeCaveScript::CheckJamesKilled(CScriptThing james)
{
    bool hit = false;
    long health = (long)Game->GetHealth(james);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (james.MsgIsKilledBy("")) {
            if (IsActiveThreadTerminating())
                return;
            MissionFailed = true;
            if (IsActiveThreadTerminating())
                return;
            return;
        }
        if (!hit && james.MsgIsHitBy("") && Game->GetHealth(james) < health) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            Data->Flag114 = false;
            hit = true;
        }
        Game->NewScriptFrame();
    }
}

static void Run(int termAt, int killedAt, CQ_HobbeCaveScript& s)
{
    Reset(); g_termAt = termAt; g_killedAt = killedAt;
    static CGameScriptInterface game; s.Game = &game; s.Data = &g_data; s.MissionFailed = false; g_data.Flag114 = true;
    s.CheckJamesKilled(CScriptThing(7));
}
int main()
{
    CQ_HobbeCaveScript s;
    for (int i = 0; i < 16; ++i) g_healthAt[i] = 100.0f;
    Run(1, 1 << 30, s); CHECK(!s.MissionFailed && g_data.Flag114 && g_frames == 0 && g_live == 0);
    // hit at frame 1 without damage (ignored), hit at frame 3 with damage (flag cleared), killed at frame 6
    g_hitCount = 2; g_hitFrames[0] = 1; g_hitFrames[1] = 3;
    for (int j = 3; j < 16; ++j) g_healthAt[j] = 80.0f;
    Run(1 << 30, 6, s); CHECK(s.MissionFailed && !g_data.Flag114 && g_frames == 6 && g_live == 0 && !g_badName);
    // the flag is re-armed at frame 4 and James is hit and damaged again at frame 5: only the first hit counts
    g_hitCount = 3; g_hitFrames[2] = 5; g_rearmAt = 4;
    Run(1 << 30, 7, s); CHECK(s.MissionFailed && g_data.Flag114 && g_frames == 7 && g_live == 0);
    g_hitCount = 2; g_rearmAt = -1;
    Run(1 << 30, 2, s); CHECK(s.MissionFailed && g_data.Flag114 && g_frames == 2 && g_live == 0);
    // frames 0..2 one check each (1..3), frame 3: top (4), hit block (5) terminates
    Run(5, 1 << 30, s); CHECK(!s.MissionFailed && g_data.Flag114 && g_frames == 3 && g_live == 0 && g_iatt == 5);
    // killed: second check after MissionFailed terminates
    Run(3, 0, s); CHECK(s.MissionFailed && g_frames == 0 && g_live == 0 && g_iatt == 3);
    char line[64];
    _snprintf(line, sizeof(line), "00d97170_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}