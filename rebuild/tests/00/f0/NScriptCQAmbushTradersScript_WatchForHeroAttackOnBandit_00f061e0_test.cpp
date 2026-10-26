// Behaviour test for 0x00F061E0 (RST2): model plus the body verbatim from final.cpp, callees stubbed.
// Checks that a melee or special-ability hit on a bandit with health left flags the attack and counts it, a hit on a
// bandit at zero health does not, termination at each of the three tests, and the release of the by-value bandit on every exit.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#define _snprintf snprintf
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static int g_live = 0, g_iatt = 0, g_termAt = 1 << 30, g_frames = 0, g_hitKind = 0, g_stopFrame = 1 << 30;
static bool g_badName = false;
static float g_health = 100.0f;
static bool Term() { ++g_iatt; return g_iatt >= g_termAt || g_frames >= g_stopFrame; }
struct CCharString {
    const char* Text;
    CCharString(const char* text, long length = -1) : Text(text) { (void)length; }
    ~CCharString() { }
};
static bool HitCheck(const CCharString& a, int kind)
{
    if (strcmp(a.Text, "SCRIPT_NAME_HERO") != 0) g_badName = true;
    return (g_frames % 2) == 1 && g_hitKind == kind;   // hit on odd frames
}
struct CScriptThing {
    int Id;
    explicit CScriptThing(int id) : Id(id) { ++g_live; }
    CScriptThing(const CScriptThing& o) : Id(o.Id) { ++g_live; }
    __forceinline ~CScriptThing() { --g_live; }
    bool MsgIsHitBy(const CCharString& a) const { return HitCheck(a, 1); }
    bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& a) const { return HitCheck(a, 2); }
};
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; }
    float GetHealth(const CScriptThing& t) const { if (t.Id != 5) g_badName = true; return g_health; }
};
struct CScriptBase {
    bool IsActiveThreadTerminating() { return Term(); }
};
struct CQ_AmbushTradersScript : CScriptBase {
    CGameScriptInterface* GSI;
    bool BanditAttackedByHero;
    long BanditAttackByHeroCount;
    void WatchForHeroAttackOnBandit(CScriptThing bandit);
};

void CQ_AmbushTradersScript::WatchForHeroAttackOnBandit(CScriptThing bandit)
{
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        bool attacked = bandit.MsgIsHitBy("SCRIPT_NAME_HERO") ||
                        bandit.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO");
        if (attacked) {
            if (IsActiveThreadTerminating())
                return;
            if (GSI->GetHealth(bandit) > 0.0001f) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                BanditAttackedByHero = true;
                ++BanditAttackByHeroCount;
            }
        }
        GSI->NewScriptFrame();
    }
}

static void Run(CQ_AmbushTradersScript& s, int termAt, int stopFrame, int kind, float health)
{
    static CGameScriptInterface gsi;
    g_live = 0; g_iatt = 0; g_frames = 0; g_badName = false;
    g_termAt = termAt; g_stopFrame = stopFrame; g_hitKind = kind; g_health = health;
    s.GSI = &gsi; s.BanditAttackedByHero = false; s.BanditAttackByHeroCount = 0;
    s.WatchForHeroAttackOnBandit(CScriptThing(5));
}
int main()
{
    CQ_AmbushTradersScript s;
    Run(s, 1, 1 << 30, 1, 50.0f); CHECK(g_frames == 0 && !s.BanditAttackedByHero && g_live == 0);
    Run(s, 1 << 30, 6, 1, 50.0f); CHECK(g_frames == 6 && s.BanditAttackedByHero && s.BanditAttackByHeroCount == 3 && g_live == 0 && !g_badName);
    Run(s, 1 << 30, 6, 2, 50.0f); CHECK(s.BanditAttackByHeroCount == 3 && g_live == 0);
    Run(s, 1 << 30, 6, 1, 0.0f);  CHECK(!s.BanditAttackedByHero && s.BanditAttackByHeroCount == 0 && g_live == 0);
    Run(s, 1 << 30, 6, 0, 50.0f); CHECK(!s.BanditAttackedByHero && g_frames == 6 && g_live == 0);
    Run(s, 3, 1 << 30, 1, 50.0f); CHECK(g_frames == 1 && !s.BanditAttackedByHero && g_iatt == 3 && g_live == 0);   // the frame-1 hit's first re-test
    Run(s, 4, 1 << 30, 1, 50.0f); CHECK(g_frames == 1 && !s.BanditAttackedByHero && g_iatt == 4 && g_live == 0);   // the out-of-line exit
    char line[64];
    _snprintf(line, sizeof(line), "00f061e0_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}