// Behaviour test for 0x00ED9790 (RST): model plus the body verbatim from final.cpp, callees stubbed.
// Checks the alive predicate over the bard, the opening frame and early exit, that a hit sets AttackedByHero and the
// watch continues (dead frames only yield), termination in the alive and hit blocks, and the release of the bard on every exit.
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

static int g_predicates = 0;
struct CIsThingAlive {
    CScriptThing m_thing;
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
};
struct CV_TalentlessBardScript : CQuestScriptBase {
    bool AttackedByHero;
    void SetPredicate(const CIsThingAlive& p) { if (p.m_thing.Id == 7) ++g_predicates; }
    void WatchForAttack(CScriptThing bard);
};

void CV_TalentlessBardScript::WatchForAttack(CScriptThing bard)
{
    SetPredicate(CIsThingAlive(bard));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (bard.IsAlive()) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            bool hit = bard.MsgIsHitBy("SCRIPT_NAME_HERO") || bard.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO");
            if (hit) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                AttackedByHero = true;
            }
        }
        Game->NewScriptFrame();
    }
}

static void Run(int termAt, int termFrame, int dieAt, int hitAt, int hitKind, CV_TalentlessBardScript& s)
{
    Reset(); g_termAt = termAt; g_termFrame = termFrame; g_dieAt = dieAt; g_hitAt = hitAt; g_hitKind = hitKind; g_predicates = 0;
    static CGameScriptInterface game; s.Game = &game; s.AttackedByHero = false;
    s.WatchForAttack(CScriptThing(7));
}
int main()
{
    CV_TalentlessBardScript s;
    Run(1, 1 << 30, 1000, -1, 0, s); CHECK(g_predicates == 1 && g_frames == 1 && !s.AttackedByHero && g_live == 0);
    Run(1 << 30, 6, 1000, 3, 1, s); CHECK(s.AttackedByHero && g_frames == 6 && g_live == 0 && !g_badName);
    Run(1 << 30, 6, 2, 4, 2, s); CHECK(!s.AttackedByHero && g_frames == 6 && g_live == 0);   // dead before the hit frame
    Run(3, 1 << 30, 1000, -1, 0, s); CHECK(g_frames == 1 && g_live == 0 && g_iatt == 3);     // alive-block check
    // prologue(1); frame 1: top(2), alive(3); frame 2: top(4), alive(5), hit block(6)
    Run(6, 1 << 30, 1000, 2, 1, s); CHECK(!s.AttackedByHero && g_frames == 2 && g_live == 0 && g_iatt == 6);
    char line[64];
    _snprintf(line, sizeof(line), "00ed9790_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}