#include <cstdio>
#include <cstring>
#include <cstdlib>
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
// bodyguard harness
template <class T> struct vector { T items[4]; unsigned int n; vector() : n(0) {} unsigned int size() const { return n; } T& operator[](unsigned int i) { return items[i]; } };
static char g_ibuf[8][8]; static int g_ni = 0;
static CCharString GFIntToCharString(long v) { char* b = g_ibuf[g_ni++ & 7]; std::sprintf(b, "%ld", v); return CCharString(b); }
static int g_tele[64]; static float g_face[64]; static int g_faceFlag[64];
static bool* g_pFist = 0; static int g_fistFrom = 2, g_fistTo = 5;
struct GameCalls {
    void NewScriptFrame() const { ++g_frame; *g_pFist = g_frame >= g_fistFrom && g_frame < g_fistTo; }
    CScriptThing GetThingWithScriptName(const CCharString& n) const
    { if (std::strncmp(n.p, "ObservePosBG", 12) == 0) return CScriptThing(40 + std::atoi(n.p + 12)); return CScriptThing(0); }
    void EntityTeleportToThing(const CScriptThing& t, const CScriptThing& to, bool b) const { if (!b) g_tele[t.id] = to.id; }
    void EntitySetFacingAngle(const CScriptThing& t, float a, bool b) const { g_face[t.id] = a; g_faceFlag[t.id] = b; }
};
struct CV_BodyGuardOwner { bool InFisticuffs; };
struct CV_BodyGuardScript {
    GameCalls* Game; CV_BodyGuardOwner* Owner; vector<CScriptThing> HenchmanList;
    bool IsActiveThreadTerminating() { return g_frame >= g_termAt; }
    void FisticuffsWatch();
};

void CV_BodyGuardScript::FisticuffsWatch()
{
    while (!IsActiveThreadTerminating()) {
        if (Owner->InFisticuffs) {
            if (IsActiveThreadTerminating())
                return;
            for (unsigned int i = 0; i < HenchmanList.size(); ++i) {
                Game->EntityTeleportToThing(HenchmanList[i], Game->GetThingWithScriptName("ObservePosBG" + GFIntToCharString(i + 1)), false);
                Game->EntitySetFacingAngle(HenchmanList[i], Game->GetThingWithScriptName("ObservePosBG" + GFIntToCharString(i + 1)).GetAngleXY(), true);
            }
            while (Owner->InFisticuffs) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        Game->NewScriptFrame();
    }
}

int main()
{
    GameCalls g; CV_BodyGuardOwner o; o.InFisticuffs = false; g_pFist = &o.InFisticuffs;
    CV_BodyGuardScript s; s.Game = &g; s.Owner = &o;
    s.HenchmanList.items[0] = CScriptThing(11); s.HenchmanList.items[1] = CScriptThing(12); s.HenchmanList.n = 2;
    g_angle[41] = 1.5f; g_angle[42] = 3.0f; g_termAt = 9;
    s.FisticuffsWatch();
    CHECK(g_tele[11] == 41 && g_tele[12] == 42 && g_face[11] == 1.5f && g_face[12] == 3.0f && g_faceFlag[11] == 1 && g_frame == 9);
    // terminating while the fight is on
    g_frame = 0; g_tele[11] = 0; g_fistTo = 100; g_termAt = 4;
    s.FisticuffsWatch();
    CHECK(g_tele[11] == 41 && g_frame == 4);
    if (g_fail) { std::printf("00e5db30_TEST FAIL\n"); return 1; }
    std::printf("00e5db30_TEST PASS\n");
    return 0;
}