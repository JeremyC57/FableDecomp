#ifndef FABLE_ENGINE_CCREATUREACTION_BANDITKINGCHANGEWEAPONS_H
#define FABLE_ENGINE_CCREATUREACTION_BANDITKINGCHANGEWEAPONS_H
#include "engine/CCreatureActionBase.h"
#include "engine/CIntelligentPointer.h"
class CCreatureAction_BanditKingChangeWeapons:public CCreatureActionBase {
public:
 CCreatureAction_BanditKingChangeWeapons(CThingCreatureBase&);
 virtual void PerformAction();
 virtual CCreatureActionBase* Clone()const;
private:
 bool Knockdown;
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_BanditKingChangeWeapons)==172);
#endif
