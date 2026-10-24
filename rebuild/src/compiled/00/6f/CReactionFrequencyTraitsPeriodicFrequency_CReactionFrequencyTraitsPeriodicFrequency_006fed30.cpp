// CReactionFrequencyTraits_PeriodicFrequency::CReactionFrequencyTraits_PeriodicFrequency(float rampUpTime, float cutoffInterval, float lowFreqPerMin, float highFreqPerMin) -- periodic reaction frequency: rates per minute and frame counts derived from the frame rate
// Leaf: ??0CReactionFrequencyTraits_PeriodicFrequency@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FED30, 92 bytes, `ret 0x10`.
// Label correction: catalog "ConsoleSetViewPort" is propagated; the vtable stored (0x01264B7C) is
//   CReactionFrequencyTraits_PeriodicFrequency's, so this is its (float, float, float, float) constructor.
// Callees: _ftol 0x00BFEA70 (float -> unsigned long, twice). Data: g_framesPerSecond 0x01375550.
// Shape: the init list holds the two rates and FramesPerMinute (`imul ecx,ecx,3Ch`); the body converts the ramp-up and
//   cutoff times to frames and zeroes LastCutoffFrame.
extern long g_framesPerSecond;                            // 0x01375550

class CThingAICreature;
class CMemoryDataInputStream;
class CDataOutputStream;
enum EReactionFrequencyType { REACTION_FREQUENCY_LINEAR_TIME = 1, REACTION_FREQUENCY_PERIODIC = 2, REACTION_FREQUENCY_CONTROLLED_COUNT = 3 };

class CReactionFrequencyTraits {                          // polymorphic base (vptr only)
public:
    CReactionFrequencyTraits() {}
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream) = 0;
    virtual void TransferBinaryOut(CDataOutputStream& stream) const = 0;
    virtual EReactionFrequencyType GetType() const = 0;
    virtual float CalculateWeight(const CThingAICreature* creature) = 0;
    virtual void LogReaction(const CThingAICreature& creature, bool happened) = 0;
};

class CReactionFrequencyTraits_PeriodicFrequency : public CReactionFrequencyTraits {   // vtable 0x01264B7C, 0x20 bytes
public:
    float LowFreqPerMin;                                  // +0x04
    float HighLowFreqInterval;                            // +0x08
    unsigned long FramesPerMinute;                        // +0x0c
    unsigned long LastCutoffFrame;                        // +0x10
    unsigned long RampUpTimeFrames;                       // +0x14
    unsigned long CutoffIntervalFrames;                   // +0x18
    unsigned long LastHappenedFrame;                      // +0x1c
    CReactionFrequencyTraits_PeriodicFrequency(float rampUpTime, float cutoffInterval, float lowFreqPerMin, float highFreqPerMin);
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream);
    virtual void TransferBinaryOut(CDataOutputStream& stream) const;
    virtual EReactionFrequencyType GetType() const;
    virtual float CalculateWeight(const CThingAICreature* creature);
    virtual void LogReaction(const CThingAICreature& creature, bool happened);
};

CReactionFrequencyTraits_PeriodicFrequency::CReactionFrequencyTraits_PeriodicFrequency(float rampUpTime, float cutoffInterval,
                                                                                       float lowFreqPerMin, float highFreqPerMin)
    : LowFreqPerMin(lowFreqPerMin), HighLowFreqInterval(highFreqPerMin - lowFreqPerMin), FramesPerMinute(g_framesPerSecond * 60)
{
    RampUpTimeFrames = (unsigned long)(g_framesPerSecond * rampUpTime);
    CutoffIntervalFrames = (unsigned long)((rampUpTime + cutoffInterval) * g_framesPerSecond);
    LastCutoffFrame = 0;
}