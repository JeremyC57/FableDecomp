// Behaviour test for CTCCombat::UpdateCombatTimers (0x007339E0): grace countdowns, idle timer, ability timers, block counters.
#include <cstdio>
struct Ability { long Timer, Cooldown; float Seconds; };
static long g_idle = 40, g_fps = 30;
struct Combat {
    long ComboChainPos, ComboChainID, ComboGrace, HandGrace, Idle, BlockCounter, BlockHold, BlockFrames;
    bool PerformingAction, PerformingStrike, Blocking;
    bool nowPerforming, nowStrike; long handedness; int expired;
    Ability ab[2];
    void Update()
    {
        bool performing = nowPerforming;
        bool strike = performing ? nowStrike : false;
        bool wasPerforming = PerformingAction, wasStrike = PerformingStrike;
        if (!strike) {
            if (wasStrike) ComboGrace = HandGrace = 3;
            if (ComboGrace) --ComboGrace;
            else if (ComboChainPos != -777 || ComboChainID != -999) { ComboChainPos = -777; ComboChainID = -999; }
            if (HandGrace) --HandGrace;
            else if (handedness) handedness = 0;
        }
        PerformingAction = performing; PerformingStrike = strike;
        if (wasPerforming && wasStrike) Idle = g_idle; else if (Idle) --Idle;
        if (BlockCounter >= 0) --BlockCounter;
        for (int i = 0; i < 2; ++i) {
            if (ab[i].Timer >= 0) --ab[i].Timer;
            if (ab[i].Cooldown >= 0 && --ab[i].Cooldown == -1) ab[i].Timer = (long)(g_fps * ab[i].Seconds);
        }
        if (BlockHold == 0) ++expired;
        if (BlockHold >= 0) --BlockHold;
        if (Blocking) ++BlockFrames; else if (BlockFrames > 0) BlockFrames = 0;
    }
};
int main()
{
    Combat c = { 5, 6, 0, 0, 0, 2, 0, 4, true, true, false, false, false, 1, 0, { { 3, 0, 2.0f }, { -1, -1, 1.0f } } };
    c.Update();   // strike just ended: graces reset to 3 then counted, idle armed
    if (c.ComboGrace != 2 || c.HandGrace != 2 || c.Idle != 40 || c.BlockCounter != 1 || c.ab[0].Timer != 60 || c.ab[1].Timer != -1
        || c.expired != 1 || c.BlockHold != -1 || c.BlockFrames != 0 || c.PerformingAction) { std::printf("007339e0_TEST FAIL\n"); return 1; }
    c.ComboGrace = 0; c.HandGrace = 0; c.Update();
    if (c.ComboChainPos != -777 || c.ComboChainID != -999 || c.handedness != 0 || c.Idle != 39) { std::printf("007339e0_TEST FAIL\n"); return 1; }
    std::printf("007339e0_TEST PASS\n");
    return 0;
}