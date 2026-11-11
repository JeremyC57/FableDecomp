#ifndef FABLE_ENGINE_CTHINGFILTER_ISAICREATUREWILLINGTOATTACK_H
#define FABLE_ENGINE_CTHINGFILTER_ISAICREATUREWILLINGTOATTACK_H
// Original complete PDB records in both Ego_r and FableWin; native007f6200 layout/calls.
#include "stlport/fable_containers.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
class CThing;class CThingCreatureBase;class CThingAICreature;
class CThingFilter_IsAICreatureWillingToAttack : public std::unary_function<CThingCreatureBase,bool> {
public:
 CThingFilter_IsAICreatureWillingToAttack(CThingAICreature const & p0);
 bool operator()(CThingCreatureBase const & p0) const;
private:
 CThingAICreature const & SourceCreature;
};
#endif
