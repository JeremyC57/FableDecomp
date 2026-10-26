#include <cstdio>
#include <cstring>
class CCharString { public:
    const char* p;
    CCharString() : p("") {}
    CCharString(const char* t, long = -1) : p(t) {}
    CCharString(const CCharString& o) : p(o.p) {}
    bool is(const char* t) const { return std::strcmp(p, t) == 0; }
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
class CScriptThing { public:
    int id;
    CScriptThing() : id(0) {}
    explicit CScriptThing(int i) : id(i) {}
    bool IsAlive() const { return g_health[id] > 0.0f; }
    const C3DVector& GetPos() const { return g_pos[id]; }
};
static char g_cat[8][96]; static int g_ncat = 0;
inline CCharString operator+(const CCharString& a, const CCharString& b)
{
    char* buf = g_cat[g_ncat++ & 7]; std::strcpy(buf, a.p); std::strcat(buf, b.p); return CCharString(buf);
}
inline CCharString operator+(const CCharString& a, const char* b) { return a + CCharString(b); }
static int g_preds = 0, g_moves = 0, g_released = 0, g_target = 0, g_scripting = 0;
static float g_hp = 30.0f;
struct CIsThingConscious { CIsThingConscious(const CScriptThing&) {} };
class CScriptGameResourceObjectScriptedThingBase { public:
    bool active;
    CScriptGameResourceObjectScriptedThingBase() : active(false) {}
    bool IsScripting() const { return active; }
    void Release() { ++g_released; active = false; }
    void MoveToPosition(const C3DVector& p, float, float, bool, bool) { if (p.X == 7.0f) ++g_moves; }
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_3 = 3 };
struct GameCalls {
    CScriptThing hero;
    void NewScriptFrame() { ++g_frame; g_hp += 10.0f; }
    bool StartScriptingEntity(const CScriptThing& t, CScriptGameResourceObjectScriptedThingBase& h, EScriptAIPriority p)
    {
        if (g_frame < 6 || p != SCRIPT_AI_PRIORITY_3 || t.id != 2) return false;
        h.active = true; ++g_scripting; return true;
    }
    float GetHealth(const CScriptThing&) { return g_hp; }
    CScriptThing& GetHero() { return hero; }
    void GiveThingBestEnemyTarget(const CScriptThing& a, const CScriptThing& b) { if (a.id == 2 && b.id == 1) ++g_target; }
};
class CSpawnedFuncBase { public: virtual ~CSpawnedFuncBase() {} };
struct CQ_UndeadRisingScript;
typedef void (CQ_UndeadRisingScript::*SpawnedThingCallback)(CScriptThing);
static char g_spawnName[96]; static int g_added = 0;
struct CSpawnedFunc1 : CSpawnedFuncBase {
    SpawnedThingCallback Func; CQ_UndeadRisingScript* PClass; CScriptThing Par1;
    CSpawnedFunc1(const CCharString& name, unsigned long, SpawnedThingCallback f, CQ_UndeadRisingScript* c, const CScriptThing& t)
        : Func(f), PClass(c), Par1(t) { std::strcpy(g_spawnName, name.p); }
};
struct CQ_UndeadRisingScript {
    bool BossCutscenePlayed; int looked;
    void LookAfterHealth(CScriptThing t) { looked = t.id; }
    void Add(CSpawnedFuncBase* f, const CCharString& region)
    {
        CSpawnedFunc1* s = static_cast<CSpawnedFunc1*>(f);
        if (region.is("")) { (this->*(s->Func))(s->Par1); ++g_added; }
        delete f;
    }
};
struct CUndeadRisingZombieAttackerBoss {
    GameCalls* Game; CScriptThing Me; CQ_UndeadRisingScript* ParentClass; C3DVector StartPos; float MaxHealth;
    void SetPredicate(const CIsThingConscious&) { ++g_preds; }
    bool IsActiveThreadTerminating() { if (g_frame == 4) ParentClass->BossCutscenePlayed = true; return g_frame >= g_termAt; }
    void Main();
};

void CUndeadRisingZombieAttackerBoss::Main()
{
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    while (!ParentClass->BossCutscenePlayed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->Add(new CSpawnedFunc1(CCharString("ParentClass.") + CCharString("LookAfterHealth"), 0,
                                       &CQ_UndeadRisingScript::LookAfterHealth, ParentClass, Me), CCharString(""));
    if (handle.IsScripting())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, SCRIPT_AI_PRIORITY_3)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (Game->GetHealth(Me) < MaxHealth * 0.5f) {
        if (IsActiveThreadTerminating())
            return;
        handle.MoveToPosition(StartPos, 0.0f, 0.0f, true, true);
        while (Game->GetHealth(Me) < MaxHealth) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}

int main()
{
    GameCalls g; g.hero = CScriptThing(1); CQ_UndeadRisingScript q; q.BossCutscenePlayed = false; q.looked = 0;
    CUndeadRisingZombieAttackerBoss b; b.Game = &g; b.Me = CScriptThing(2); b.ParentClass = &q;
    b.StartPos.X = 7.0f; b.StartPos.Y = 0; b.StartPos.Z = 0; b.MaxHealth = 200.0f; g_termAt = 50; g_hp = 0.0f;
    b.Main();
    CHECK(g_preds == 1 && g_added == 1 && q.looked == 2 && std::strcmp(g_spawnName, "ParentClass.LookAfterHealth") == 0);
    CHECK(g_scripting == 1 && g_released == 0 && g_moves == 1 && g_target == 1 && g_frame == 50);
    if (g_fail) { std::printf("00e0e2e0_TEST FAIL\n"); return 1; }
    std::printf("00e0e2e0_TEST PASS\n");
    return 0;
}