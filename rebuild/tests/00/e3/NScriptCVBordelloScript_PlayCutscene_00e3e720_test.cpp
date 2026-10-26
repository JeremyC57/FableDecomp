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
// cut-scene harness
struct CScriptGameResourceObjectScriptedThingBase { int thing; int mode; CScriptGameResourceObjectScriptedThingBase() : thing(0), mode(0) {} };
struct CCutsceneCastMap {
    const char* keys[8]; CScriptGameResourceObjectScriptedThingBase vals[8]; int n;
    CCutsceneCastMap() : n(0) {}
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& k)
    {
        for (int i = 0; i < n; ++i) if (std::strcmp(keys[i], k.p) == 0) return vals[i];
        keys[n] = k.p; return vals[n++];
    }
    int Find(const char* k) const { for (int i = 0; i < n; ++i) if (std::strcmp(keys[i], k) == 0) return vals[i].thing; return -1; }
};
struct CCutsceneArgMap { int n; void clear() { n = 0; } };
struct CCutsceneFlagMap;
static bool g_camFixed = false, g_skippable = false;
static int g_runs = 0, g_castHero = -1, g_castBoss = -1, g_castWhore = -1, g_castN = 0; static bool g_runCam = false, g_runSkip = false; static const char* g_runName = "";
static int g_runArgs = -1;
static void RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6)
{
    ++g_runs; g_runName = name.p; g_castHero = cast.Find("HERO"); g_castBoss = cast.Find("BOSS"); g_castWhore = cast.Find("WHORE"); g_castN = cast.n;
    g_runCam = g_camFixed; g_runSkip = g_skippable; g_runArgs = (flags == 0 && args && !a5 && a6) ? args->n : -1;
}
struct GameCalls {
    CScriptThing& GetHero() { static CScriptThing hero(1); return hero; }
    void StartScriptingEntity(const CScriptThing& t, CScriptGameResourceObjectScriptedThingBase& seh, long mode) { seh.thing = t.id; seh.mode = (int)mode; }
    void FixMovieSequenceCamera(bool b) { g_camFixed = b; }
    void SetCutsceneSkippableWhilePaused(bool b) { g_skippable = b; }
};
struct CV_BordelloScript {
    GameCalls* Game; bool CutscenePlaying; bool TermNow;
    CScriptGameResourceObjectScriptedThingBase seh_Boss, seh_Guard, seh_Madam, seh_Whore;
    CCutsceneArgMap csargs;
    bool IsActiveThreadTerminating() { return TermNow; }
    void PlayCutscene(CCharString name, bool skippable);
};

void CV_BordelloScript::PlayCutscene(CCharString name, bool skippable)
{
    CutscenePlaying = true;
    CScriptGameResourceObjectScriptedThingBase hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, 4);
    CCutsceneCastMap cast;
    cast["HERO"] = hero;
    cast["BOSS"] = seh_Boss;
    cast["GUARD"] = seh_Guard;
    cast["MADAM"] = seh_Madam;
    cast["WHORE"] = seh_Whore;
    Game->FixMovieSequenceCamera(true);
    if (skippable) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetCutsceneSkippableWhilePaused(true);
    }
    RunCutsceneMacro(name, cast, 0, &csargs, false, true);
    if (skippable) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetCutsceneSkippableWhilePaused(false);
    }
    Game->FixMovieSequenceCamera(false);
    CutscenePlaying = false;
    csargs.clear();
}

int main()
{
    GameCalls g; CV_BordelloScript s; s.Game = &g; s.CutscenePlaying = false; s.TermNow = false;
    s.seh_Boss.thing = 21; s.seh_Guard.thing = 22; s.seh_Madam.thing = 23; s.seh_Whore.thing = 24; s.csargs.n = 3;
    s.PlayCutscene(CCharString("CS_Bordello1"), true);
    CHECK(g_runs == 1 && std::strcmp(g_runName, "CS_Bordello1") == 0 && g_castHero == 1 && g_castBoss == 21 && g_castWhore == 24 && g_castN == 5);
    CHECK(g_runCam && g_runSkip && g_runArgs == 3 && !g_camFixed && !g_skippable && !s.CutscenePlaying && s.csargs.n == 0);
    s.csargs.n = 2;
    s.PlayCutscene(CCharString("CS_Bordello2"), false);
    CHECK(g_runs == 2 && g_runCam && !g_runSkip && g_runArgs == 2 && !g_camFixed && s.csargs.n == 0);
    // terminating before the skippable switch: nothing runs, the flag stays set
    s.TermNow = true; s.csargs.n = 4;
    s.PlayCutscene(CCharString("CS_Bordello3"), true);
    CHECK(g_runs == 2 && s.CutscenePlaying && s.csargs.n == 4 && g_camFixed);
    if (g_fail) { std::printf("00e3e720_TEST FAIL\n"); return 1; }
    std::printf("00e3e720_TEST PASS\n");
    return 0;
}