#ifndef FABLE_ENGINE_CWITNESSESSEARCHINGDEED_H
#define FABLE_ENGINE_CWITNESSESSEARCHINGDEED_H
#include "engine/CIntelligentPointer.h"
#include "engine/EOpinionDeedType.h"
#include "engine/EOpinionPostDeedType.h"
class CThingAICreature;
class CFaction;
struct CWitnessesSearchingDeed {
    long DeedHandle;
    EOpinionDeedType DeedType;
    CIntelligentPointer<CThingAICreature> Recipient;
    EOpinionPostDeedType PostType;
    CFaction* PFaction;
    bool ReverseFactionSearch;
};
FABLE_STATIC_ASSERT(sizeof(CWitnessesSearchingDeed) == 0x1C);
#endif
