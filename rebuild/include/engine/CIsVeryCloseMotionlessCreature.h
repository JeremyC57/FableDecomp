#ifndef FABLE_ENGINE_CISVERYCLOSEMOTIONLESSCREATURE_H
#define FABLE_ENGINE_CISVERYCLOSEMOTIONLESSCREATURE_H
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
// Layout: both PDB types, original constructor/00942230 and predicate/00942270.
class CIsVeryCloseMotionlessCreature:public std::unary_function<CThingCreatureBase,bool>{
 C3DVector MyPos;float MyRadius;
public:
 CIsVeryCloseMotionlessCreature(CThingAICreature&);
 bool operator()(const CThingCreatureBase&)const;
};
#endif
