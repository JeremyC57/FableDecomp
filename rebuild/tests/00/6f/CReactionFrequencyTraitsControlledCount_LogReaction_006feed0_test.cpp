// Behaviour test for CReactionFrequencyTraits_ControlledCount::LogReaction @ 0x006FEED0.
#include <cstdio>

static long g_frame = 0;
long GFGetCurrentFrame() { return g_frame; }

class CThingAICreature { public: int m_id; };

struct CReactionFrequencyTraits_ControlledCount {
    bool AllowIndividualRepeats; CThingAICreature* PLastReactionCreature; bool InRisingPhase;
    unsigned long MinGapFrames; float CountRechargePerFrame, CurrentAvailableCount, MaxCount, InvMaxCount;
    unsigned long FrameLastStarted, FrameLastRecharged;
    void LogReaction(CThingAICreature* creature, bool started)
    {
        if (started) {
            unsigned long frame = GFGetCurrentFrame();
            PLastReactionCreature = creature;
            FrameLastStarted = frame;
            CurrentAvailableCount -= 1.0f;
            CurrentAvailableCount = CurrentAvailableCount > MaxCount ? MaxCount : CurrentAvailableCount;
            if (CurrentAvailableCount <= 0.0f) {
                InRisingPhase = true;
                FrameLastRecharged = frame;
            }
        }
    }
};

int main()
{
    CThingAICreature a, b;
    CReactionFrequencyTraits_ControlledCount t;
    t.PLastReactionCreature = 0; t.InRisingPhase = false; t.CurrentAvailableCount = 2.0f; t.MaxCount = 3.0f;
    t.FrameLastStarted = 0; t.FrameLastRecharged = 0;
    g_frame = 100;
    t.LogReaction(&a, false);                       // not started: nothing changes
    if (t.PLastReactionCreature != 0 || t.CurrentAvailableCount != 2.0f) { std::printf("006feed0_TEST FAIL\n"); return 1; }
    t.LogReaction(&a, true);                        // 2 -> 1, still positive
    if (t.PLastReactionCreature != &a || t.FrameLastStarted != 100 || t.CurrentAvailableCount != 1.0f || t.InRisingPhase) { std::printf("006feed0_TEST FAIL\n"); return 1; }
    g_frame = 140;
    t.LogReaction(&b, true);                        // 1 -> 0: empty, recharge phase starts
    if (t.PLastReactionCreature != &b || t.CurrentAvailableCount != 0.0f || !t.InRisingPhase || t.FrameLastRecharged != 140) { std::printf("006feed0_TEST FAIL\n"); return 1; }
    t.CurrentAvailableCount = 10.0f; t.InRisingPhase = false;
    t.LogReaction(&a, true);                        // 10 -> 9, clamped to the max of 3
    if (t.CurrentAvailableCount != 3.0f || t.InRisingPhase) { std::printf("006feed0_TEST FAIL\n"); return 1; }
    std::printf("006feed0_TEST PASS\n");
    return 0;
}