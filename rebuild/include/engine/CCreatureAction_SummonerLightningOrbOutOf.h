#ifndef FABLE_ENGINE_CCREATUREACTION_SUMMONERLIGHTNINGORBOUTOF_H
#define FABLE_ENGINE_CCREATUREACTION_SUMMONERLIGHTNINGORBOUTOF_H
#include "engine/CCombatActionBase.h"
class CThing;
class CThingCreatureBase;
class CCreatureAction_SummonerLightningOrbOutOf : public CCombatActionBase {
public:
    virtual CCreatureActionBase* Clone() const;
    virtual void Initialise();
    virtual void PerformAction();
    virtual void FinishAction();
    CCreatureAction_SummonerLightningOrbOutOf(CThingCreatureBase&, CThing*);
};
class CCreatureAction_SummonerLightningOrbOutOfGroundSmash : public CCombatActionBase {
public:
    virtual CCreatureActionBase* Clone() const;
    virtual void Initialise();
    virtual void PerformAction();
    virtual void FinishAction();
    explicit CCreatureAction_SummonerLightningOrbOutOfGroundSmash(CThingCreatureBase&);
};
#endif
