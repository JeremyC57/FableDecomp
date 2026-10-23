#include <cstdio>
#include <cstring>
struct CCharString {
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
struct CScriptThing {
    int id;
    CScriptThing() : id(0) {}
    explicit CScriptThing(int i) : id(i) {}
    bool IsAlive() const { return g_health[id] > 0.0f; }
    const C3DVector& GetPos() const { return g_pos[id]; }
};
static int g_effects = 0, g_markers = 0;
static float g_alpha = 1.0f;
bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float d) { return a.id == 2 && d == 0.5f && b.id >= 10; }
struct GameCalls {
    void NewScriptFrame() { ++g_frame; }
    void Pause(float) { ++g_frame; }
    CScriptThing CreateCreature(const CCharString& def, const C3DVector& p, const CCharString& script, bool)
    {
        if (!def.is("CREATURE_OK_UNDEAD_LIEUTENANT") || !script.is("UndeadRisingZombieAttackerBoss") || p.X != 4.0f) g_fail = 1;
        int i = g_nextThing++; g_health[i] = 1.0f; return CScriptThing(i);
    }
    void MiniMapAddMarker(const CScriptThing& t, const CCharString& m) { if (m.is("HUD_ORB_QUEST_CORE") && t.id >= 10) ++g_markers; }
    void EntitySetAlpha(const CScriptThing&, float a, bool) { g_alpha = a; }
    void ModifyThingHealth(const CScriptThing& t, float d, bool) { g_health[t.id] -= d; }
    CScriptThing CreateEffect(const CCharString& name, const C3DVector&, const CCharString&, float, bool, bool)
    {
        if (!name.is("GRAVEYARD_CIRCLE_LIGHT_LARGE")) g_fail = 1;
        ++g_effects; int i = g_nextThing++; g_health[i] = (g_effects == 1) ? 1.0f : 0.0f; if (g_effects == 3) g_health[i] = 1.0f;
        return CScriptThing(i);
    }
};
struct CQ_UndeadRisingScript { unsigned long number_of_boss_kills_in_right_area; long quest_status; };
struct CUndeadRisingBoss {
    GameCalls* Game;
    CScriptThing Me;
    CQ_UndeadRisingScript* ParentClass;
    bool IsActiveThreadTerminating()
    {
        if (g_frame == 3) ParentClass->quest_status = 2;
        if (g_frame == 12) g_health[11] = 0.0f;   // the first light burns out
        return g_frame >= g_termAt;
    }
    void Main();
};

void CUndeadRisingBoss::Main()
{
    while (ParentClass->quest_status != 2) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing boss = Game->CreateCreature("CREATURE_OK_UNDEAD_LIEUTENANT", Me.GetPos(), "UndeadRisingZombieAttackerBoss", false);
    Game->MiniMapAddMarker(boss, "HUD_ORB_QUEST_CORE");
    Game->EntitySetAlpha(boss, 0.0f, true);
    while (boss.IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsUnder(Me, boss, 0.5f)) {
            if (IsActiveThreadTerminating())
                return;
            Game->ModifyThingHealth(boss, 0.5f, false);
            Game->Pause(1.0f);
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->Pause(1.0f);
    ++ParentClass->number_of_boss_kills_in_right_area;
    while (!IsActiveThreadTerminating()) {
        {
            CScriptThing light = Game->CreateEffect("GRAVEYARD_CIRCLE_LIGHT_LARGE", Me.GetPos(), "", 0.0f, false, false);
            while (light.IsAlive()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            while (!light.IsAlive()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                Game->Pause(3.0f);
                light = Game->CreateEffect("GRAVEYARD_CIRCLE_LIGHT_LARGE", Me.GetPos(), "", 0.0f, false, false);
            }
            if (IsActiveThreadTerminating())
                return;
        }
        Game->NewScriptFrame();
    }
}

int main()
{
    GameCalls g; CQ_UndeadRisingScript q; q.number_of_boss_kills_in_right_area = 0; q.quest_status = 0;
    CUndeadRisingBoss b; b.Game = &g; b.Me = CScriptThing(2); b.ParentClass = &q; g_pos[2].X = 4.0f;
    g_termAt = 30;
    b.Main();
    CHECK(g_markers == 1 && g_alpha == 0.0f && g_health[10] <= 0.0f && q.number_of_boss_kills_in_right_area == 1);
    CHECK(g_effects >= 3 && g_frame == 30);
    if (g_fail) { std::printf("00e0d210_TEST FAIL\n"); return 1; }
    std::printf("00e0d210_TEST PASS\n");
    return 0;
}