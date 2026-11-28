#ifndef FABLE_ENGINE_CCREATUREACTION_ICETROLLICICLEASSAULTLOOP_H
#define FABLE_ENGINE_CCREATUREACTION_ICETROLLICICLEASSAULTLOOP_H
#ifdef FABLE_RETAIL_ICICLE_ASSAULT_PASS30
#include "engine/CCombatActionBase.h"
class CCreatureAction_IceTrollIcicleAssaultLoop : public CCombatActionBase {
public:
    CCreatureAction_IceTrollIcicleAssaultLoop(CThingCreatureBase& creature) : CCombatActionBase(creature, 150, CSoundPair(0)) {}
    virtual CCreatureActionBase* Clone() const;
    virtual void Initialise();
    virtual bool IsStrikeAction() const;
};
#endif
#endif
