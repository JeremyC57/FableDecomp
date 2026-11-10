#ifndef FABLE_ENGINE_CTHINGFILTER_MATCHING_TYPE_FACTION_NEARBY_H
#define FABLE_ENGINE_CTHINGFILTER_MATCHING_TYPE_FACTION_NEARBY_H
// Original Ego_r full type44; packed C3DVector gives OurPos+1.
#include "engine/C3DVector.h"
#include "engine/ECreatureType.h"
#include "stlport/fable_containers.h"
class CThingObject;class CThingAICreature;class CFaction;
class CThingFilter_IsAICreatureMatchingTypeAndFactionNearby:public std::unary_function<CThingObject,bool> {
public:
 bool RestrictAngle;
 C3DVector OurPos;
 float MaxSqDist;
 C3DVector SideVec;
 ECreatureType OurCreatureType;
 const CFaction* PFaction;
 float MinCosine;
 CThingFilter_IsAICreatureMatchingTypeAndFactionNearby(const CThingAICreature&,float,bool);
 bool operator()(const CThingAICreature&) const;
};
#endif
