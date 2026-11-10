#ifndef FABLE_ENGINE_CCREATUREACTION_KICKCREATURE_H
#define FABLE_ENGINE_CCREATUREACTION_KICKCREATURE_H
#include "engine/CCreatureActionBase.h"
#include "engine/CIntelligentPointer.h"
class CCreatureAction_KickCreature:public CCreatureActionBase {
public:
 CCreatureAction_KickCreature(CThingCreatureBase&,CThingCreatureBase*);
 virtual void PerformAction();
 virtual CCreatureActionBase* Clone()const;
private:
 CIntelligentPointer<CThingCreatureBase> PTarget;
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_KickCreature)==176);
#endif
