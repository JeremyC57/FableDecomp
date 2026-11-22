#ifndef FABLE_ENGINE_CCREATUREACTION_DRAGONFIREBLASTFORWARD_H
#define FABLE_ENGINE_CCREATUREACTION_DRAGONFIREBLASTFORWARD_H
#ifdef FABLE_RETAIL_JACKDRAGON_HOVER_PASS27
#include "engine/CCreatureAction_DragonFireBlastBase.h"
class CCreatureAction_DragonFireBlastForward : public CCreatureAction_DragonFireBlastBase {
public:
    CCreatureAction_DragonFireBlastForward(CThingCreatureBase& creature,CThing& target) : CCreatureAction_DragonFireBlastBase(creature,target) {}
    virtual CCreatureActionBase* Clone() const;
#ifdef FABLE_RETAIL_JACKDRAGON_NATIVE_ACTION_SLOTS_PASS27
    virtual void Initialise();
#endif
};
#endif
#endif
