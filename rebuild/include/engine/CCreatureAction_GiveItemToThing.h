#ifndef FABLE_ENGINE_CCREATUREACTION_GIVEITEMTOTHING_H
#define FABLE_ENGINE_CCREATUREACTION_GIVEITEMTOTHING_H
#ifdef FABLE_RETAIL_GIVE_ITEM_ACTION_PASS26
#include "engine/CCreatureActionBase.h"
class CCreatureAction_GiveItemToThing : public CCreatureActionBase {
public:
    virtual void PerformAction();
private:
    CIntelligentPointer<CThing> PReceivingThing;
    CIntelligentPointer<CThing> PItemBeingGiven;
};
#endif
#endif
