// Behaviour test for 006fed30 (CReactionFrequencyTraits_PeriodicFrequency(rampUpTime, cutoffInterval, lowFreqPerMin,
// highFreqPerMin)): rates per minute, frames per minute and the two frame counts from g_framesPerSecond.
#include <cstdio>

long g_framesPerSecond = 30;

class CThingAICreature;
enum EReactionFrequencyType { REACTION_FREQUENCY_LINEAR_TIME = 1, REACTION_FREQUENCY_PERIODIC = 2, REACTION_FREQUENCY_CONTROLLED_COUNT = 3 };

class CReactionFrequencyTraits {
public:
    CReactionFrequencyTraits() {}
    virtual EReactionFrequencyType GetType() const = 0;
};

class CReactionFrequencyTraits_PeriodicFrequency : public CReactionFrequencyTraits {
public:
    float LowFreqPerMin;
    float HighLowFreqInterval;
    unsigned long FramesPerMinute;
    unsigned long LastCutoffFrame;
    unsigned long RampUpTimeFrames;
    unsigned long CutoffIntervalFrames;
    unsigned long LastHappenedFrame;
    CReactionFrequencyTraits_PeriodicFrequency(float rampUpTime, float cutoffInterval, float lowFreqPerMin, float highFreqPerMin);
    virtual EReactionFrequencyType GetType() const { return REACTION_FREQUENCY_PERIODIC; }
};

CReactionFrequencyTraits_PeriodicFrequency::CReactionFrequencyTraits_PeriodicFrequency(float rampUpTime, float cutoffInterval,
                                                                                       float lowFreqPerMin, float highFreqPerMin)
    : LowFreqPerMin(lowFreqPerMin), HighLowFreqInterval(highFreqPerMin - lowFreqPerMin), FramesPerMinute(g_framesPerSecond * 60)
{
    RampUpTimeFrames = (unsigned long)(g_framesPerSecond * rampUpTime);
    CutoffIntervalFrames = (unsigned long)((rampUpTime + cutoffInterval) * g_framesPerSecond);
    LastCutoffFrame = 0;
}

int main()
{
    CReactionFrequencyTraits_PeriodicFrequency traits(2.5f, 4.0f, 1.5f, 6.0f);
    int ok = traits.LowFreqPerMin == 1.5f && traits.HighLowFreqInterval == 4.5f && traits.FramesPerMinute == 1800 &&
             traits.RampUpTimeFrames == 75 && traits.CutoffIntervalFrames == 195 && traits.LastCutoffFrame == 0 &&
             traits.GetType() == REACTION_FREQUENCY_PERIODIC;
    g_framesPerSecond = 25;
    CReactionFrequencyTraits_PeriodicFrequency other(0.1f, 0.0f, 3.0f, 3.0f);
    if (other.HighLowFreqInterval != 0.0f || other.FramesPerMinute != 1500 || other.RampUpTimeFrames != 2 ||
        other.CutoffIntervalFrames != 2)
        ok = 0;
    if (!ok) {
        std::printf("006fed30_TEST FAIL\n");
        return 1;
    }
    std::printf("006fed30_TEST PASS\n");
    return 0;
}