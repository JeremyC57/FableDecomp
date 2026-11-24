#ifndef FABLE_ENGINE_CTHINGFILTER_ISELIGIBLEWITNESS_H
#define FABLE_ENGINE_CTHINGFILTER_ISELIGIBLEWITNESS_H
#ifdef FABLE_RETAIL_DEED_WITNESS_PASS28
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_IsAlliedWithFaction.h"
#include "engine/CThingFilter_IsDeedTypeEnabled.h"
#include "engine/CThingFilter_IsPerceivingThingAndIsThingPerceived.h"
#include "engine/CThingFilter_IsEnemyOfFaction.h"
class CThingFilter_IsEligibleWitness {
public:
    CThingFilter_IsEligibleWitness(const COpinionDeed* deed, CThing& perpetrator, bool useHearing, CThingAICreature* recipient,
                                   const CFaction* faction, bool reverseFactionSearch, bool unknownThreatSearch);   // 0x006C8DE0
    bool operator()(const CThing& thing) const;         // 0x006C8E90
private:
    CThing& Perpetrator;
    const COpinionDeed* PDeed;
    CThingFilter_HasTC HasOpinions;
    CThingFilter_IsDeedTypeEnabled DeedTypeEnabled;
    CThingFilter_IsPerceivingThingAndIsThingPerceived IsPerceivingPerpetrator;
    CThingFilter_IsNot IsNotRecipient;
    CThingFilter_IsAlliedWithFaction IsAllied;
    CThingFilter_IsEnemyOfFaction IsEnemy;
    bool UnknownThreatSearch, UseHearing, UsePerceiving, UseIsNot, UseFaction, ReverseFactionSearch;
};

#endif
#endif
