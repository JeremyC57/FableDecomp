#ifndef FABLE_ENGINE_HASGUARDPROPERTY_H
#define FABLE_ENGINE_HASGUARDPROPERTY_H
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CThingAICreature;
// Ego_rTPI38e08/FableWin3d043: empty public unary_function base,
// exact by-value CIntelligentPointer<CThingAICreature> argument, const operator.
struct HasGuardProperty : public std::unary_function<CIntelligentPointer<CThingAICreature>,bool> {
 bool operator()(CIntelligentPointer<CThingAICreature>)const;
};
#endif
