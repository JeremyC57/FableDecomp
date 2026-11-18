#ifndef FABLE_ENGINE_CTHINGFILTER_ISPERCEIVINGTHING_H
#define FABLE_ENGINE_CTHINGFILTER_ISPERCEIVINGTHING_H
#ifdef FABLE_RETAIL_CTHINGFILTER_ISPERCEIVINGTHING
#include "stlport/fable_containers.h"
class CThing;
class CThingFilter_IsPerceivingThing : public std::unary_function<CThing, bool> {
    const CThing& Thing;
public:
    explicit CThingFilter_IsPerceivingThing(const CThing& thing) : Thing(thing) {}
    bool operator()(const CThing&) const;
};
FABLE_STATIC_ASSERT(sizeof(CThingFilter_IsPerceivingThing) == 4);
#endif
#endif
