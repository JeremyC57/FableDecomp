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
struct Str { char t[16]; Str& operator=(const char* s) { std::strcpy(t, s); return *this; } };
struct Script {
    int DonationTotal, DonationThisVisit, DonationCounter; bool Flags[12]; float MoralityInitial, MoralityCurrent, MoralityCumulative;
    bool QuestFinished, QuestComplete, QuestActivated; int TortureCount; bool VictimTortured; int VictimHealth; int VictimGender;
    bool Victim[5]; Str TortureSound; int TortureType;
    void Init()
    {
        DonationTotal = DonationThisVisit = DonationCounter = 0;
        for (int i = 0; i < 12; ++i) Flags[i] = false;
        MoralityInitial = MoralityCurrent = MoralityCumulative = 0.0f;
        QuestFinished = QuestActivated = QuestComplete = false;
        TortureCount = 1; VictimTortured = false; VictimHealth = 0;
        for (int j = 0; j < 5; ++j) Victim[j] = false;
        TortureType = 0;
        TortureSound = "";
    }
};
int main()
{
    bool ok = true;
    Script s; std::memset(&s, 1, sizeof(s)); s.VictimGender = 2;
    s.Init();
    ok = s.DonationTotal == 0 && !s.Flags[0] && !s.Flags[11] && s.MoralityCumulative == 0.0f && !s.QuestComplete && s.TortureCount == 1;
    ok = ok && s.VictimGender == 2 && !s.Victim[4] && s.TortureType == 0 && s.TortureSound.t[0] == 0;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00e68fa0_TEST FAIL\n"); return 1; }
    std::printf("00e68fa0_TEST PASS\n");
    return 0;
}