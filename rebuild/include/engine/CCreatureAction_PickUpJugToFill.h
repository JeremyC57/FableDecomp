#ifndef FABLE_ENGINE_CCREATUREACTION_PICKUPJUGTOFILL_H
#define FABLE_ENGINE_CCREATUREACTION_PICKUPJUGTOFILL_H
#ifdef FABLE_RETAIL_TAVERN_FILL_JUG_PASS30
// Ego_r and FableWin give the derived class and PThingToPickUp. The retail constructor at
// 0x00844140 places that intelligent pointer at +0xA8, after CCreatureActionBase.
#include "engine/CCreatureActionBase.h"
#include "engine/CIntelligentPointer.h"

class CCreatureAction_PickUpJugToFill : public CCreatureActionBase {
public:
    CCreatureAction_PickUpJugToFill(CThingCreatureBase&, CThing&);
    virtual CCreatureActionBase* Clone() const;
    virtual void PerformAction();
private:
    CIntelligentPointer<CThing> PThingToPickUp;
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_PickUpJugToFill) == 0xb0);
#endif
#endif
