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
// book-conversation harness
template <class T> struct vector { T items[4]; T& operator[](unsigned int i) { return items[i]; } };
inline bool operator==(const CCharString& a, const CCharString& b) { return std::strcmp(a.p, b.p) == 0; }
static bool g_bScriptAnimationFlag = true;
struct CScriptGameResourceObjectScriptedThingBase {
    int busyUntil; const char* anim; bool animUntilDone; const char* loop; int plays;
    CScriptGameResourceObjectScriptedThingBase() : busyUntil(0), anim(""), animUntilDone(false), loop(""), plays(0) {}
    void PlayAnimation(const CCharString& a, bool b1, bool b2, bool b3, bool waitDone, bool flag, bool b6, bool b7)
    { if (!b1 && b2 && !b3 && flag && !b6 && !b7) { anim = a.p; animUntilDone = waitDone; ++plays; } }
    void PlayLoopingAnimation(const CCharString& a, long n, bool b1, bool b2, bool b3, bool b4, bool flag, bool b6, bool b7)
    { if (n == -1 && !b1 && !b2 && b3 && !b4 && flag && !b6 && !b7) loop = a.p; }
    bool IsPerformingScriptTask() { return g_frame < busyUntil; }
};
struct CConversation { long Lines; vector<CCharString> Speaker, Dialogue, Animation, AnimLoop; };
static int g_convActiveUntil = 3, g_conv_n = 0, g_convSpeaker = -1, g_convPerson = -1, g_removed = -99, g_removedFrame = -1;
static const char* g_convLine = "";
struct GameCalls {
    void NewScriptFrame() const { ++g_frame; }
    CScriptThing GetThingWithScriptName(const CCharString& n) const { return CScriptThing(n.is("boy0") ? 2 : n.is("girl0") ? 3 : 0); }
    CScriptThing& GetHero() { static CScriptThing hero(1); return hero; }
    long AddNewConversation(const CScriptThing& t, bool a, bool b) { ++g_conv_n; g_convSpeaker = (!a && !b) ? t.id : -1; return 7; }
    void AddPersonToConversation(long c, const CScriptThing& t) { if (c == 7) g_convPerson = t.id; }
    void AddLineToConversation(long c, const CCharString& line, bool b, const CScriptThing& from, const CScriptThing& to)
    { if (c == 7 && !b && from.id == g_convSpeaker && to.id == 1) g_convLine = line.p; }
    bool IsConversationActive(long c) const { return c == 7 && g_frame < g_convActiveUntil; }
    void RemoveConversation(long c, bool b) { if (!b) { g_removed = (int)c; g_removedFrame = g_frame; } }
};
struct CV_BookCollectingScript {
    GameCalls* Game; bool ReadingBook; vector<CConversation> BookReactions;
    CScriptGameResourceObjectScriptedThingBase seh_Boy, seh_Girl;
    bool IsActiveThreadTerminating() { return g_frame >= g_termAt; }
    void DoConversation(long book, long line);
};

void CV_BookCollectingScript::DoConversation(long book, long line)
{
    CCharString speaker = BookReactions[book].Speaker[line];
    CCharString dialogue = BookReactions[book].Dialogue[line];
    CCharString animation = BookReactions[book].Animation[line];
    CCharString animLoop = BookReactions[book].AnimLoop[line];
    CScriptThing speakerThing = Game->GetThingWithScriptName(speaker);
    long conversation = -1;
    if (dialogue != "NULL") {
        if (IsActiveThreadTerminating())
            return;
        conversation = Game->AddNewConversation(speakerThing, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        Game->AddLineToConversation(conversation, dialogue, false, speakerThing, Game->GetHero());
    }
    CScriptGameResourceObjectScriptedThingBase* handle = 0;
    if (speaker == "boy0") {
        if (IsActiveThreadTerminating())
            return;
        handle = &seh_Boy;
    } else if (speaker == "girl0") {
        if (IsActiveThreadTerminating())
            return;
        handle = &seh_Girl;
    }
    if (handle) {
        if (IsActiveThreadTerminating())
            return;
        if (animLoop == "NULL") {
            if (IsActiveThreadTerminating())
                return;
            handle->PlayAnimation(animation, false, true, false, true, g_bScriptAnimationFlag, false, false);
        } else {
            if (IsActiveThreadTerminating())
                return;
            handle->PlayAnimation(animation, false, true, false, false, g_bScriptAnimationFlag, false, false);
            handle->PlayLoopingAnimation(animLoop, -1, false, false, true, false, g_bScriptAnimationFlag, false, false);
        }
    }
    while ((Game->IsConversationActive(conversation) || handle->IsPerformingScriptTask()) && !ReadingBook) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveConversation(conversation, false);
}

int main()
{
    GameCalls g; CV_BookCollectingScript s; s.Game = &g; s.ReadingBook = false;
    CConversation& c = s.BookReactions[2];
    c.Speaker[0] = "boy0";  c.Dialogue[0] = "TXT_BOY_LINE"; c.Animation[0] = "Wave"; c.AnimLoop[0] = "NULL";
    c.Speaker[1] = "girl0"; c.Dialogue[1] = "NULL";         c.Animation[1] = "Sit";  c.AnimLoop[1] = "SitLoop";
    s.seh_Boy.busyUntil = 5;
    s.DoConversation(2, 0);
    CHECK(g_conv_n == 1 && g_convSpeaker == 2 && g_convPerson == 1 && std::strcmp(g_convLine, "TXT_BOY_LINE") == 0);
    CHECK(s.seh_Boy.plays == 1 && std::strcmp(s.seh_Boy.anim, "Wave") == 0 && s.seh_Boy.animUntilDone && s.seh_Boy.loop[0] == 0);
    CHECK(g_removed == 7 && g_removedFrame == 5 && s.seh_Girl.plays == 0);
    s.seh_Girl.busyUntil = 7;
    s.DoConversation(2, 1);
    CHECK(g_conv_n == 1 && s.seh_Girl.plays == 1 && !s.seh_Girl.animUntilDone && std::strcmp(s.seh_Girl.loop, "SitLoop") == 0);
    CHECK(g_removed == -1 && g_removedFrame == 7);
    // a book being read cuts the wait short
    s.ReadingBook = true; s.seh_Girl.busyUntil = 100;
    s.DoConversation(2, 1);
    CHECK(g_removedFrame == 7 && s.seh_Girl.plays == 2);
    if (g_fail) { std::printf("00e569d0_TEST FAIL\n"); return 1; }
    std::printf("00e569d0_TEST PASS\n");
    return 0;
}
