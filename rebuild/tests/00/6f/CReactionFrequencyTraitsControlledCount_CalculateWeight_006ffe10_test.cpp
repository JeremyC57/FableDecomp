// Behaviour test for CReactionFrequencyTraits_ControlledCount::CalculateWeight (0x006FFE10): an immediate repeat by
// the last creature weighs 0; while rising the count recharges by elapsed frames (unsigned) and capping it ends the
// phase and forgets the creature; before the minimum gap the weight is 0; otherwise count/max clamped to [0, 1].
#include <cstdio>
struct CThingAICreature { int Id; };
static long g_frame;
struct CWorld { static long GetWorldFrame() { return g_frame; } };
inline const float& Clamp(const float& v, const float& lo, const float& hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}
struct CReactionFrequencyTraits_ControlledCount {
    bool              AllowIndividualRepeats;
    CThingAICreature* PLastReactionCreature;
    bool              InRisingPhase;
    unsigned long     MinGapFrames;
    float             CountRechargePerFrame;
    float             CurrentAvailableCount;
    float             MaxCount;
    float             InvMaxCount;
    unsigned long     FrameLastStarted;
    unsigned long     FrameLastRecharged;
    float CalculateWeight(const CThingAICreature* creature);
};
float CReactionFrequencyTraits_ControlledCount::CalculateWeight(const CThingAICreature* creature)
{
    unsigned long frame = CWorld::GetWorldFrame();
    if (!AllowIndividualRepeats && creature && PLastReactionCreature && PLastReactionCreature == creature)
        return 0.0f;
    if (InRisingPhase) {
        CurrentAvailableCount += (float)(frame - FrameLastRecharged) * CountRechargePerFrame;
        FrameLastRecharged = frame;
        if (CurrentAvailableCount >= MaxCount) {
            CurrentAvailableCount = MaxCount;
            PLastReactionCreature = 0;
            InRisingPhase = false;
        }
    }
    if (!InRisingPhase && frame - FrameLastStarted >= MinGapFrames)
        return Clamp(InvMaxCount * CurrentAvailableCount, 0.0f, 1.0f);
    return 0.0f;
}
int main()
{
    CThingAICreature a = { 1 }, b = { 2 };
    CReactionFrequencyTraits_ControlledCount t = { false, &a, true, 10, 0.5f, 1.0f, 4.0f, 0.25f, 100, 100 };
    int bad = 0;
    g_frame = 104;
    if (t.CalculateWeight(&a) != 0.0f || t.FrameLastRecharged != 100 || t.CurrentAvailableCount != 1.0f) bad |= 1;   // repeat
    if (t.CalculateWeight(&b) != 0.0f || t.CurrentAvailableCount != 3.0f || t.FrameLastRecharged != 104 || !t.InRisingPhase) bad |= 2;
    g_frame = 112;                                         // +8 frames: 3 + 4 = 7 -> capped at 4, phase over
    float w = t.CalculateWeight(&b);
    if (w != 1.0f || t.CurrentAvailableCount != 4.0f || t.InRisingPhase || t.PLastReactionCreature != 0) bad |= 4;
    t.CurrentAvailableCount = 2.0f;
    if (t.CalculateWeight(&a) != 0.5f) bad |= 8;
    t.FrameLastStarted = 105; g_frame = 112;               // gap 7 < 10
    if (t.CalculateWeight(&a) != 0.0f) bad |= 16;
    t.FrameLastStarted = 100; t.CurrentAvailableCount = 8.0f;
    if (t.CalculateWeight(0) != 1.0f) bad |= 32;           // clamped high
    t.CurrentAvailableCount = -2.0f;
    if (t.CalculateWeight(0) != 0.0f) bad |= 64;           // clamped low
    t.AllowIndividualRepeats = true; t.PLastReactionCreature = &a; t.CurrentAvailableCount = 2.0f;
    if (t.CalculateWeight(&a) != 0.5f) bad |= 128;         // repeats allowed
    if (bad) { std::printf("006ffe10_TEST FAIL\n"); return 1; }
    std::printf("006ffe10_TEST PASS\n");
    return 0;
}