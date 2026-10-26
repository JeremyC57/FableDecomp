// CHeroMarriageDef::Transfer(CPersistContext& context) -- persist every tunable of the hero marriage def in member order
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00452398, length 507 bytes (virtual,
// `?Transfer@CHeroMarriageDef@@UAEXAAVCPersistContext@@@Z`).
// Callees: CPersistContext::Transfer<float> @ 0x00431061, Transfer<long> @ 0x00431102,
// Transfer<CCharString> @ 0x00431143 (namepass TR2: one member template) (each starts with the CheckCRC push 0x122d70e).
// Layout: engine/CHeroMarriageDef.h (Ego_r.pdb names), members +0x28..+0xc4; the CCharString members
// are modelled there as their PStringData pointer, so the string transfers pass that slot as the CCharString.
// Shape: context reference in edi, `this` in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call`
// per member; the last member is addressed with `add esi,0xc4` because esi is dead afterwards.
// The unaligned address (0x...398) means this sits in a size-optimised TU, hence optimize("s").
// Leaf: Transfer
#pragma optimize("s", on)
#include "engine/CHeroMarriageDef.h"

class CCharString;
class CPersistContext { public:
    template <class T> void Transfer(T& value);       // one-argument transfer helpers (namepass TR2)
};

struct CHeroMarriageDef_Transfer : CHeroMarriageDef {
    void Transfer(CPersistContext& context);
};

void CHeroMarriageDef_Transfer::Transfer(CPersistContext& context)
{
    context.Transfer(WeddingTimeForScreenToFadeOut);
    context.Transfer(WeddingTimeForScreenToFadeIn);
    context.Transfer(WeddingAfterFrescoTimeOfDayFastForwardTo);
    context.Transfer(SecondsJustMarriedDuration);
    context.Transfer(HeroStatRenownIncreasePerMarriage);
    context.Transfer(SecondsBetweenCheckingPlayersAppearance);
    context.Transfer(SecondsMaxTimeWithinNoticingCanReact);
    context.Transfer(FatnessChangeThresholdForComment);
    context.Transfer(SecondsBetweenReceivingGiftOpinionReactionCanHappen);
    context.Transfer(SecondsThatGiftsReceivedWillBeRemembered);
    context.Transfer(FramesBetweenCullingOldGifts);
    context.Transfer(SecondsBetweenEvaluatingGiftOpinion);
    context.Transfer(SecondsBetweenGiftGivingOpportunities);
    context.Transfer(ChanceThatGiftWillBeGiven);
    context.Transfer(AmountOfGoldPerHourOfMarriageToIncreaseGiftGivingPriceValuePerHourOfPlay);
    context.Transfer(MaxNumHoursOfMarriageToIncreaseGiftGivingPriceValue);
    context.Transfer(MultiplierForGiftGivingValueEveryOpportunity);
    context.Transfer(MaxDistanceFromMaxStatsThatGiftsCanBeGiven);
    context.Transfer(SecondsBetweenEvaluatingLoveAttitude);
    context.Transfer(SecondsOutOfTheRegionForLoveAttitudeToDecayOneToZero);
    context.Transfer(ThresholdForLoveAttitudeOverride);
    context.Transfer(*(CCharString*)&FallenInLoveSoundCriteria_PStringData);
    context.Transfer(FallenInLoveSoundGain);
    context.Transfer(FianceeOpinionPersonalityDef);
    context.Transfer(ProbabilityOfCourtingAdviceOnGreet);
    context.Transfer(SecondsTimeSinceSeeingYouMinThreshold);
    context.Transfer(SecondsRunOutWifeOnViolenceHateSpike);
    context.Transfer(PeakEffectWifeOnViolenceHateSpike);
    context.Transfer(PersistEffectWifeOnViolenceHateSpike);
    context.Transfer(SecsInHateBeforeDivorceOccurs);
    context.Transfer(SecsInHateBetweenDivorceWarnings);
    context.Transfer(*(CCharString*)&TextGuiWifeDivorceOccurred_PStringData);
    context.Transfer(*(CCharString*)&TextGuiWifeDivorceRenting_PStringData);
    context.Transfer(HeroStatMoralityInOpinionFormChangePerDivorce);
    context.Transfer(SecsInLoveAndHusbandPresentBeforeSexOffered);
    context.Transfer(SecsInLoveAndHusbandPresentBetweenSexOffers);
    context.Transfer(*(CCharString*)&TextGuiWifeSexOffered_PStringData);
    context.Transfer(SecsMinSoundDurationInSexCutscene);
    context.Transfer(SecsWaitAfterSoundInSexCutscene);
    context.Transfer(*(CCharString*)&DialogueGroupForSexCutscene_PStringData);
}