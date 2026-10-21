#include <cstdio>
#include <cstring>
// CHeroMarriageDef::operator=(const CHeroMarriageDef&) -- memberwise def copy: CDefClassBase part out of line, then the marriage tuning floats/longs and five CCharString members; returns *this
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00453911, length 399 bytes (odd address: size-optimised def TU).
// Pragma: #pragma optimize("s", on).
// Callees: CDefClassBase::operator= @ 0x00431F10 (catalogued "CThingBuildingDef::operator=", propagated: it copies
//          PDefManager/GlobalIndex and the DefaultValsApplied bits, the root def copy), CCharString::operator= @ 0x0099EFB0.
// Identity: the catalog label "CThingBuildingDef::operator=" is propagated. Vtable 0x01232074 (CHeroMarriageDef, RTTI)
// slot 19 (Copy) is the 5-byte `jmp` thunk at 0x0045390C right before this body.
// Layout: engine/CHeroMarriageDef.h (Ego_r.pdb) matches every copied offset (+0x28..+0xc4, CCharStrings at
// +0x7c/+0xa4/+0xa8/+0xb8/+0xc4), but it is a flattened pack(1) struct with no base class and raw CCharStringData
// pointers, so the copy cannot be written against it: the class is modelled here with a polymorphic CDefClassBase
// base (0x25 bytes, out-of-line operator=) and real CCharString members, at the header's offsets and names.
struct CCharStringData;

struct CCharString {
    CCharStringData* PStringData;
    CCharString& operator=(const CCharString& other);    // 0x0099EFB0
};

#pragma pack(push, 1)
struct CDefClassBase {
    virtual void Copy(const CDefClassBase& other);
    unsigned char _base_0x04[0x18];
    void*         PDefManager;                          // +0x1c
    unsigned long GlobalIndex;                          // +0x20
    unsigned char DefaultValsApplied;                   // +0x24 bitfield group
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
#pragma pack(pop)

struct CHeroMarriageDef : CDefClassBase {               // vtable 0x01232074
    float       WeddingTimeForScreenToFadeOut;          // +0x28
    float       WeddingTimeForScreenToFadeIn;           // +0x2c
    float       WeddingAfterFrescoTimeOfDayFastForwardTo; // +0x30
    float       SecondsJustMarriedDuration;             // +0x34
    float       HeroStatRenownIncreasePerMarriage;      // +0x38
    long        SecondsBetweenCheckingPlayersAppearance; // +0x3c
    long        SecondsMaxTimeWithinNoticingCanReact;   // +0x40
    float       FatnessChangeThresholdForComment;       // +0x44
    long        SecondsBetweenReceivingGiftOpinionReactionCanHappen; // +0x48
    long        SecondsThatGiftsReceivedWillBeRemembered; // +0x4c
    long        FramesBetweenCullingOldGifts;           // +0x50
    long        SecondsBetweenEvaluatingGiftOpinion;    // +0x54
    long        SecondsBetweenGiftGivingOpportunities;  // +0x58
    float       ChanceThatGiftWillBeGiven;              // +0x5c
    float       AmountOfGoldPerHourOfMarriageToIncreaseGiftGivingPriceValuePerHourOfPlay; // +0x60
    float       MaxNumHoursOfMarriageToIncreaseGiftGivingPriceValue; // +0x64
    float       MultiplierForGiftGivingValueEveryOpportunity; // +0x68
    float       MaxDistanceFromMaxStatsThatGiftsCanBeGiven; // +0x6c
    long        SecondsBetweenEvaluatingLoveAttitude;   // +0x70
    float       SecondsOutOfTheRegionForLoveAttitudeToDecayOneToZero; // +0x74
    float       ThresholdForLoveAttitudeOverride;       // +0x78
    CCharString FallenInLoveSoundCriteria;              // +0x7c
    float       FallenInLoveSoundGain;                  // +0x80
    long        FianceeOpinionPersonalityDef;           // +0x84
    float       ProbabilityOfCourtingAdviceOnGreet;     // +0x88
    float       SecondsTimeSinceSeeingYouMinThreshold;  // +0x8c
    float       SecondsRunOutWifeOnViolenceHateSpike;   // +0x90
    float       PeakEffectWifeOnViolenceHateSpike;      // +0x94
    float       PersistEffectWifeOnViolenceHateSpike;   // +0x98
    float       SecsInHateBeforeDivorceOccurs;          // +0x9c
    float       SecsInHateBetweenDivorceWarnings;       // +0xa0
    CCharString TextGuiWifeDivorceOccurred;             // +0xa4
    CCharString TextGuiWifeDivorceRenting;              // +0xa8
    float       HeroStatMoralityInOpinionFormChangePerDivorce; // +0xac
    float       SecsInLoveAndHusbandPresentBeforeSexOffered; // +0xb0
    float       SecsInLoveAndHusbandPresentBetweenSexOffers; // +0xb4
    CCharString TextGuiWifeSexOffered;                  // +0xb8
    float       SecsMinSoundDurationInSexCutscene;      // +0xbc
    float       SecsWaitAfterSoundInSexCutscene;        // +0xc0
    CCharString DialogueGroupForSexCutscene;            // +0xc4
    CHeroMarriageDef& operator=(const CHeroMarriageDef& other);
};

CHeroMarriageDef& CHeroMarriageDef::operator=(const CHeroMarriageDef& other)
{
    CDefClassBase::operator=(other);
    WeddingTimeForScreenToFadeOut = other.WeddingTimeForScreenToFadeOut;
    WeddingTimeForScreenToFadeIn = other.WeddingTimeForScreenToFadeIn;
    WeddingAfterFrescoTimeOfDayFastForwardTo = other.WeddingAfterFrescoTimeOfDayFastForwardTo;
    SecondsJustMarriedDuration = other.SecondsJustMarriedDuration;
    HeroStatRenownIncreasePerMarriage = other.HeroStatRenownIncreasePerMarriage;
    SecondsBetweenCheckingPlayersAppearance = other.SecondsBetweenCheckingPlayersAppearance;
    SecondsMaxTimeWithinNoticingCanReact = other.SecondsMaxTimeWithinNoticingCanReact;
    FatnessChangeThresholdForComment = other.FatnessChangeThresholdForComment;
    SecondsBetweenReceivingGiftOpinionReactionCanHappen = other.SecondsBetweenReceivingGiftOpinionReactionCanHappen;
    SecondsThatGiftsReceivedWillBeRemembered = other.SecondsThatGiftsReceivedWillBeRemembered;
    FramesBetweenCullingOldGifts = other.FramesBetweenCullingOldGifts;
    SecondsBetweenEvaluatingGiftOpinion = other.SecondsBetweenEvaluatingGiftOpinion;
    SecondsBetweenGiftGivingOpportunities = other.SecondsBetweenGiftGivingOpportunities;
    ChanceThatGiftWillBeGiven = other.ChanceThatGiftWillBeGiven;
    AmountOfGoldPerHourOfMarriageToIncreaseGiftGivingPriceValuePerHourOfPlay = other.AmountOfGoldPerHourOfMarriageToIncreaseGiftGivingPriceValuePerHourOfPlay;
    MaxNumHoursOfMarriageToIncreaseGiftGivingPriceValue = other.MaxNumHoursOfMarriageToIncreaseGiftGivingPriceValue;
    MultiplierForGiftGivingValueEveryOpportunity = other.MultiplierForGiftGivingValueEveryOpportunity;
    MaxDistanceFromMaxStatsThatGiftsCanBeGiven = other.MaxDistanceFromMaxStatsThatGiftsCanBeGiven;
    SecondsBetweenEvaluatingLoveAttitude = other.SecondsBetweenEvaluatingLoveAttitude;
    SecondsOutOfTheRegionForLoveAttitudeToDecayOneToZero = other.SecondsOutOfTheRegionForLoveAttitudeToDecayOneToZero;
    ThresholdForLoveAttitudeOverride = other.ThresholdForLoveAttitudeOverride;
    FallenInLoveSoundCriteria = other.FallenInLoveSoundCriteria;
    FallenInLoveSoundGain = other.FallenInLoveSoundGain;
    FianceeOpinionPersonalityDef = other.FianceeOpinionPersonalityDef;
    ProbabilityOfCourtingAdviceOnGreet = other.ProbabilityOfCourtingAdviceOnGreet;
    SecondsTimeSinceSeeingYouMinThreshold = other.SecondsTimeSinceSeeingYouMinThreshold;
    SecondsRunOutWifeOnViolenceHateSpike = other.SecondsRunOutWifeOnViolenceHateSpike;
    PeakEffectWifeOnViolenceHateSpike = other.PeakEffectWifeOnViolenceHateSpike;
    PersistEffectWifeOnViolenceHateSpike = other.PersistEffectWifeOnViolenceHateSpike;
    SecsInHateBeforeDivorceOccurs = other.SecsInHateBeforeDivorceOccurs;
    SecsInHateBetweenDivorceWarnings = other.SecsInHateBetweenDivorceWarnings;
    TextGuiWifeDivorceOccurred = other.TextGuiWifeDivorceOccurred;
    TextGuiWifeDivorceRenting = other.TextGuiWifeDivorceRenting;
    HeroStatMoralityInOpinionFormChangePerDivorce = other.HeroStatMoralityInOpinionFormChangePerDivorce;
    SecsInLoveAndHusbandPresentBeforeSexOffered = other.SecsInLoveAndHusbandPresentBeforeSexOffered;
    SecsInLoveAndHusbandPresentBetweenSexOffers = other.SecsInLoveAndHusbandPresentBetweenSexOffers;
    TextGuiWifeSexOffered = other.TextGuiWifeSexOffered;
    SecsMinSoundDurationInSexCutscene = other.SecsMinSoundDurationInSexCutscene;
    SecsWaitAfterSoundInSexCutscene = other.SecsWaitAfterSoundInSexCutscene;
    DialogueGroupForSexCutscene = other.DialogueGroupForSexCutscene;
    return *this;
}


static int g_charAssigns = 0;
CCharString& CCharString::operator=(const CCharString& other) { PStringData = other.PStringData; ++g_charAssigns; return *this; }
void CDefClassBase::Copy(const CDefClassBase& other) { *this = other; }
static int g_baseAssigns = 0;
CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other)
{ PDefManager = other.PDefManager; GlobalIndex = other.GlobalIndex; DefaultValsApplied = other.DefaultValsApplied; ++g_baseAssigns; return *this; }

int main()
{
    static CHeroMarriageDef a, b;
    a.GlobalIndex = 77; a.WeddingTimeForScreenToFadeOut = 1.5f; a.SecondsBetweenEvaluatingLoveAttitude = 42;
    a.ThresholdForLoveAttitudeOverride = 0.25f; a.FallenInLoveSoundCriteria.PStringData = (CCharStringData*)&a;
    a.DialogueGroupForSexCutscene.PStringData = (CCharStringData*)&b; a.SecsWaitAfterSoundInSexCutscene = 9.0f;
    a.FianceeOpinionPersonalityDef = -3; a.HeroStatMoralityInOpinionFormChangePerDivorce = 2.5f;
    CHeroMarriageDef& r = (b = a);
    if (&r != &b || b.GlobalIndex != 77 || b.WeddingTimeForScreenToFadeOut != 1.5f || b.SecondsBetweenEvaluatingLoveAttitude != 42
        || b.ThresholdForLoveAttitudeOverride != 0.25f || b.FallenInLoveSoundCriteria.PStringData != (CCharStringData*)&a
        || b.DialogueGroupForSexCutscene.PStringData != (CCharStringData*)&b || b.SecsWaitAfterSoundInSexCutscene != 9.0f
        || b.FianceeOpinionPersonalityDef != -3 || b.HeroStatMoralityInOpinionFormChangePerDivorce != 2.5f
        || g_charAssigns != 5 || g_baseAssigns != 1) { std::printf("00453911_TEST FAIL\n"); return 1; }
    std::printf("00453911_TEST PASS\n");
    return 0;
}
