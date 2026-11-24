#ifndef FABLE_ENGINE_CTHINGFILTER_HASNAVIGATIONINFORMATION_H
#define FABLE_ENGINE_CTHINGFILTER_HASNAVIGATIONINFORMATION_H
#ifdef FABLE_RETAIL_MAP_CACHED_VOLUMES_PASS26
#include "stlport/fable_containers.h"
class CThingPhysical;
// Ego_r type 35ab2 and the FableWin predicate identify its unary base and argument.
class CThingFilter_HasNavigationInformation : public std::unary_function<CThingPhysical,bool> {
public:
    bool operator()(const CThingPhysical&) const;
};
#endif
#endif
