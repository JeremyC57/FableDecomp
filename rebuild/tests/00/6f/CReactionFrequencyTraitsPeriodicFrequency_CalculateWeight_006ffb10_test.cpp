#include <cstdio>
// Behaviour model of CReactionFrequencyTraits_PeriodicFrequency::CalculateWeight @ 0x006FFB10 (copy of the source logic).
static unsigned int g_frame = 0;
struct CWorld { static unsigned int GetWorldFrame() { return g_frame; } };
static const float& Clamp(const float& v, const float& lo, const float& hi) { if (v < lo) return lo; if (v > hi) return hi; return v; }
struct CReactionFrequencyTraits_PeriodicFrequency {
    float LowFreqPerMin, HighLowFreqInterval;
    unsigned int FramesPerMinute, LastCutoffFrame, RampUpTimeFrames, CutoffIntervalFrames, LastHappenedFrame;
    float CalculateWeight(const void*)
    {
        unsigned int frame = CWorld::GetWorldFrame();
        if (frame - LastCutoffFrame > CutoffIntervalFrames)
            LastCutoffFrame += CutoffIntervalFrames;
        float ramp = Clamp((float)(frame - LastCutoffFrame) / (float)RampUpTimeFrames, 0.0f, 1.0f);
        float frequency = ramp * ramp;
        frequency *= ramp;
        frequency = frequency * HighLowFreqInterval + LowFreqPerMin;
        float observed = (float)FramesPerMinute / (float)(frame - LastHappenedFrame);
        return Clamp((frequency - observed) / (frequency - LowFreqPerMin), 0.0f, 1.0f);
    }
};
int main()
{
    CReactionFrequencyTraits_PeriodicFrequency f = { 1.0f, 3.0f, 1800, 0, 100, 1000, 0 };
    g_frame = 50;  f.LastHappenedFrame = 0;                    // ramp 0.5 -> target 1.375/min; observed 36/min -> negative -> 0
    float a = f.CalculateWeight(0);
    g_frame = 3650; f.LastHappenedFrame = 0; f.FramesPerMinute = 10950;   // cutoff rolls once to 1000; ramp 1 -> target 4; observed 3
    float b = f.CalculateWeight(0);
    float observed = 10950.0f / 3650.0f;
    float expect = (4.0f - observed) / (4.0f - 1.0f);
    int ok = a == 0.0f && f.LastCutoffFrame == 1000 && b > expect - 1e-5f && b < expect + 1e-5f && b > 0.3f && b < 0.34f;
    if (!ok) { std::printf("006ffb10_TEST FAIL\n"); return 1; }
    std::printf("006ffb10_TEST PASS\n");
    return 0;
}