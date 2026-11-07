#ifndef FABLE_ENGINE_CTHINGFILTER_OVERLAPPINGAREA_H
#define FABLE_ENGINE_CTHINGFILTER_OVERLAPPINGAREA_H
// CThingFilter_OverlappingArea (chunk 855, row 0x00642AA0): Ego_r.pdb 4 bytes, `const C2DBoxI* Area` +0 (built on the stack as the
// box address, 0x00642CAC `lea ecx,[esp+28h] ; mov [esp+10h],ecx`); the filter of the landed GetAllThings<CThingBuilding,
// vector<CThingBuilding*>, CThingFilter_OverlappingArea> 0x006437D0 (its test body is that row's, not needed by callers).
#include "stlport/fable_containers.h"
class CThing; class C2DBoxI;
class CThingFilter_OverlappingArea : public std::unary_function<CThing, bool> {
public:
    CThingFilter_OverlappingArea(const C2DBoxI* area) : Area(area) { }
    const C2DBoxI* Area;                                                 // +0x00
};
#endif
