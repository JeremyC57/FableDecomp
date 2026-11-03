#ifndef FABLE_ENGINE_CTHINGFILTER_OVERLAPPINGAREA_H
#define FABLE_ENGINE_CTHINGFILTER_OVERLAPPINGAREA_H
// Original Ego_r TPI: unary_function<CThing,bool> base, one const C2DBoxI* Area (size 4).
// operator() is out of line: Ego_r 0x004E0870 ??RCThingFilter_OverlappingArea@@QBE_NABVCThing@@@Z.
// (2026-11-03, chunk 719 row 0x00509A20, agent719c.)
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/C2DBoxI.h"
class CThing;
class CThingFilter_OverlappingArea : public std::unary_function<CThing,bool> {
public:
    CThingFilter_OverlappingArea(const C2DBoxI& area):Area(&area){}
    bool operator()(const CThing& thing) const;
protected:
    const C2DBoxI* Area;
};
FABLE_STATIC_ASSERT(sizeof(CThingFilter_OverlappingArea)==4);
#endif
