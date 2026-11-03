#ifndef FABLE_ENGINE_CTHINGFILTER_HASNAVIGATIONINFORMATION_H
#define FABLE_ENGINE_CTHINGFILTER_HASNAVIGATIONINFORMATION_H
// Original Ego_r TPI: empty, unary_function<CThingPhysical,bool> base (the non-POD base keeps VC7.1 from zero-filling the
// value-initialised temporary at 0x00509A20). operator() is out of line: Ego_r 0x004E0840
// ??RCThingFilter_HasNavigationInformation@@QBE_NABVCThingPhysical@@@Z. (2026-11-03, chunk 719 row 0x00509A20, agent719c.)
#include "stlport/fable_containers.h"
class CThingPhysical;
class CThingFilter_HasNavigationInformation : public std::unary_function<CThingPhysical,bool> {
public:
    bool operator()(const CThingPhysical& thing) const;
};
#endif
