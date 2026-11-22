#ifndef FABLE_ENGINE_CCREATUREACTION_DRAGONFIREBLASTLEFT_H
#define FABLE_ENGINE_CCREATUREACTION_DRAGONFIREBLASTLEFT_H
#ifdef FABLE_RETAIL_JACKDRAGON_HOVER_PASS27
#include "engine/CCreatureAction_DragonFireBlastBase.h"
class CCreatureAction_DragonFireBlastLeft : public CCreatureAction_DragonFireBlastBase {
public:
    CCreatureAction_DragonFireBlastLeft(CThingCreatureBase& creature,CThing& target);
    virtual CCreatureActionBase* Clone() const;
};
#endif
#endif
