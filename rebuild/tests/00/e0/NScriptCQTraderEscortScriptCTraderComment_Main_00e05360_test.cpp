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
// trader-comment harness
template <class T> struct vector {
    T items[8]; unsigned int n;
    vector() : n(0) {}
    void push_back(const T& t) { items[n++] = t; }
    unsigned int size() const { return n; }
    T& operator[](unsigned int i) { return items[i]; }
};
static float g_dist[64];
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float d) { return g_dist[b.id] < d; }
static int g_removed = -1, g_removedFrame = -1;
struct GameCalls {
    void NewScriptFrame() const { ++g_frame; if (g_frame == 3) g_dist[12] = 1.0f; }
    long GetAllThingsWithScriptName(const CCharString& name, vector<CScriptThing>& things) const
    {
        if (name.is("DarkwoodTrader")) { things.push_back(CScriptThing(11)); things.push_back(CScriptThing(12)); things.push_back(CScriptThing(13)); }
        return (long)things.size();
    }
    void RemoveThing(const CScriptThing& t, bool a, bool b) const { if (!a && b) { g_removed = t.id; g_removedFrame = g_frame; } }
};
struct CScriptDef { long TE_TraderCommentDistance; };
static CScriptDef g_def = { 5 };
static CScriptDef* g_pScriptDef = &g_def;
struct CPredicateBase { virtual bool IsTrue() = 0; virtual ~CPredicateBase() {} };
struct CIsThingAlive : CPredicateBase {
    CScriptThing Thing;
    CIsThingAlive(const CScriptThing& t) : Thing(t) {}
    bool IsTrue() { return Thing.IsAlive(); }
};
static int g_pred = -1;
static int g_comments = 0, g_commentTrader = -1;
static bool g_accept = true;
struct CQ_TraderEscortScript {
    enum ETraderComment { TRADER_COMMENT_0, TRADER_COMMENT_1 };
    bool MakeTraderComment(const CCharString& text, const CScriptThing& trader, ETraderComment c)
    {
        ++g_comments; g_commentTrader = trader.id;
        return g_accept && text.is("TraderLine") && c == TRADER_COMMENT_1;
    }
    struct CTraderComment;
};
struct CQ_TraderEscortScript::CTraderComment {
    GameCalls* Game; CScriptThing Me; CQ_TraderEscortScript* ParentClass;
    void SetPredicate(const CPredicateBase& p) { g_pred = ((CIsThingAlive&)p).Thing.id; }
    bool IsActiveThreadTerminating() { return g_frame >= g_termAt; }
    void Main();
};

void CQ_TraderEscortScript::CTraderComment::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    vector<CScriptThing> traders;
    Game->GetAllThingsWithScriptName("DarkwoodTrader", traders);
    long distance = g_pScriptDef->TE_TraderCommentDistance;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        for (unsigned int i = 0; i < traders.size(); ++i) {
            if (IsActiveThreadTerminating())
                return;
            if (traders[i].IsAlive() && IsDistanceBetweenThingsUnder(Me, traders[i], (float)distance)) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                if (ParentClass->MakeTraderComment(Me.GetDataString(), traders[i], TRADER_COMMENT_1)) {
#pragma inline_depth(0)
                    if (IsActiveThreadTerminating())
                        return;
                    Game->RemoveThing(Me, false, true);
                    return;
#pragma inline_depth()
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
    GameCalls g; CQ_TraderEscortScript p; CQ_TraderEscortScript::CTraderComment c;
    c.Game = &g; c.Me = CScriptThing(2); c.ParentClass = &p; g_data[2] = "TraderLine";
    g_health[11] = 0.0f; g_dist[11] = 1.0f;   // dead but close: ignored
    g_health[12] = 9.0f; g_dist[12] = 50.0f;  // alive, comes close at frame 3
    g_health[13] = 9.0f; g_dist[13] = 50.0f;
    c.Main();
    CHECK(g_pred == 2 && g_comments == 1 && g_commentTrader == 12 && g_removed == 2 && g_removedFrame == 3);
    // comment declined: keeps looping until terminate, never removes
    g_frame = 0; g_removed = -1; g_comments = 0; g_accept = false; g_termAt = 6; g_dist[12] = 50.0f;
    c.Main();
    CHECK(g_removed == -1 && g_comments == 3 && g_frame == 6);
    if (g_fail) { std::printf("00e05360_TEST FAIL\n"); return 1; }
    std::printf("00e05360_TEST PASS\n");
    return 0;
}