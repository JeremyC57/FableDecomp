#ifndef FABLE_ENGINE_CCOMBATACTION_GENERICRESPONSETODRAIN_H
#define FABLE_ENGINE_CCOMBATACTION_GENERICRESPONSETODRAIN_H
#include "engine/CCombatActionBase.h"
// Original 0x3c04f; native constructor keeps the complete0x114 base.
class CCombatAction_GenericResponseToDrain : public CCombatActionBase {
public: CCombatAction_GenericResponseToDrain(CThingCreatureBase&,CThing&);
    virtual CCreatureActionBase* Clone() const;
};
FABLE_STATIC_ASSERT(sizeof(CCombatAction_GenericResponseToDrain)==0x114);
#endif
