#ifndef FABLE_ENGINE_CCOMBATACTION_GENERICSTRIKERESPONSE_H
#define FABLE_ENGINE_CCOMBATACTION_GENERICSTRIKERESPONSE_H
#include "engine/CCombatAction_GenericStrikeResponseBase.h"
// Original TPI0x3bfb3, no additional members; native constructor00867990.
class CCombatAction_GenericStrikeResponse : public CCombatAction_GenericStrikeResponseBase {
public:
    CCombatAction_GenericStrikeResponse(CThingCreatureBase&,const CThing*,const C3DVector&,const C3DVector&);
private: virtual CCreatureActionBase* Clone() const;
};
FABLE_STATIC_ASSERT(sizeof(CCombatAction_GenericStrikeResponse)==0x148);
#endif
