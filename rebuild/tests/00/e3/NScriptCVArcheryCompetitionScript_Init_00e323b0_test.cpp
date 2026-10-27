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
struct Game { void SetTimer(int timer, int time) { LogI("timer", timer * 100 + time); } };
static Game g_game;
class CTimer { public: int TimerIndex; CTimer& operator=(int t) { g_game.SetTimer(TimerIndex, t); return *this; } };
struct Script {
    int Score; bool Flags[8]; int DummyCounter; int Ids[3]; bool Flags2[7]; CTimer GameTimeLeft; CTimer RandomSpeechTimer;
    Str RandomSpeech[10]; int ScorePowerLevel; int ContestState;
    void Init()
    {
        ContestState = 0;
        for (int i = 0; i < 8; ++i) Flags[i] = false;
        for (int j = 0; j < 7; ++j) Flags2[j] = false;
        DummyCounter = 0;
        GameTimeLeft = 0;
        for (int k = 0; k < 10; ++k) { char b[40]; std::sprintf(b, "TEXT_QST_004_RANDOM_%d0", k); RandomSpeech[k] = b; }
    }
};
int main()
{
    bool ok = true;
    Script s; std::memset(&s, 1, sizeof(s)); s.GameTimeLeft.TimerIndex = 3; s.Score = 5;
    Reset(1000); s.Init();
    ok = s.ContestState == 0 && s.DummyCounter == 0 && s.Score == 5 && !s.Flags[0] && !s.Flags[7] && !s.Flags2[6];
    ok = ok && std::strcmp(g_log, "timer300;") == 0 && std::strcmp(s.RandomSpeech[9].t, "TEXT_QST_004_RANDOM_90") == 0;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00e323b0_TEST FAIL\n"); return 1; }
    std::printf("00e323b0_TEST PASS\n");
    return 0;
}