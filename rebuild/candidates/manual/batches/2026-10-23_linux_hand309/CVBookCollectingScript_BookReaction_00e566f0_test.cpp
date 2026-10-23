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
// book-reaction harness
template <class T> struct vector { T items[4]; T& operator[](unsigned int i) { return items[i]; } };
struct CScriptGameResourceObjectScriptedThingBase {
    int thing, clears, cmdClears; bool released;
    CScriptGameResourceObjectScriptedThingBase() : thing(0), clears(0), cmdClears(0), released(false) {}
    void ClearAllActionsIncludingLoopingAnimations() { ++clears; }
    void ClearCommands() { ++cmdClears; }
    void Release() { released = true; }
};
struct CConversation { long Lines; };
struct CScriptDef { vector<CCharString> BookBoyMarks; vector<CCharString> BookGirlMarks; };
static CScriptDef g_def; static CScriptDef* g_pScriptDef = &g_def;
static int g_tele[64]; static int g_push[64];
struct GameCalls {
    void NewScriptFrame() const { ++g_frame; }
    CScriptThing GetThingWithScriptName(const CCharString& n) const
    {
        if (n.is("boy0")) return CScriptThing(2);
        if (n.is("girl0")) return CScriptThing(3);
        if (n.is("BoyMark1")) return CScriptThing(30);
        if (n.is("GirlMark1")) return CScriptThing(31);
        return CScriptThing(0);
    }
    void StartScriptingEntity(const CScriptThing& t, CScriptGameResourceObjectScriptedThingBase& s, long m) const { if (m == 4) s.thing = t.id; }
    void EntityTeleportToThing(const CScriptThing& t, const CScriptThing& to, bool b) const { if (!b) g_tele[t.id] = to.id; }
    void SetIsPushableByHero(const CScriptThing& t, bool b) const { g_push[t.id] = b ? 1 : 2; }
};
static int g_lines_n = 0, g_lineBook[16], g_lineNo[16];
struct CV_BookCollectingScript {
    GameCalls* Game; bool ReadingBook; vector<CConversation> BookReactions;
    CScriptGameResourceObjectScriptedThingBase seh_Boy, seh_Girl; int termCalls;
    bool IsActiveThreadTerminating() { ++termCalls; return g_frame >= g_termAt; }
    void DoConversation(long book, long line) { g_lineBook[g_lines_n] = (int)book; g_lineNo[g_lines_n++] = (int)line; if (line == 2) ReadingBook = true; }
    void BookReaction(long book);
};

void CV_BookCollectingScript::BookReaction(long book)
{
    long lines = BookReactions[book].Lines;
    if (lines == 0) {
        IsActiveThreadTerminating();
        return;
    }
    CScriptThing boy = Game->GetThingWithScriptName("boy0");
    CScriptThing girl = Game->GetThingWithScriptName("girl0");
    CScriptThing boyMark = Game->GetThingWithScriptName(g_pScriptDef->BookBoyMarks[book]);
    CScriptThing girlMark = Game->GetThingWithScriptName(g_pScriptDef->BookGirlMarks[book]);
    Game->StartScriptingEntity(boy, seh_Boy, 4);
    Game->EntityTeleportToThing(boy, boyMark, false);
    Game->SetIsPushableByHero(boy, false);
    seh_Boy.ClearAllActionsIncludingLoopingAnimations();
    seh_Boy.ClearCommands();
    Game->StartScriptingEntity(girl, seh_Girl, 4);
    Game->EntityTeleportToThing(girl, girlMark, false);
    Game->SetIsPushableByHero(girl, false);
    seh_Girl.ClearAllActionsIncludingLoopingAnimations();
    seh_Girl.ClearCommands();
    for (long i = 0; !ReadingBook && i < lines; ++i) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        DoConversation(book, i);
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetIsPushableByHero(boy, true);
    Game->SetIsPushableByHero(girl, true);
    seh_Boy.ClearAllActionsIncludingLoopingAnimations();
    seh_Boy.ClearCommands();
    seh_Boy.Release();
    seh_Girl.ClearAllActionsIncludingLoopingAnimations();
    seh_Girl.ClearCommands();
    seh_Girl.Release();
}

int main()
{
    GameCalls g; CV_BookCollectingScript s; s.Game = &g; s.ReadingBook = false; s.termCalls = 0;
    s.BookReactions[0].Lines = 0; s.BookReactions[1].Lines = 5;
    g_def.BookBoyMarks[1] = CCharString("BoyMark1"); g_def.BookGirlMarks[1] = CCharString("GirlMark1");
    s.BookReaction(0);
    CHECK(s.termCalls == 1 && g_lines_n == 0 && s.seh_Boy.thing == 0);
    s.BookReaction(1);   // lines 0,1,2 then a book is being read
    CHECK(g_lines_n == 3 && g_lineBook[2] == 1 && g_lineNo[2] == 2 && g_frame == 3);
    CHECK(s.seh_Boy.thing == 2 && s.seh_Girl.thing == 3 && g_tele[2] == 30 && g_tele[3] == 31 && g_push[2] == 1 && g_push[3] == 1);
    CHECK(s.seh_Boy.clears == 2 && s.seh_Boy.cmdClears == 2 && s.seh_Boy.released && s.seh_Girl.clears == 2 && s.seh_Girl.released);
    // terminated mid-conversation: the pair stays scripted and unpushable
    CV_BookCollectingScript t; t.Game = &g; t.ReadingBook = false; t.termCalls = 0; t.BookReactions[1].Lines = 5;
    g_frame = 0; g_termAt = 2; g_lines_n = 0; g_push[2] = 0;
    t.BookReaction(1);
    CHECK(g_lines_n == 1 && g_push[2] == 2 && !t.seh_Boy.released && t.seh_Boy.clears == 1);
    if (g_fail) { std::printf("00e566f0_TEST FAIL\n"); return 1; }
    std::printf("00e566f0_TEST PASS\n");
    return 0;
}
