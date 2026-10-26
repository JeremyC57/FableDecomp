// Behaviour test for CReactionFrequencyTraits_ControlledCount::TransferBinaryOut @ 0x006FEF30: the persisted members in order.
#include <cstdio>

class CDataOutputStream { public:
    int kinds[16]; float values[16]; int n;
    CDataOutputStream() : n(0) {}
    void WriteBool(bool v) { kinds[n] = 1; values[n++] = v ? 1.0f : 0.0f; }
    void WriteULong(unsigned long v) { kinds[n] = 2; values[n++] = (float)v; }
    void WriteFloat(float v) { kinds[n] = 3; values[n++] = v; }
};

struct CReactionFrequencyTraits_ControlledCount {
    bool AllowIndividualRepeats; void* PLastReactionCreature; bool InRisingPhase;
    unsigned long MinGapFrames; float CountRechargePerFrame, CurrentAvailableCount, MaxCount, InvMaxCount;
    unsigned long FrameLastStarted, FrameLastRecharged;
    void TransferBinaryOut(CDataOutputStream& stream) const
    {
        stream.WriteBool(AllowIndividualRepeats);
        stream.WriteULong(MinGapFrames);
        stream.WriteFloat(CountRechargePerFrame);
        stream.WriteFloat(CurrentAvailableCount);
        stream.WriteFloat(MaxCount);
        stream.WriteFloat(InvMaxCount);
    }
};

int main()
{
    CReactionFrequencyTraits_ControlledCount t;
    t.AllowIndividualRepeats = true; t.PLastReactionCreature = 0; t.InRisingPhase = true; t.MinGapFrames = 30;
    t.CountRechargePerFrame = 0.25f; t.CurrentAvailableCount = 2.0f; t.MaxCount = 4.0f; t.InvMaxCount = 0.25f;
    t.FrameLastStarted = 7; t.FrameLastRecharged = 9;
    CDataOutputStream s;
    t.TransferBinaryOut(s);
    const int kinds[6] = { 1, 2, 3, 3, 3, 3 };
    const float values[6] = { 1.0f, 30.0f, 0.25f, 2.0f, 4.0f, 0.25f };
    if (s.n != 6) { std::printf("006fef30_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 6; ++i)
        if (s.kinds[i] != kinds[i] || s.values[i] != values[i]) { std::printf("006fef30_TEST FAIL\n"); return 1; }
    std::printf("006fef30_TEST PASS\n");
    return 0;
}