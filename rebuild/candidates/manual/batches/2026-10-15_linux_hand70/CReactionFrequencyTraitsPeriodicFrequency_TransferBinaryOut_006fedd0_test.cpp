// Behaviour test for CReactionFrequencyTraits_PeriodicFrequency::TransferBinaryOut @ 0x006FEDD0.
#include <cstdio>

struct CDataOutputStream {
    int kinds[16]; float values[16]; int n;
    CDataOutputStream() : n(0) {}
    void WriteULong(unsigned long v) { kinds[n] = 2; values[n++] = (float)v; }
    void WriteFloat(float v) { kinds[n] = 3; values[n++] = v; }
};

struct CReactionFrequencyTraits_PeriodicFrequency {
    float LowFreqPerMin, HighLowFreqInterval;
    unsigned long FramesPerMinute, LastCutoffFrame, RampUpTimeFrames, CutoffIntervalFrames, LastHappenedFrame;
    void TransferBinaryOut(CDataOutputStream& stream) const
    {
        stream.WriteFloat(LowFreqPerMin);
        stream.WriteFloat(HighLowFreqInterval);
        stream.WriteULong(FramesPerMinute);
        stream.WriteULong(RampUpTimeFrames);
        stream.WriteULong(CutoffIntervalFrames);
    }
};

int main()
{
    CReactionFrequencyTraits_PeriodicFrequency t;
    t.LowFreqPerMin = 1.5f; t.HighLowFreqInterval = 3.0f; t.FramesPerMinute = 1800; t.LastCutoffFrame = 11;
    t.RampUpTimeFrames = 60; t.CutoffIntervalFrames = 90; t.LastHappenedFrame = 13;
    CDataOutputStream s;
    t.TransferBinaryOut(s);
    const int kinds[5] = { 3, 3, 2, 2, 2 };
    const float values[5] = { 1.5f, 3.0f, 1800.0f, 60.0f, 90.0f };
    if (s.n != 5) { std::printf("006fedd0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (s.kinds[i] != kinds[i] || s.values[i] != values[i]) { std::printf("006fedd0_TEST FAIL\n"); return 1; }
    std::printf("006fedd0_TEST PASS\n");
    return 0;
}
