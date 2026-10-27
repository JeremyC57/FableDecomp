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
struct Ctx {
    void Transfer(const char* n, bool& v) { LogI(n, v); v = !v; }
    void Transfer(const char* n, unsigned long& v) { LogI(n, (int)v); v += 1; }
    void TransferVector(const char* n, int& v) { LogI(n, v); }
};
struct Bard { bool bHurtBard, bBard_finished; unsigned long lNumber_of_things_sung_about; bool bRewardGiven;
    bool BardIntroComplete, AttackedByHero, BardFollowing; int SongThingAvailable;
    void OnPersist(Ctx& context)
    {
        context.Transfer("bBard_finished", bBard_finished);
        context.Transfer("bRewardGiven", bRewardGiven);
        context.Transfer("bHurtBard", bHurtBard);
        context.Transfer("lNumber_of_things_sung_about", lNumber_of_things_sung_about);
        context.Transfer("AttackedByHero", AttackedByHero);
        context.Transfer("BardIntroComplete", BardIntroComplete);
        context.Transfer("BardFollowing", BardFollowing);
        context.TransferVector("SongThingAvailable", SongThingAvailable);
    }
};
int main()
{
    bool ok = true;
    Ctx c; Bard b; b.bHurtBard = true; b.bBard_finished = false; b.lNumber_of_things_sung_about = 7; b.bRewardGiven = true;
    b.BardIntroComplete = false; b.AttackedByHero = true; b.BardFollowing = false; b.SongThingAvailable = 3;
    Reset(0); b.OnPersist(c);
    ok = ok && std::strcmp(g_log, "bBard_finished0;bRewardGiven1;bHurtBard1;lNumber_of_things_sung_about7;AttackedByHero1;BardIntroComplete0;BardFollowing0;SongThingAvailable3;") == 0;
    ok = ok && b.lNumber_of_things_sung_about == 8 && b.bBard_finished && !b.bHurtBard;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00ed85a0_TEST FAIL\n"); return 1; }
    std::printf("00ed85a0_TEST PASS\n");
    return 0;
}