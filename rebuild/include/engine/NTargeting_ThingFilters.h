#ifndef FABLE_ENGINE_NTARGETING_THINGFILTERS_H
#define FABLE_ENGINE_NTARGETING_THINGFILTERS_H
#include "stlport/fable_containers.h"
class CThing;class CThingCreatureBase;
// Layout: complete original Ego23f6f/FW25fb5,4 bytes.
class CThingFilter_IsInterestingToCreature:public std::unary_function<CThing,bool> {const CThingCreatureBase& SourceCreature;public:
 CThingFilter_IsInterestingToCreature(const CThingCreatureBase& creature):SourceCreature(creature){}
 bool operator()(const CThing&)const;
};
// Layout: complete original Ego23f78/FW25fbe,8 bytes.
class CThingFilter_IsInCreatureFOV:public std::unary_function<CThing,bool> {const CThingCreatureBase&SourceCreature;bool IncludeHeadAngle;public:
 CThingFilter_IsInCreatureFOV(const CThingCreatureBase& creature,bool head):SourceCreature(creature),IncludeHeadAngle(head){}
 bool operator()(const CThing&)const;
};
// Layout: original Ego29353/FW2b991 is empty,size1.
class CThingFilter_IsPerceptible:public std::unary_function<CThing,bool> {public:bool operator()(const CThing&)const;};
// Layout: complete original Ego29381/FW2ba33,8 bytes.
class CThingFilter_IsInSameVolumeContextAsThing {CThing*PThingInside;bool Override;public:
 CThingFilter_IsInSameVolumeContextAsThing(const CThing&,bool);
 bool operator()(const CThing&)const;
};
#endif
