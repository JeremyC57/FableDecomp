// CReactionFrequencyTraits::CreateFromType(EReactionFrequencyType type) -- factory: a default-constructed linear-time, periodic or controlled-count frequency trait, or null
// Leaf: ?CreateFromType@CReactionFrequencyTraits@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FF930, 102 bytes, plain `ret` (static __fastcall, type in ecx).
// Callees: operator new 0x00BFEA1A. Vtables: LinearTime 0x01264B64, PeriodicFrequency 0x01264B7C, ControlledCount
//   0x01264B94.
// Shape: a switch on 1/2/3 (`dec ecx` chain) returning `new` of each traits class with its default constructor's
//   member zeroing; 0 for any other type.
void* __cdecl operator new(unsigned int size);            // MSVCR71 import thunk 0x00BFEA1A

class CThingAICreature;
class CMemoryDataInputStream;
class CDataOutputStream;
enum EReactionFrequencyType { REACTION_FREQUENCY_LINEAR_TIME = 1, REACTION_FREQUENCY_PERIODIC = 2, REACTION_FREQUENCY_CONTROLLED_COUNT = 3 };

class CReactionFrequencyTraits {                          // polymorphic base (vptr only)
public:
    CReactionFrequencyTraits() {}
    static CReactionFrequencyTraits* __fastcall CreateFromType(EReactionFrequencyType type);
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream) = 0;
    virtual void TransferBinaryOut(CDataOutputStream& stream) const = 0;
    virtual EReactionFrequencyType GetType() const = 0;
    virtual float CalculateWeight(const CThingAICreature* creature) = 0;
    virtual void LogReaction(const CThingAICreature& creature, bool happened) = 0;
};

class CReactionFrequencyTraits_LinearTime : public CReactionFrequencyTraits {   // vtable 0x01264B64, 0x10 bytes
public:
    float MinWait;                                        // +0x04
    float WaitRange;                                      // +0x08
    unsigned long FrameHappened;                          // +0x0c
    CReactionFrequencyTraits_LinearTime() : FrameHappened(0) {}
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream);
    virtual void TransferBinaryOut(CDataOutputStream& stream) const;
    virtual EReactionFrequencyType GetType() const;
    virtual float CalculateWeight(const CThingAICreature* creature);
    virtual void LogReaction(const CThingAICreature& creature, bool happened);
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
    CReactionFrequencyTraits_PeriodicFrequency() : LastCutoffFrame(0), LastHappenedFrame(0) {}
    CReactionFrequencyTraits_PeriodicFrequency(float rampUpTime, float cutoffInterval, float lowFreqPerMin, float highFreqPerMin);
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream);
    virtual void TransferBinaryOut(CDataOutputStream& stream) const;
    virtual EReactionFrequencyType GetType() const;
    virtual float CalculateWeight(const CThingAICreature* creature);
    virtual void LogReaction(const CThingAICreature& creature, bool happened);
};

class CReactionFrequencyTraits_ControlledCount : public CReactionFrequencyTraits {   // vtable 0x01264B94, 0x2c bytes
public:
    bool AllowIndividualRepeats;                          // +0x04
    CThingAICreature* PLastReactionCreature;              // +0x08
    bool InRisingPhase;                                   // +0x0c
    unsigned long MinGapFrames;                           // +0x10
    float CountRechargePerFrame;                          // +0x14
    float CurrentAvailableCount;                          // +0x18
    float MaxCount;                                       // +0x1c
    float InvMaxCount;                                    // +0x20
    unsigned long FrameLastStarted;                       // +0x24
    unsigned long FrameLastRecharged;                     // +0x28
    CReactionFrequencyTraits_ControlledCount() : PLastReactionCreature(0), InRisingPhase(false), FrameLastStarted(0), FrameLastRecharged(0) {}
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream);
    virtual void TransferBinaryOut(CDataOutputStream& stream) const;
    virtual EReactionFrequencyType GetType() const;
    virtual float CalculateWeight(const CThingAICreature* creature);
    virtual void LogReaction(const CThingAICreature& creature, bool happened);
};

CReactionFrequencyTraits* __fastcall CReactionFrequencyTraits::CreateFromType(EReactionFrequencyType type)
{
    switch (type) {
    case REACTION_FREQUENCY_LINEAR_TIME:
        return new CReactionFrequencyTraits_LinearTime;
    case REACTION_FREQUENCY_PERIODIC:
        return new CReactionFrequencyTraits_PeriodicFrequency;
    case REACTION_FREQUENCY_CONTROLLED_COUNT:
        return new CReactionFrequencyTraits_ControlledCount;
    }
    return 0;
}