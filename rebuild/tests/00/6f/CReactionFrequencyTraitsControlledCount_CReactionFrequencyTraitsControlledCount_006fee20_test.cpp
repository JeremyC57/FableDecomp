// Behaviour test for 0x006FEE20: ControlledCount ctor fills the pool, min gap frames, recharge rate and inverse count.
#include <cstdio>
static int g_framesPerSecond = 30;
struct M { bool AllowIndividualRepeats; void* PLastReactionCreature; bool InRisingPhase; unsigned int MinGapFrames;
  float CountRechargePerFrame, CurrentAvailableCount, MaxCount, InvMaxCount; unsigned int FrameLastStarted, FrameLastRecharged;
  M(unsigned int maxCount, float minGap, float recharge, bool allow)
    : AllowIndividualRepeats(allow), PLastReactionCreature(0), InRisingPhase(false), MinGapFrames((unsigned int)(g_framesPerSecond * minGap)),
      CurrentAvailableCount((float)maxCount), MaxCount((float)maxCount), FrameLastStarted(0), FrameLastRecharged(0)
  { CountRechargePerFrame = recharge > 0.0f ? MaxCount / (g_framesPerSecond * recharge) : MaxCount; InvMaxCount = 1.0f / MaxCount; } };
int main()
{
    M a(4, 2.0f, 2.0f, true);
    M b(2, 0.5f, 0.0f, false);
    bool ok = a.AllowIndividualRepeats && a.MinGapFrames == 60 && a.CurrentAvailableCount == 4.0f && a.MaxCount == 4.0f
           && a.CountRechargePerFrame == 4.0f / 60.0f && a.InvMaxCount == 0.25f && !a.InRisingPhase && a.FrameLastStarted == 0
           && !b.AllowIndividualRepeats && b.MinGapFrames == 15 && b.CountRechargePerFrame == 2.0f && b.InvMaxCount == 0.5f;
    if (!ok) { std::printf("006fee20_TEST FAIL\n"); return 1; }
    std::printf("006fee20_TEST PASS\n");
    return 0;
}