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
struct Str { char t[40]; Str& operator=(const char* s) { std::strcpy(t, s); return *this; } };
struct Script {
    bool Flags1[20]; float TricksterApproachProximity; bool Flags2[17]; int HitCount; int BuddyCounter; int Timer; Str RandomSpeech[5];
    void Init()
    {
        for (int i = 0; i < 20; ++i) Flags1[i] = false;
        TricksterApproachProximity = 0.0f;
        for (int j = 0; j < 17; ++j) if (j != 2 && j != 3) Flags2[j] = false;   // ExpressionDetectedFollow/Wait are kept
        HitCount = 0;
        BuddyCounter = 1;
        RandomSpeech[0] = "TEXT_QST_002_RANDOM_00";
        RandomSpeech[1] = "TEXT_QST_002_RANDOM_10";
        RandomSpeech[2] = "TEXT_QST_002_RANDOM_20";
        RandomSpeech[3] = "TEXT_QST_002_RANDOM_30";
        RandomSpeech[4] = "TEXT_QST_002_RANDOM_40";
    }
};
int main()
{
    bool ok = true;
    Script s; std::memset(&s, 1, sizeof(s)); s.Timer = 9;
    s.Init();
    for (int i = 0; i < 20; ++i) ok = ok && !s.Flags1[i];
    for (int j = 0; j < 17; ++j) ok = ok && (s.Flags2[j] == (j == 2 || j == 3));
    ok = ok && s.TricksterApproachProximity == 0.0f && s.HitCount == 0 && s.BuddyCounter == 1 && s.Timer == 9;
    ok = ok && std::strcmp(s.RandomSpeech[0].t, "TEXT_QST_002_RANDOM_00") == 0 && std::strcmp(s.RandomSpeech[4].t, "TEXT_QST_002_RANDOM_40") == 0;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00e2abc0_TEST FAIL\n"); return 1; }
    std::printf("00e2abc0_TEST PASS\n");
    return 0;
}