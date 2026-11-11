#ifndef FABLE_ENGINE_CCREATUREACTION_DECLAREPLAYERACRIMINAL_H
#define FABLE_ENGINE_CCREATUREACTION_DECLAREPLAYERACRIMINAL_H
// Complete original owner from both original PDBs.
#include "engine/CCreatureActionBase.h"
class CThingPlayerCreature;
class CCreatureAction_DeclarePlayerACriminal:public CCreatureActionBase{
public:
 CCreatureAction_DeclarePlayerACriminal(const CCreatureAction_DeclarePlayerACriminal & p0);
 CCreatureAction_DeclarePlayerACriminal(CThingCreatureBase & p0, CThingPlayerCreature & p1);
 virtual void PerformAction();
 virtual CCreatureActionBase * Clone() const;
private:
 CIntelligentPointer<CThingPlayerCreature> PCriminal;
public:
 virtual ~CCreatureAction_DeclarePlayerACriminal();
 CCreatureAction_DeclarePlayerACriminal & operator=(const CCreatureAction_DeclarePlayerACriminal & p0);
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_DeclarePlayerACriminal)==176);
#endif
