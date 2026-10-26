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
static float g_angle[64];
static const char* g_defName[64];
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
// hero-duel harness (Thunder is thing 5, the hero 1)
struct CV_HeroDuelScript;
static CV_HeroDuelScript* g_duel = 0;
static int g_levelAt = 9, g_scriptAt = 7;
static int g_target = -1, g_modN = 0; static float g_mod[4]; static int g_allowFollow = -1, g_regionFollow = -1;
static int g_flee1 = -1, g_flee2 = -1, g_lost = -1, g_releases = 0;
class CScriptGameResourceObjectScriptedThingBase { public:
    int thing; CScriptGameResourceObjectScriptedThingBase() : thing(0) {}
    bool IsScripting() { return thing != 0; }
    void Release() { thing = 0; ++g_releases; }
};
struct GameCalls {
    void NewScriptFrame() const;
    CScriptThing& GetHero() { static CScriptThing hero(1); return hero; }
    void GiveThingBestEnemyTarget(const CScriptThing& t, const CScriptThing& e) { g_target = t.id * 10 + e.id; }
    float GetHealth(const CScriptThing& t) const { return g_health[t.id]; }
    void ModifyThingHealth(const CScriptThing& t, float d, bool b) { if (!b && t.id == 5 && g_modN < 4) g_mod[g_modN++] = d; }
    bool IsLevelLoaded(const CCharString& n) const { return n.is("HeroDualAreaCave") && g_frame >= g_levelAt; }
    bool StartScriptingEntity(const CScriptThing& t, CScriptGameResourceObjectScriptedThingBase& h, long m) { if (g_frame < g_scriptAt || m != 4) return false; h.thing = t.id; return true; }
    void EntitySetAsAllowedToFollowHero(const CScriptThing& t, bool b) { g_allowFollow = t.id * 10 + b; }
    void SetEntityAsRegionFollowing(const CScriptThing& a, const CScriptThing& b, bool f) { g_regionFollow = a.id * 100 + b.id * 10 + f; }
};
struct CPredicateBase { virtual bool IsTrue() = 0; virtual ~CPredicateBase() {} };
struct CIsThingAlive : CPredicateBase { CScriptThing Thing; CIsThingAlive(const CScriptThing& t) : Thing(t) {} bool IsTrue() { return Thing.IsAlive(); } };
struct CV_HeroDuelScript {
    bool DuelStarted; unsigned long MyHealth;
    void FirstFleeCutscene() { g_flee1 = g_frame; }
    void SecondFleeCutscene() { g_flee2 = g_frame; }
    void DuelLost() { g_lost = g_frame; }
    struct CThunder;
};
void GameCalls::NewScriptFrame() const
{
    ++g_frame;
    if (g_frame == 2) g_duel->DuelStarted = true;
    if (g_frame == 4) g_duel->MyHealth = 50;
    if (g_frame == 6) g_duel->MyHealth = 30;
    if (g_frame == 11) g_duel->MyHealth = 5;
}
static int g_pred = -1;
struct CV_HeroDuelScript::CThunder {
    GameCalls* Game; CScriptThing Me; CV_HeroDuelScript* ParentClass; unsigned long MaxHealth; unsigned long RecoveredHealth;
    void SetPredicate(const CPredicateBase& p) { g_pred = ((const CIsThingAlive&)p).Thing.id; }
    bool IsActiveThreadTerminating() { return g_frame >= g_termAt; }
    void Main();
};

void CV_HeroDuelScript::CThunder::Main()
{
    SetPredicate(CIsThingAlive(Me));
    while (!ParentClass->DuelStarted) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    while (ParentClass->MyHealth > MaxHealth / 2 + 5) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    MaxHealth = MaxHealth / 2 + 5;
    RecoveredHealth = (unsigned long)((float)MaxHealth - Game->GetHealth(Me));
    Game->ModifyThingHealth(Me, (float)RecoveredHealth, false);
    ParentClass->FirstFleeCutscene();
    while (ParentClass->MyHealth > MaxHealth / 2 + 5) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    MaxHealth = MaxHealth / 2 + 5;
    RecoveredHealth = (unsigned long)((float)MaxHealth - Game->GetHealth(Me));
    Game->ModifyThingHealth(Me, (float)RecoveredHealth, false);
    if (!Game->IsLevelLoaded("HeroDualAreaCave")) {
        if (IsActiveThreadTerminating())
            return;
        ParentClass->SecondFleeCutscene();
    }
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsScripting())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, 4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!Game->IsLevelLoaded("HeroDualAreaCave")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (handle.IsScripting())
        handle.Release();
    Game->EntitySetAsAllowedToFollowHero(Me, false);
    Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, false);
    while (ParentClass->MyHealth > 5) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->DuelLost();
}

int main()
{
    GameCalls g; CV_HeroDuelScript d; d.DuelStarted = false; d.MyHealth = 100; g_duel = &d;
    CV_HeroDuelScript::CThunder t; t.Game = &g; t.Me = CScriptThing(5); t.ParentClass = &d; t.MaxHealth = 100; t.RecoveredHealth = 0;
    g_health[5] = 20.0f; g_termAt = 30;
    t.Main();
    CHECK(g_pred == 5 && g_target == 51 && g_flee1 == 4 && g_flee2 == 6 && g_lost == 11 && t.MaxHealth == 32 && t.RecoveredHealth == 12);
    CHECK(g_modN == 2 && g_mod[0] == 35.0f && g_mod[1] == 12.0f && g_releases == 1 && g_allowFollow == 50 && g_regionFollow == 150);
    // cave already loaded: no second flee cut-scene; terminated before the health falls
    g_frame = 0; d.DuelStarted = false; d.MyHealth = 100; t.MaxHealth = 100; g_flee2 = -1; g_lost = -1; g_levelAt = 0; g_termAt = 10; g_releases = 0;
    t.Main();
    CHECK(g_flee2 == -1 && g_lost == -1 && g_releases == 1 && g_frame == 10);
    if (g_fail) { std::printf("00e9a5c0_TEST FAIL\n"); return 1; }
    std::printf("00e9a5c0_TEST PASS\n");
    return 0;
}