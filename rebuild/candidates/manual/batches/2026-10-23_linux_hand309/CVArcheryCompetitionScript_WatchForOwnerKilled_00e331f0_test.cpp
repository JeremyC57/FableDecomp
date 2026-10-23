#include <cstdio>
#include <cstring>
struct CCharString {
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
struct CTimer {
    long Index;
    CTimer() { Index = g_ntimers++; g_timerSet[Index] = 0; g_timerAt[Index] = g_frame; }
    void SetTime(long t) { g_timerSet[Index] = t; g_timerAt[Index] = g_frame; }
    long GetTime() const { return g_timerSet[Index] - (g_frame - g_timerAt[Index]); }
};
struct C3DVector { float X, Y, Z; };
// Things: an id into a small world table (health > 0 means alive).
static float g_health[64];
static C3DVector g_pos[64];
static int g_nextThing = 10;
static const char* g_data[64];
static bool (*g_isHit)(int id, int kind, const char* by) = 0;
struct CScriptThing {
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
// archery-owner harness: the world script advances per frame
static int g_stateAt1 = 2, g_aliveAt = 3, g_deadAt = 6, g_state2At = 1000;
static long* g_pState = 0;
static int g_teleport = -1, g_sheathe = -1, g_will = -1;
struct GameCalls {
    void NewScriptFrame() const
    {
        ++g_frame;
        if (g_frame == g_stateAt1) *g_pState = 1;
        if (g_frame == g_state2At) *g_pState = 2;
        if (g_frame == g_aliveAt) g_health[7] = 30.0f;
        if (g_frame == g_deadAt) g_health[7] = 0.0f;
    }
    CScriptThing GetThingWithScriptName(const CCharString& n) const { return CScriptThing(n.is("AC_Owner") ? 7 : 0); }
    float GetHealth(const CScriptThing& t) const { return g_health[t.id]; }
    CScriptThing& GetHero() { static CScriptThing hero(1); return hero; }
    void SetTeleportingAsActive(bool b) const { g_teleport = b; }
    void EntitySheatheWeapons(const CScriptThing& t, bool b) const { g_sheathe = t.id * 10 + b; }
    void SetHeroWillAsUsable(bool b) const { g_will = b; }
};
struct CV_ArcheryCompetitionScript {
    GameCalls* Game; bool QuestFinished; CTimer GameTimeLeft; long ContestState;
    bool IsActiveThreadTerminating() { return g_frame >= g_termAt; }
    void WatchForOwnerKilled();
};

void CV_ArcheryCompetitionScript::WatchForOwnerKilled()
{
    CScriptThing owner;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (ContestState != 1) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        owner = Game->GetThingWithScriptName("AC_Owner");
        while (!owner.IsAlive()) {
            Game->NewScriptFrame();
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
        }
#pragma inline_depth(0)
        if (IsActiveThreadTerminating())
            return;
#pragma inline_depth()
        for (;;) {
            if (IsActiveThreadTerminating())
                return;
            if (ContestState == 2) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                break;
            }
            if (Game->GetHealth(owner) < 0.0001f) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                GameTimeLeft.SetTime(0);
                ContestState = 0;
                QuestFinished = true;
                Game->SetTeleportingAsActive(true);
                Game->EntitySheatheWeapons(Game->GetHero(), false);
                Game->SetHeroWillAsUsable(true);
                break;
            }
            Game->NewScriptFrame();
        }
#pragma inline_depth(0)
        if (IsActiveThreadTerminating())
            return;
#pragma inline_depth()
        Game->NewScriptFrame();
    }
}

int main()
{
    GameCalls g; CV_ArcheryCompetitionScript s; s.Game = &g; s.QuestFinished = false; s.ContestState = 0; g_pState = &s.ContestState;
    s.GameTimeLeft.SetTime(500); g_termAt = 10;
    s.WatchForOwnerKilled();
    CHECK(s.QuestFinished && s.ContestState == 0 && s.GameTimeLeft.GetTime() <= 0 && g_teleport == 1 && g_sheathe == 10 && g_will == 1 && g_frame == 10);
    // contest ends normally (state 2) before the owner dies: no finish
    g_frame = 0; g_health[7] = 0.0f; g_teleport = g_sheathe = g_will = -1; s.QuestFinished = false; s.ContestState = 0;
    g_state2At = 5; g_deadAt = 1000;
    s.WatchForOwnerKilled();
    CHECK(!s.QuestFinished && s.ContestState == 2 && g_teleport == -1 && g_will == -1 && g_frame == 10);
    if (g_fail) { std::printf("00e331f0_TEST FAIL\n"); return 1; }
    std::printf("00e331f0_TEST PASS\n");
    return 0;
}
