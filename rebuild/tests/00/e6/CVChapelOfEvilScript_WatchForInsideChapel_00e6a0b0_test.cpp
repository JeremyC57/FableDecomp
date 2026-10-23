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
static float g_angle[64];
static const char* g_defName[64];
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
    float GetAngleXY() const { return g_angle[id]; }
    bool IsNull() const { return id == 0; }
    CCharString GetDefName() const { return CCharString(g_defName[id] ? g_defName[id] : ""); }
};
static char g_cat[8][96]; static int g_ncat = 0;
inline CCharString operator+(const CCharString& a, const CCharString& b)
{
    char* buf = g_cat[g_ncat++ & 7]; std::strcpy(buf, a.p); std::strcat(buf, b.p); return CCharString(buf);
}
inline CCharString operator+(const CCharString& a, const char* b) { return a + CCharString(b); }
inline CCharString operator+(const char* a, const CCharString& b) { return CCharString(a) + b; }
// chapel harness: the hero (1) walks none,none,chapel,chapel,other,none... ; InsideChapel sampled each frame
static bool* g_pInside = 0; static int g_seen[16];
struct GameCalls {
    void NewScriptFrame() const { if (g_frame < 16) g_seen[g_frame] = *g_pInside; ++g_frame; }
    CScriptThing& GetHero() { static CScriptThing hero(1); return hero; }
    CScriptThing EntityGetStandingInsideBuilding(const CScriptThing& t) const
    {
        if (t.id != 1) return CScriptThing(0);
        if (g_frame == 2 || g_frame == 3) return CScriptThing(50);
        if (g_frame == 4) return CScriptThing(51);
        return CScriptThing(0);
    }
};
struct CV_ChapelOfEvilScript {
    GameCalls* Game; bool InsideChapel;
    bool IsActiveThreadTerminating() { return g_frame >= g_termAt; }
    void WatchForInsideChapel();
};

void CV_ChapelOfEvilScript::WatchForInsideChapel()
{
    CScriptThing building;
    for (;;) {
        if (IsActiveThreadTerminating()) return;
        if (Game->GetHero().IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            building = Game->EntityGetStandingInsideBuilding(Game->GetHero());
            if (!building.IsNull() && building.GetDefName() == "BUILDING_CHAPEL_EVIL" && !InsideChapel) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating()) return;
#pragma inline_depth()
                InsideChapel = true;
            } else if (building.IsNull() && InsideChapel) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating()) return;
#pragma inline_depth()
                InsideChapel = false;
            }
        }
        Game->NewScriptFrame();
    }
}

int main()
{
    GameCalls g; CV_ChapelOfEvilScript s; s.Game = &g; s.InsideChapel = false; g_pInside = &s.InsideChapel;
    g_health[1] = 10.0f; g_defName[50] = "BUILDING_CHAPEL_EVIL"; g_defName[51] = "BUILDING_TAVERN"; g_termAt = 8;
    s.WatchForInsideChapel();
    CHECK(!g_seen[0] && !g_seen[1] && g_seen[2] && g_seen[3] && g_seen[4] && !g_seen[5] && !g_seen[6] && !s.InsideChapel);
    // a dead hero is not tracked
    g_frame = 0; g_health[1] = 0.0f;
    s.WatchForInsideChapel();
    CHECK(!g_seen[2] && !g_seen[3] && g_frame == 8);
    if (g_fail) { std::printf("00e6a0b0_TEST FAIL\n"); return 1; }
    std::printf("00e6a0b0_TEST PASS\n");
    return 0;
}