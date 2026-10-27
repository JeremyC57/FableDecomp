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
struct Str { char t[16]; };
struct Vec { int n; };
struct Ctx {
    void Transfer(const char* name, int& v, const int& d) { LogI(name, v * 10 + d); }
    void Transfer(const char* name, bool& v, const bool& d) { LogI(name, v * 10 + d); }
    void Transfer(const char* name, Str& v, const Str& d) { LogS(name, v.t); }
    void TransferVector(const char* name, Vec& v) { LogI(name, v.n); }
    void Transfer(const char* name, int& v) { Transfer(name, v, 0); }
    void Transfer(const char* name, bool& v) { Transfer(name, v, false); }
    void Transfer(const char* name, Str& v) { Str d; d.t[0] = 0; Transfer(name, v, d); }
};
struct Script { int AssassinObjectCount; bool Activated; Str RegionOfMostRecentKill; Vec RegionKillCounts; bool QuestComplete, RewardsGiven, GivenQuestCard;
    void OnPersist(Ctx& context)
    {
        context.Transfer("AssassinObjectCount", AssassinObjectCount);
        context.Transfer("Activated", Activated);
        context.Transfer("QuestComplete", QuestComplete);
        context.Transfer("RegionOfMostRecentKill", RegionOfMostRecentKill);
        context.Transfer("RewardsGiven", RewardsGiven);
        context.Transfer("GivenQuestCard", GivenQuestCard);
        context.TransferVector("RegionKillCounts", RegionKillCounts);
    }
};
int main()
{
    bool ok = true;
    Ctx c; Script s; s.AssassinObjectCount = 3; s.Activated = true; std::strcpy(s.RegionOfMostRecentKill.t, "Bower");
    s.RegionKillCounts.n = 5; s.QuestComplete = false; s.RewardsGiven = true; s.GivenQuestCard = false;
    Reset(1000); s.OnPersist(c);
    ok = std::strcmp(g_log, "AssassinObjectCount30;Activated10;QuestComplete0;RegionOfMostRecentKill Bower;RewardsGiven10;GivenQuestCard0;RegionKillCounts5;") == 0;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00e39780_TEST FAIL\n"); return 1; }
    std::printf("00e39780_TEST PASS\n");
    return 0;
}