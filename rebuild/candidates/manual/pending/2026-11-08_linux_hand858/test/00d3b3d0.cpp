// Behaviour test for 0x00D3B3D0 CQ_GuildTrainingScript::Init: member resets, the timer reset through the interface's SetTimer, then GossipSetup.
#include <cstdio>
struct Iface { long lastIndex, lastValue; int calls; void SetTimer(long i, long v) { lastIndex = i; lastValue = v; ++calls; } };
static Iface g_iface;
struct CTimer { long TimerIndex; CTimer& operator=(long n) { g_iface.SetTimer(TimerIndex, n); return *this; } };
struct GuildTraining {
    long GameState; bool SleepNeeded, ReachedPlatform, DomeCutsceneStart; long HeroWarnings, CurrentBirdsKilled, StartedTesting;
    bool StartedMeleeTesting; CTimer WillHelpTimer; bool FightFinished, DisplayBirdKilledMessage; int gossip;
    void GossipSetup() { ++gossip; }
    void Init()
    {
        GameState = 0; HeroWarnings = 0; DomeCutsceneStart = false; StartedTesting = 0; StartedMeleeTesting = false;
        WillHelpTimer = 0; FightFinished = false; DisplayBirdKilledMessage = false; GossipSetup();
    }
};
int main()
{
    GuildTraining g;
    g.GameState = 5; g.SleepNeeded = true; g.ReachedPlatform = true; g.DomeCutsceneStart = true; g.HeroWarnings = 3;
    g.CurrentBirdsKilled = 7; g.StartedTesting = 2; g.StartedMeleeTesting = true; g.WillHelpTimer.TimerIndex = 42;
    g.FightFinished = true; g.DisplayBirdKilledMessage = true; g.gossip = 0; g_iface.calls = 0; g_iface.lastValue = -1;
    g.Init();
    if (g.GameState || g.HeroWarnings || g.DomeCutsceneStart || g.StartedTesting || g.StartedMeleeTesting || g.FightFinished
        || g.DisplayBirdKilledMessage || !g.SleepNeeded || !g.ReachedPlatform || g.CurrentBirdsKilled != 7
        || g_iface.calls != 1 || g_iface.lastIndex != 42 || g_iface.lastValue != 0 || g.gossip != 1)
    { std::printf("00d3b3d0_TEST FAIL\n"); return 1; }
    std::printf("00d3b3d0_TEST PASS\n");
    return 0;
}
