#ifndef FABLE_ENGINE_CCREATUREACTION_ALERTTOPLAYER_H
#define FABLE_ENGINE_CCREATUREACTION_ALERTTOPLAYER_H
// Both original PDB type records: public CCreatureActionBase, no own fields.
#include "engine/CCreatureActionBase.h"
class CThingAICreature;
class CCreatureAction_AlertToPlayer:public CCreatureActionBase {
public:
 CCreatureAction_AlertToPlayer(CThingAICreature& creature);
 virtual void PerformAction();
 virtual CCreatureActionBase* Clone()const;
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_AlertToPlayer)==0xa8);
#endif
