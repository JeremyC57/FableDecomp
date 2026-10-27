#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (calls are recorded, not executed).
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_str = "";
struct Script {
    bool MissionSucceeded;
    bool MissionFailed;
    bool MissionOver;
    bool GotToExecutionTree;
    bool DuelStarted;
    bool ThunderDefeated;
    bool OkToShowHealthBar;
    bool ThunderEnteredCaveFirst;
    bool OkToCheer;
    bool SupportThunder;
    bool StopSupportersCheering;
    bool SaidSurprisedLine;
    int TimesHitSpectators;
    unsigned int MyHealth;
    int Tail;

    void Init()
    {
        MissionSucceeded = false; MissionFailed = false; MissionOver = false; GotToExecutionTree = false; DuelStarted = false; ThunderDefeated = false; OkToShowHealthBar = true; ThunderEnteredCaveFirst = false; OkToCheer = false; SupportThunder = false; StopSupportersCheering = false; SaidSurprisedLine = false;
        TimesHitSpectators = 0; MyHealth = 0;
    }
};

int main()
{
    g_log[0] = 0;
    bool ok = true;
    Script s;
    std::memset(&s, 0x5a, sizeof(s));
    s.Tail = 77;
    s.Init();
    ok = (!s.MissionSucceeded && !s.MissionFailed && !s.MissionOver && !s.GotToExecutionTree && !s.DuelStarted && !s.ThunderDefeated && s.OkToShowHealthBar && !s.ThunderEnteredCaveFirst && !s.OkToCheer && !s.SupportThunder && !s.StopSupportersCheering && !s.SaidSurprisedLine && s.TimesHitSpectators == 0 && s.MyHealth == 0) && s.Tail == 77;

    if (!ok) { std::printf("00e986d0_TEST FAIL\n"); return 1; }
    std::printf("00e986d0_TEST PASS\n");
    return 0;
}