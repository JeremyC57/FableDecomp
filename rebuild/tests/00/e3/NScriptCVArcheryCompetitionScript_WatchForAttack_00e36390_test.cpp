// Behaviour test for 0x00E36390 (RST): model plus the body verbatim from final.cpp, callees stubbed.
// Checks that hits only count while the contest is idle (0) or over (2) and the owner survives them (both flags set),
// a running contest ignores them, a dead owner stops the watch, termination in the hit block, and the release on every exit.
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
    CCharString(const char* text, long length = -1) : Text(text) { (void)length; }
    ~CCharString() { }
};
static int g_dieAt = 1 << 30;           // the watched thing is alive while g_frames < g_dieAt
static int g_hitAt = -1, g_hitKind = 0; // hit at this frame: 1 melee (MsgIsHitBy), 2 aggressive special ability
static float g_health = 100.0f;
static bool HitCheck(const CCharString& a, int kind)
{
    if (strcmp(a.Text, "SCRIPT_NAME_HERO") != 0) g_badName = true;
    return g_frames == g_hitAt && g_hitKind == kind;
}
class CScriptThing { public:
    int Id;
    explicit CScriptThing(int id) : Id(id) { ++g_live; }
    CScriptThing(const CScriptThing& o) : Id(o.Id) { ++g_live; }
    __forceinline ~CScriptThing() { --g_live; }
    bool IsAlive() const { return g_frames < g_dieAt; }
    bool MsgIsHitBy(const CCharString& a) const { return HitCheck(a, 1); }
    bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& a) const { return HitCheck(a, 2); }
};
class CGameScriptInterface { public:
    void NewScriptFrame() const { ++g_frames; }
    float GetHealth(const CScriptThing& t) const { if (t.Id != 7) g_badName = true; return g_health; }
};
struct CQuestScriptBase {
    CGameScriptInterface* Game;
    bool IsActiveThreadTerminating() { return Term(); }
};

struct CV_ArcheryCompetitionScript : CQuestScriptBase {
    bool AttackedByHero;
    bool GiveAttackWarning;
    long ContestState;
    void WatchForAttack(CScriptThing owner);
};

void CV_ArcheryCompetitionScript::WatchForAttack(CScriptThing owner)
{
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (!owner.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            if (IsActiveThreadTerminating())
                return;
            break;
        }
        if (ContestState == 2 || ContestState == 0) {
            if (IsActiveThreadTerminating())
                return;
            bool attacked = (owner.MsgIsHitBy("SCRIPT_NAME_HERO") || owner.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO"))
                && Game->GetHealth(owner) > 0.0001f;
            if (attacked) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                AttackedByHero = true;
                GiveAttackWarning = true;
            }
        }
        Game->NewScriptFrame();
    }
}

static void Run(int termAt, int dieAt, int hitAt, int hitKind, long state, CV_ArcheryCompetitionScript& s)
{
    Reset(); g_termAt = termAt; g_dieAt = dieAt; g_hitAt = hitAt; g_hitKind = hitKind; g_health = 50.0f;
    static CGameScriptInterface game; s.Game = &game; s.AttackedByHero = false; s.GiveAttackWarning = false; s.ContestState = state;
    s.WatchForAttack(CScriptThing(7));
}
int main()
{
    CV_ArcheryCompetitionScript s;
    Run(1, 1000, -1, 0, 0, s); CHECK(!s.AttackedByHero && g_frames == 0 && g_live == 0);
    Run(1 << 30, 4, 2, 1, 0, s); CHECK(s.AttackedByHero && s.GiveAttackWarning && g_frames == 4 && g_live == 0 && !g_badName);
    Run(1 << 30, 4, 2, 2, 2, s); CHECK(s.AttackedByHero && s.GiveAttackWarning && g_frames == 4 && g_live == 0);
    Run(1 << 30, 4, 2, 1, 1, s); CHECK(!s.AttackedByHero && !s.GiveAttackWarning && g_frames == 4 && g_live == 0);
    Run(1 << 30, 0, -1, 0, 0, s); CHECK(g_frames == 0 && g_live == 0 && g_iatt == 3);
    // frame 0: top(1), contest check(2), frame 1: top(3), contest(4), hit block(5)
    Run(5, 1000, 1, 1, 0, s); CHECK(!s.AttackedByHero && g_frames == 1 && g_live == 0 && g_iatt == 5);
    char line[64];
    _snprintf(line, sizeof(line), "00e36390_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}