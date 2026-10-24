// Behaviour test for 006ff930 (CReactionFrequencyTraits::CreateFromType): types 1/2/3 give a default-constructed
// linear-time / periodic / controlled-count traits object (members zeroed as their constructors do); others give null.
#include <cstdio>

class CThingAICreature;
enum EReactionFrequencyType { REACTION_FREQUENCY_LINEAR_TIME = 1, REACTION_FREQUENCY_PERIODIC = 2, REACTION_FREQUENCY_CONTROLLED_COUNT = 3 };

class CReactionFrequencyTraits {
public:
    CReactionFrequencyTraits() {}
    virtual ~CReactionFrequencyTraits() {}
    static CReactionFrequencyTraits* CreateFromType(EReactionFrequencyType type);
    virtual EReactionFrequencyType GetType() const = 0;
};

class CReactionFrequencyTraits_LinearTime : public CReactionFrequencyTraits {
public:
    float MinWait;
    float WaitRange;
    unsigned long FrameHappened;
    CReactionFrequencyTraits_LinearTime() : FrameHappened(0) {}
    virtual EReactionFrequencyType GetType() const { return REACTION_FREQUENCY_LINEAR_TIME; }
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
    CReactionFrequencyTraits_PeriodicFrequency() : LastCutoffFrame(0), LastHappenedFrame(0) {}
    virtual EReactionFrequencyType GetType() const { return REACTION_FREQUENCY_PERIODIC; }
};

class CReactionFrequencyTraits_ControlledCount : public CReactionFrequencyTraits {
public:
    bool AllowIndividualRepeats;
    CThingAICreature* PLastReactionCreature;
    bool InRisingPhase;
    unsigned long MinGapFrames;
    float CountRechargePerFrame;
    float CurrentAvailableCount;
    float MaxCount;
    float InvMaxCount;
    unsigned long FrameLastStarted;
    unsigned long FrameLastRecharged;
    CReactionFrequencyTraits_ControlledCount() : PLastReactionCreature(0), InRisingPhase(false), FrameLastStarted(0), FrameLastRecharged(0) {}
    virtual EReactionFrequencyType GetType() const { return REACTION_FREQUENCY_CONTROLLED_COUNT; }
};

CReactionFrequencyTraits* CReactionFrequencyTraits::CreateFromType(EReactionFrequencyType type)
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

int main()
{
    int ok = 1;
    CReactionFrequencyTraits* linear = CReactionFrequencyTraits::CreateFromType(REACTION_FREQUENCY_LINEAR_TIME);
    if (!linear || linear->GetType() != REACTION_FREQUENCY_LINEAR_TIME ||
        static_cast<CReactionFrequencyTraits_LinearTime*>(linear)->FrameHappened != 0)
        ok = 0;
    CReactionFrequencyTraits* periodic = CReactionFrequencyTraits::CreateFromType(REACTION_FREQUENCY_PERIODIC);
    if (!periodic || periodic->GetType() != REACTION_FREQUENCY_PERIODIC ||
        static_cast<CReactionFrequencyTraits_PeriodicFrequency*>(periodic)->LastCutoffFrame != 0 ||
        static_cast<CReactionFrequencyTraits_PeriodicFrequency*>(periodic)->LastHappenedFrame != 0)
        ok = 0;
    CReactionFrequencyTraits* counted = CReactionFrequencyTraits::CreateFromType(REACTION_FREQUENCY_CONTROLLED_COUNT);
    if (!counted || counted->GetType() != REACTION_FREQUENCY_CONTROLLED_COUNT) {
        ok = 0;
    }
    else {
        CReactionFrequencyTraits_ControlledCount* c = static_cast<CReactionFrequencyTraits_ControlledCount*>(counted);
        if (c->PLastReactionCreature != 0 || c->InRisingPhase || c->FrameLastStarted != 0 || c->FrameLastRecharged != 0)
            ok = 0;
    }
    if (CReactionFrequencyTraits::CreateFromType((EReactionFrequencyType)0) != 0 ||
        CReactionFrequencyTraits::CreateFromType((EReactionFrequencyType)4) != 0)
        ok = 0;
    delete linear;
    delete periodic;
    delete counted;
    if (!ok) {
        std::printf("006ff930_TEST FAIL\n");
        return 1;
    }
    std::printf("006ff930_TEST PASS\n");
    return 0;
}