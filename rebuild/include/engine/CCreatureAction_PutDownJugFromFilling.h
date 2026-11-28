#ifndef FABLE_ENGINE_CCREATUREACTION_PUTDOWNJUGFROMFILLING_H
#define FABLE_ENGINE_CCREATUREACTION_PUTDOWNJUGFROMFILLING_H
#ifdef FABLE_RETAIL_TAVERN_FILL_JUG_PASS30
// Ego_r and FableWin give the derived class and PThingToPutDown. The retail constructor at
// 0x008441D0 places that intelligent pointer at +0xA8, after CCreatureActionBase.
#include "engine/CCreatureActionBase.h"
#include "engine/CIntelligentPointer.h"

class CCreatureAction_PutDownJugFromFilling : public CCreatureActionBase {
public:
    CCreatureAction_PutDownJugFromFilling(CThingCreatureBase&, CThing&);
    virtual CCreatureActionBase* Clone() const;
    virtual void PerformAction();
private:
    CIntelligentPointer<CThing> PThingToPutDown;
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_PutDownJugFromFilling) == 0xb0);
#endif
#endif
