#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (interface calls are logged, not executed).
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static void LogI(const char* s, int v) { char b[128]; std::sprintf(b, "%s%d", s, v); Log(b); }
static void LogS(const char* s, const char* v) { char b[256]; std::sprintf(b, "%s %s", s, v); Log(b); }
static int g_termAfter = 1000;   // IsActiveThreadTerminating() turns true after this many calls
static int g_termCalls = 0;
static bool Term() { return ++g_termCalls > g_termAfter; }
static void Reset(int termAfter) { g_log[0] = 0; g_termCalls = 0; g_termAfter = termAfter; }
struct Thing { int Id; };
struct Str { char t[16]; Str& operator=(const char* s) { std::strcpy(t, s); return *this; } };
struct Game {
    void SetIsPushableByHero(Thing, bool p) { LogI("push", p); }
    void EntitySetInFaction(const Thing&, const char* f) { LogS("faction", f); }
};
struct Tyler { Game* G; Thing Me; bool AnsweredYes, ShownItem, IntroducedPunchClub; int Money; Str PunchClubLeftReason; bool OutstandingCrimes;
    void Init()
    {
        AnsweredYes = false;
        ShownItem = false;
        G->SetIsPushableByHero(Me, false);
        G->EntitySetInFaction(Me, "FACTION_NEUTRAL");
        IntroducedPunchClub = false;
        Money = 0;
        PunchClubLeftReason = "NONE";
        OutstandingCrimes = false;
    }
};
int main()
{
    bool ok = true;
    Game g; Tyler t; std::memset(&t, 1, sizeof(t)); t.G = &g;
    Reset(1000); t.Init();
    ok = std::strcmp(g_log, "push0;faction FACTION_NEUTRAL;") == 0 && !t.AnsweredYes && !t.ShownItem && !t.IntroducedPunchClub && t.Money == 0;
    ok = ok && std::strcmp(t.PunchClubLeftReason.t, "NONE") == 0 && !t.OutstandingCrimes;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00e836f0_TEST FAIL\n"); return 1; }
    std::printf("00e836f0_TEST PASS\n");
    return 0;
}