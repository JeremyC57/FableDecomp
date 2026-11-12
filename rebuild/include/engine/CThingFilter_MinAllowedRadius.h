#ifndef FABLE_CTHINGFILTER_MINALLOWEDRADIUS_H
#define FABLE_CTHINGFILTER_MINALLOWEDRADIUS_H
// New header (Q18, row 0x006FE7C0): CThingFilter_MinAllowedRadius, Ego_r.pdb layout (4 bytes: float MinRadius +0) and name set
// (??0CThingFilter_MinAllowedRadius@@QAE@M@Z inline; ??RCThingFilter_MinAllowedRadius@@QBE_NABVCThing@@@Z out of line in retail at
// 0x006A7410, the call in the rough-area instance 0x006FE92E). Written like engine/CThingFilter_IsOfType.h (a unary_function filter).
#include "stlport/fable_containers.h"
class CThing;
class CThingFilter_MinAllowedRadius:public std::unary_function<CThing,bool> {
public:CThingFilter_MinAllowedRadius(float min_radius):MinRadius(min_radius){}bool operator()(const CThing&)const;
private:float MinRadius;
};
#endif
