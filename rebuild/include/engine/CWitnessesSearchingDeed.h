#ifndef FABLE_ENGINE_CWITNESSESSEARCHINGDEED_H
#define FABLE_ENGINE_CWITNESSESSEARCHINGDEED_H
#include "engine/CIntelligentPointer.h"
#include "engine/EOpinionDeedType.h"
#include "engine/EOpinionPostDeedType.h"
class CThingAICreature;
class CFaction;
#ifdef FABLE_RETAIL_POST_DEED_PASS29
class CWitnessesSearchingDeed {
public:
    CWitnessesSearchingDeed(long,EOpinionDeedType,CThingAICreature*,EOpinionPostDeedType,const CFaction*,bool);
#else
struct CWitnessesSearchingDeed {
#endif
    long DeedHandle;
    EOpinionDeedType DeedType;
    CIntelligentPointer<CThingAICreature> Recipient;
    EOpinionPostDeedType PostType;
#ifdef FABLE_RETAIL_POST_DEED_PASS29
    const CFaction* PFaction;
#else
    CFaction* PFaction;
#endif
    bool ReverseFactionSearch;
};
FABLE_STATIC_ASSERT_AT(16, sizeof(CWitnessesSearchingDeed) == 0x1C);
#endif
