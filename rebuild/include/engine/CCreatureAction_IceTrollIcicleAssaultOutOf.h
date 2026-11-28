#ifndef FABLE_ENGINE_CCREATUREACTION_ICETROLLICICLEASSAULTOUTOF_H
#define FABLE_ENGINE_CCREATUREACTION_ICETROLLICICLEASSAULTOUTOF_H
#ifdef FABLE_RETAIL_ICICLE_ASSAULT_PASS30
#include "engine/CCombatActionBase.h"
class CCreatureAction_IceTrollIcicleAssaultOutOf : public CCombatActionBase {
public:
    CCreatureAction_IceTrollIcicleAssaultOutOf(CThingCreatureBase& creature);
    virtual CCreatureActionBase* Clone() const;
    virtual void Initialise();
};
#endif
#endif
