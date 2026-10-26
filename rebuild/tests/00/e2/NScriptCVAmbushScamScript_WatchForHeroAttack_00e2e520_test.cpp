#include <cstdio>
#include <cstring>
class CCharString { public:
    const char* p;
    CCharString() : p("") {}
    CCharString(const char* t, long = -1) : p(t) {}
    CCharString(const CCharString& o) : p(o.p) {}
    bool is(const char* t) const { return std::strcmp(p, t) == 0; }
    bool operator==(const char* t) const { return is(t); }
    bool operator!=(const char* t) const { return !is(t); }
};
static int g_fail = 0;
static void check(bool c, int line) { if (!c) { if (!g_fail) std::printf("check failed at line %d\n", line); g_fail = 1; } }
#define CHECK(c) check((c), __LINE__)
static int g_frame = 0;
static int g_termAt = 100;
static int g_log_n = 0;
static char g_log[64][96];
static void LOG(const char* a, const char* b = "")
{
    if (g_log_n < 64) { std::strncpy(g_log[g_log_n], a, 47); g_log[g_log_n][47] = 0; std::strncat(g_log[g_log_n], b, 47); }
    ++g_log_n;
}
static int FindLog(const char* s)
{
    for (int i = 0; i < g_log_n && i < 64; ++i) if (std::strcmp(g_log[i], s) == 0) return i;
    return -1;
}
// Script timers: SetTime(t) at frame f reads back t - (frame - f).
static long g_timerSet[8], g_timerAt[8]; static int g_ntimers = 0;
class CTimer { public:
    long Index;
    CTimer() { Index = g_ntimers++; g_timerSet[Index] = 0; g_timerAt[Index] = g_frame; }
    void SetTime(long t) { g_timerSet[Index] = t; g_timerAt[Index] = g_frame; }
    long GetTime() const { return g_timerSet[Index] - (g_frame - g_timerAt[Index]); }
};
class C3DVector { public: float X, Y, Z; };
// Things: an id into a small world table (health > 0 means alive).
static float g_health[64];
static C3DVector g_pos[64];
static int g_nextThing = 10;
static const char* g_data[64];
static bool (*g_isHit)(int id, int kind, const char* by) = 0;
class CScriptThing { public:
    int id;
    CScriptThing() : id(0) {}
    explicit CScriptThing(int i) : id(i) {}
    bool IsAlive() const { return g_health[id] > 0.0f; }
    const C3DVector& GetPos() const { return g_pos[id]; }
    bool MsgIsHitBy(const CCharString& by) const { return g_isHit && g_isHit(id, 0, by.p); }
    bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& by) const { return g_isHit && g_isHit(id, 1, by.p); }
    bool MsgIsUsedBy(const CCharString& by) const { return g_isHit && g_isHit(id, 2, by.p); }
    CCharString GetDataString() const { return CCharString(g_data[id]); }
};
static char g_cat[8][96]; static int g_ncat = 0;
inline CCharString operator+(const CCharString& a, const CCharString& b)
{
    char* buf = g_cat[g_ncat++ & 7]; std::strcpy(buf, a.p); std::strcat(buf, b.p); return CCharString(buf);
}
inline CCharString operator+(const CCharString& a, const char* b) { return a + CCharString(b); }
static bool Hits(int id, int kind, const char* by)
{
    if (std::strcmp(by, "SCRIPT_NAME_HERO") != 0) return false;
    if (id == 3) return kind == 0 && (g_frame == 2 || g_frame == 4 || g_frame == 6 || g_frame == 8);
    if (id == 4) return kind == 1 && g_frame == 3;
    if (id == 5) return kind == 0 && g_frame == 1;
    return false;
}
struct GameCalls {
    void NewScriptFrame() { ++g_frame; }
    float GetHealth(const CScriptThing& t) { return g_health[t.id]; }
};
struct CV_AmbushScamScript {
    GameCalls* Game; bool DoAmbush; bool HeroAttackOnBuddy; bool HeroAttackOnTricksterBeforeAmbush; bool HeroAttackOnTricksterAfterAmbush;
    bool HeroAttackOnTricksterKillingBlow; bool HeroAttachOnTricksterWarning; long HitCount;
    bool IsActiveThreadTerminating() { if (g_frame == 5) DoAmbush = true; return g_frame >= g_termAt; }
    void WatchForHeroAttack(CScriptThing target);
};

void CV_AmbushScamScript::WatchForHeroAttack(CScriptThing target)
{
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (target.MsgIsHitBy("SCRIPT_NAME_HERO") || target.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            if (Game->GetHealth(target) > 0.0001f) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                ++HitCount;
                if (target.GetDataString() == "AS_Trickster") {
#pragma inline_depth(0)
                    if (IsActiveThreadTerminating())
                        return;
#pragma inline_depth()
                    if (DoAmbush) {
#pragma inline_depth(0)
                        if (IsActiveThreadTerminating())
                            return;
#pragma inline_depth()
                        if (HitCount > 2) {
#pragma inline_depth(0)
                            if (IsActiveThreadTerminating())
                                return;
                            HeroAttackOnTricksterAfterAmbush = true;
                            IsActiveThreadTerminating();
                            return;
#pragma inline_depth()
                        }
#pragma inline_depth(0)
                        if (IsActiveThreadTerminating())
                            return;
#pragma inline_depth()
                        HeroAttachOnTricksterWarning = true;
                    } else {
#pragma inline_depth(0)
                        if (IsActiveThreadTerminating())
                            return;
#pragma inline_depth()
                        HeroAttackOnTricksterBeforeAmbush = true;
                    }
                } else if (target.GetDataString() == "AS_Buddy") {
#pragma inline_depth(0)
                    if (IsActiveThreadTerminating())
                        return;
#pragma inline_depth()
                    HeroAttackOnBuddy = true;
                }
            } else if (HitCount == 0) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                HeroAttackOnTricksterKillingBlow = true;
            }
        }
        Game->NewScriptFrame();
    }
}

static void Reset(CV_AmbushScamScript& s)
{
    s.DoAmbush = false; s.HeroAttackOnBuddy = false; s.HeroAttackOnTricksterBeforeAmbush = false; s.HeroAttackOnTricksterAfterAmbush = false;
    s.HeroAttackOnTricksterKillingBlow = false; s.HeroAttachOnTricksterWarning = false; s.HitCount = 0; g_frame = 0;
}
int main()
{
    g_isHit = Hits; g_data[3] = "AS_Trickster"; g_data[4] = "AS_Buddy"; g_data[5] = "AS_Trickster";
    g_health[3] = 50.0f; g_health[4] = 50.0f; g_health[5] = 0.0f;
    GameCalls g; CV_AmbushScamScript s; s.Game = &g; Reset(s); g_termAt = 30;
    s.WatchForHeroAttack(CScriptThing(3));
    // hit at 2 (before ambush), 4 (before), 6 (ambush on, count 3 -> after-ambush, return)
    CHECK(s.HeroAttackOnTricksterBeforeAmbush && s.HeroAttackOnTricksterAfterAmbush && !s.HeroAttachOnTricksterWarning && s.HitCount == 3 && g_frame == 6);
    Reset(s);
    s.WatchForHeroAttack(CScriptThing(4));
    CHECK(s.HeroAttackOnBuddy && s.HitCount == 1 && g_frame == 30);
    Reset(s);
    s.WatchForHeroAttack(CScriptThing(5));
    CHECK(s.HeroAttackOnTricksterKillingBlow && s.HitCount == 0);
    if (g_fail) { std::printf("00e2e520_TEST FAIL\n"); return 1; }
    std::printf("00e2e520_TEST PASS\n");
    return 0;
}