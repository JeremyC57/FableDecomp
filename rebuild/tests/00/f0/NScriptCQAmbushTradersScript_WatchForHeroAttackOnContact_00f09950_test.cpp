// Behaviour test for 0x00F09950 (RST): model plus the body verbatim from final.cpp, callees stubbed.
// Checks that a hit sets ContactAttackedByHero and the watch keeps running (no break) until the contact dies, the
// immediate-terminate exit, and termination inside the hit block (no flag); the by-value contact is released on every exit.
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
static int g_dieAt = 1 << 30;           // the watched thing is alive while g_frames < g_dieAt
static int g_hitAt = -1, g_hitKind = 0; // hit at this frame: 1 melee (MsgIsHitBy), 2 aggressive special ability
static float g_health = 100.0f;
static bool HitCheck(const CCharString& a, int kind)
{
    if (strcmp(a.Text, "SCRIPT_NAME_HERO") != 0) g_badName = true;
    return g_frames == g_hitAt && g_hitKind == kind;
}
struct CScriptThing {
    int Id;
    explicit CScriptThing(int id) : Id(id) { ++g_live; }
    CScriptThing(const CScriptThing& o) : Id(o.Id) { ++g_live; }
    __forceinline ~CScriptThing() { --g_live; }
    bool IsAlive() const { return g_frames < g_dieAt; }
    bool MsgIsHitBy(const CCharString& a) const { return HitCheck(a, 1); }
    bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& a) const { return HitCheck(a, 2); }
};
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; }
    float GetHealth(const CScriptThing& t) const { if (t.Id != 7) g_badName = true; return g_health; }
};
struct CQuestScriptBase {
    CGameScriptInterface* Game;
    bool IsActiveThreadTerminating() { return Term(); }
};

struct CQ_AmbushTradersScript : CQuestScriptBase {
    bool ContactAttackedByHero;
    void WatchForHeroAttackOnContact(CScriptThing contact);
};

void CQ_AmbushTradersScript::WatchForHeroAttackOnContact(CScriptThing contact)
{
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (!contact.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            break;
        }
        if (contact.MsgIsHitBy("SCRIPT_NAME_HERO") || contact.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO")) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            ContactAttackedByHero = true;
        }
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        return;
}

static void Run(int termAt, int dieAt, int hitAt, int hitKind, CQ_AmbushTradersScript& s)
{
    Reset(); g_termAt = termAt; g_dieAt = dieAt; g_hitAt = hitAt; g_hitKind = hitKind;
    static CGameScriptInterface game; s.Game = &game; s.ContactAttackedByHero = false;
    s.WatchForHeroAttackOnContact(CScriptThing(7));
}
int main()
{
    CQ_AmbushTradersScript s;
    Run(1, 1000, -1, 0, s);  CHECK(!s.ContactAttackedByHero && g_frames == 0 && g_live == 0);
    Run(1 << 30, 5, 1, 1, s); CHECK(s.ContactAttackedByHero && g_frames == 5 && g_live == 0 && !g_badName);
    Run(1 << 30, 4, 2, 2, s); CHECK(s.ContactAttackedByHero && g_frames == 4 && g_live == 0);
    Run(1 << 30, 3, -1, 0, s); CHECK(!s.ContactAttackedByHero && g_frames == 3 && g_live == 0);
    Run(3, 1000, 1, 1, s);   CHECK(!s.ContactAttackedByHero && g_frames == 1 && g_live == 0);
    char line[64];
    _snprintf(line, sizeof(line), "00f09950_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}