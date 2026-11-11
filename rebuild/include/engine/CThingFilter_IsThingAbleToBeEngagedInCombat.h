#ifndef FABLE_ENGINE_CTHINGFILTER_ISTHINGABLETOBEENGAGEDINCOMBAT_H
#define FABLE_ENGINE_CTHINGFILTER_ISTHINGABLETOBEENGAGEDINCOMBAT_H
// Original complete PDB records in both Ego_r and FableWin; native007f6200 layout/calls.
#include "stlport/fable_containers.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
class CThing;class CThingCreatureBase;class CThingAICreature;
class CThingFilter_IsThingAbleToBeEngagedInCombat : public std::unary_function<CThing,bool> {
public:
 bool operator()(CThing const & p0) const;
};
#endif
