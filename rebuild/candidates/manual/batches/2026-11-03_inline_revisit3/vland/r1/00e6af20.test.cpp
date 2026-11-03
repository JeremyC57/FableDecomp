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
// acolyte-hit harness
static bool Hits(int id, int kind, const char* by)
{
    if (std::strcmp(by, "SCRIPT_NAME_HERO") != 0 || id != 6) return false;
    return (kind == 0 && g_frame == 2) || (kind == 1 && g_frame == 4);
}
static int g_conv_n = 0, g_convWith = -1, g_convPerson = -1; static const char* g_convLine = "";
struct GameCalls {
    void NewScriptFrame() const { ++g_frame; }
    CScriptThing GetThingWithScriptName(const CCharString& n) const { return CScriptThing(n.is("M_SoundAltar") ? 8 : 0); }
    CScriptThing& GetHero() { static CScriptThing hero(1); return hero; }
    long AddNewConversation(const CScriptThing& t, bool a, bool b) { ++g_conv_n; g_convWith = (!a && !b) ? t.id : -1; return 5; }
    void AddPersonToConversation(long c, const CScriptThing& t) { if (c == 5) g_convPerson = t.id; }
    void AddLineToConversation(long c, const CCharString& line, bool b, const CScriptThing& from, const CScriptThing& to)
    { if (c == 5 && !b && from.id == 8 && to.id == 1) g_convLine = line.p; }
};
struct CPredicateBase { virtual bool IsTrue() = 0; virtual ~CPredicateBase() {} };
struct CIsThingAlive : CPredicateBase { CScriptThing Thing; CIsThingAlive(const CScriptThing& t) : Thing(t) {} bool IsTrue() { return Thing.IsAlive(); } };
static int g_pred = -1;
struct CV_ChapelOfEvilScript {
    GameCalls* Game; bool AttackedByHero; bool AcolyteAttackMessageComplete;
    void SetPredicate(const CPredicateBase& p) { g_pred = ((const CIsThingAlive&)p).Thing.id; }
    bool IsActiveThreadTerminating() { return g_frame >= g_termAt; }
    void WatchForAcolyteHit(CScriptThing acolyte);
};

void CV_ChapelOfEvilScript::WatchForAcolyteHit(CScriptThing acolyte)
{
    SetPredicate(CIsThingAlive(acolyte));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptThing altar = Game->GetThingWithScriptName("M_SoundAltar");
    AcolyteAttackMessageComplete = false;
    while (!IsActiveThreadTerminating()) {
        bool hit = acolyte.MsgIsHitBy("SCRIPT_NAME_HERO") || acolyte.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO");
        if (hit) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            AttackedByHero = true;
            if (!AcolyteAttackMessageComplete && altar.IsAlive()) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                long conversation = Game->AddNewConversation(altar, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_019_WEAPON_02", false, altar, Game->GetHero());
                AcolyteAttackMessageComplete = true;
            }
        }
        Game->NewScriptFrame();
    }
}

int main()
{
    GameCalls g; CV_ChapelOfEvilScript s; s.Game = &g; s.AttackedByHero = false; s.AcolyteAttackMessageComplete = true;
    g_isHit = Hits; g_health[8] = 5.0f; g_termAt = 8;
    s.WatchForAcolyteHit(CScriptThing(6));
    CHECK(g_pred == 6 && s.AttackedByHero && s.AcolyteAttackMessageComplete && g_conv_n == 1 && g_convWith == 8 && g_convPerson == 1);
    CHECK(std::strcmp(g_convLine, "TEXT_QST_019_WEAPON_02") == 0 && g_frame == 8);
    // the altar is gone: flagged, no warning
    g_frame = 0; g_conv_n = 0; g_health[8] = 0.0f; s.AttackedByHero = false;
    s.WatchForAcolyteHit(CScriptThing(6));
    CHECK(s.AttackedByHero && !s.AcolyteAttackMessageComplete && g_conv_n == 0);
    // never hit
    g_frame = 0; s.AttackedByHero = false;
    s.WatchForAcolyteHit(CScriptThing(7));
    CHECK(!s.AttackedByHero && g_pred == 7);
    if (g_fail) { std::printf("00e6af20_TEST FAIL\n"); return 1; }
    std::printf("00e6af20_TEST PASS\n");
    return 0;
}