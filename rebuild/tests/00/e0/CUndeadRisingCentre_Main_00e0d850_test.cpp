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
struct CScriptThing { int id; };
static int g_near = 15;
bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float d) { return a.id == 1 && b.id == 2 && d == 3.0f && g_frame >= g_near; }
struct GameCalls {
    CScriptThing hero;
    CScriptThing& GetHero() { return hero; }
    void NewScriptFrame() { ++g_frame; }
    long AddNewConversation(const CScriptThing& t, bool, bool) const { return t.id * 10; }
    void AddPersonToConversation(long c, const CScriptThing& t) const { if (c != 20 || t.id != 1) g_fail = 1; }
    void AddLineToConversation(long c, const CCharString& line, bool, const CScriptThing& sp, const CScriptThing& to) const
    {
        if (c != 20 || sp.id != 2 || to.id != 1) g_fail = 1;
        LOG(line.p);
    }
    void MiniMapRemoveMarker(const CScriptThing& t) { if (t.id == 2) LOG("REMOVE"); }
};
struct CQ_UndeadRisingScript { bool HeroReachedCentre; };
struct CUndeadRisingCentre {
    GameCalls* Game;
    CScriptThing Me;
    CQ_UndeadRisingScript* ParentClass;
    bool IsActiveThreadTerminating() { return g_frame >= g_termAt; }
    void Main();
};

void CUndeadRisingCentre::Main()
{
    CTimer timer;
    timer.SetTime(0);
    long line = 0;
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 3.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        long conversation = Game->AddNewConversation(Me, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        if (timer.GetTime() <= 0) {
            if (IsActiveThreadTerminating()) return;
            switch (line) {
            case 0:
                Game->AddLineToConversation(conversation, "TEXT_QST_049_VOICE_LIGHT_FIRST", false, Me, Game->GetHero());
                break;
            case 1:
                Game->AddLineToConversation(conversation, "TEXT_QST_049_VOICE_LIGHT_SECOND", false, Me, Game->GetHero());
                break;
            default:
                Game->AddLineToConversation(conversation, "TEXT_QST_049_VOICE_LIGHT_THIRD", false, Me, Game->GetHero());
                break;
            }
            ++line;
            timer.SetTime(10);
        }
    }
    if (IsActiveThreadTerminating()) return;
    ParentClass->HeroReachedCentre = true;
    Game->MiniMapRemoveMarker(Me);
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}

int main()
{
    GameCalls g; g.hero.id = 1; CQ_UndeadRisingScript q; q.HeroReachedCentre = false;
    CUndeadRisingCentre c; c.Game = &g; c.Me.id = 2; c.ParentClass = &q;
    g_near = 25; g_termAt = 40;
    c.Main();
    // lines at frames 1, 11, 21 (every 10 s); reached at 25
    CHECK(g_log_n == 4 && std::strcmp(g_log[0], "TEXT_QST_049_VOICE_LIGHT_FIRST") == 0);
    CHECK(std::strcmp(g_log[1], "TEXT_QST_049_VOICE_LIGHT_SECOND") == 0 && std::strcmp(g_log[2], "TEXT_QST_049_VOICE_LIGHT_THIRD") == 0);
    CHECK(std::strcmp(g_log[3], "REMOVE") == 0 && q.HeroReachedCentre && g_frame == 40);
    g_frame = 0; g_log_n = 0; q.HeroReachedCentre = false; g_near = 100; g_termAt = 5;
    c.Main();
    CHECK(!q.HeroReachedCentre && g_frame == 5 && g_log_n == 1);
    if (g_fail) { std::printf("00e0d850_TEST FAIL\n"); return 1; }
    std::printf("00e0d850_TEST PASS\n");
    return 0;
}