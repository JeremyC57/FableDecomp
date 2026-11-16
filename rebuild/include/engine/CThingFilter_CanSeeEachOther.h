#ifndef FABLE_ENGINE_CTHINGFILTER_CANSEEEACHOTHER_H
#define FABLE_ENGINE_CTHINGFILTER_CANSEEEACHOTHER_H
#ifdef FABLE_RETAIL_KID_HURT_SELF_PASS23
#include "stlport/fable_containers.h"
class CThingCreatureBase;
// Both original PDB complete types; native006a78d0 consumes const object references.
class CThingFilter_CanSeeEachOther : public std::unary_function<CThingCreatureBase,bool> {
public:
    CThingFilter_CanSeeEachOther(const CThingCreatureBase& creature):Creature(creature){}
    bool operator()(const CThingCreatureBase&)const;
private:
    const CThingCreatureBase& Creature;
};
FABLE_STATIC_ASSERT(sizeof(CThingFilter_CanSeeEachOther)==4);
#endif
#endif
