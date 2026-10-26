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
struct CTimerS {
    long Index;
    CTimerS() { Index = g_ntimers++; g_timerSet[Index] = 0; g_timerAt[Index] = g_frame; }
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
// bordello harness: timers count down one tick per frame and stop at 0
struct CTimer {
    long setTo, setAt;
    CTimer() : setTo(0), setAt(0) {}
    void SetTime(long t) { setTo = t; setAt = g_frame; }
    long GetTime() const { long r = setTo - (g_frame - setAt); return r < 0 ? 0 : r; }
};
static long* g_pAlive = 0;
static int g_spawn_n = 0, g_spawnFrame[8];
static const char* g_spawnKind[8];
struct GameCalls {
    void NewScriptFrame() const { ++g_frame; }
    bool IsRegionLoaded(const CCharString& n) const { return n.is("Bordello") && g_frame < 40; }
    CScriptThing GetThingWithScriptName(const CCharString& n) const { return CScriptThing(n.is("M_ClientSpawn") ? 9 : 0); }
    CScriptThing CreateCreature(const CCharString& kind, const C3DVector& pos, const CCharString& name, bool b) const
    {
        if (&pos == &g_pos[9] && name.is("BordelloClient") && !b && g_spawn_n < 8) { g_spawnKind[g_spawn_n] = kind.p; g_spawnFrame[g_spawn_n] = g_frame; }
        ++g_spawn_n; ++*g_pAlive;
        return CScriptThing(g_nextThing++);
    }
};
struct CV_BordelloScript {
    GameCalls* Game; bool BecomeNunnery; long ClientsAlive;
    bool IsActiveThreadTerminating() { return g_frame >= g_termAt; }
    void CreateCustomers();
};

void CV_BordelloScript::CreateCustomers()
{
    CTimer timer;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (Game->IsRegionLoaded("Bordello")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (timer.GetTime() == 0 && ClientsAlive < 3) {
                if (IsActiveThreadTerminating())
                    return;
                timer.SetTime(15);
                if (BecomeNunnery) {
                    if (IsActiveThreadTerminating())
                        return;
                    Game->CreateCreature("CREATURE_BS_VILLAGER_FEMALE", Game->GetThingWithScriptName("M_ClientSpawn").GetPos(), "BordelloClient", false);
                } else {
                    if (IsActiveThreadTerminating())
                        return;
                    Game->CreateCreature("CREATURE_BS_VILLAGER_MALE", Game->GetThingWithScriptName("M_ClientSpawn").GetPos(), "BordelloClient", false);
                }
            }
        }
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
    }
}

int main()
{
    GameCalls g; CV_BordelloScript s; s.Game = &g; s.BecomeNunnery = false; s.ClientsAlive = 0; g_pAlive = &s.ClientsAlive; g_termAt = 50;
    s.CreateCustomers();
    CHECK(g_spawn_n == 3 && g_spawnFrame[0] == 1 && g_spawnFrame[1] == 16 && g_spawnFrame[2] == 31 && g_frame == 50);
    CHECK(std::strcmp(g_spawnKind[0], "CREATURE_BS_VILLAGER_MALE") == 0 && std::strcmp(g_spawnKind[2], "CREATURE_BS_VILLAGER_MALE") == 0);
    g_frame = 0; g_spawn_n = 0; s.BecomeNunnery = true; s.ClientsAlive = 2;
    s.CreateCustomers();
    CHECK(g_spawn_n == 1 && g_spawnFrame[0] == 1 && std::strcmp(g_spawnKind[0], "CREATURE_BS_VILLAGER_FEMALE") == 0);
    if (g_fail) { std::printf("00e3a350_TEST FAIL\n"); return 1; }
    std::printf("00e3a350_TEST PASS\n");
    return 0;
}