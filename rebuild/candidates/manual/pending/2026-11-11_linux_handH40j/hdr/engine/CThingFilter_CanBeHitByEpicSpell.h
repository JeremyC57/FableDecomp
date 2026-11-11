#ifndef FABLE_ENGINE_CTHINGFILTER_CANBEHITBYEPICSPELL_H
#define FABLE_ENGINE_CTHINGFILTER_CANBEHITBYEPICSPELL_H
// New header (chunk H40i, row 0x0078FD60 CTCDivineWrath::FrameUpdate): the empty thing filter of the epic spells. Ego_r.pdb
// ??RCThingFilter_CanBeHitByEpicSpell@@QBE_NABVCThing@@@Z (Ego_r 0x005A4D70), out of line; Ego_r instantiates
// GetAllThingsInRoughArea with CFilter_Bind2<CThingFilter_OverlappingCircle / CThingFilter_In2DRange / CThingFilter_In3DRange,
// CThingFilter_CanBeHitByEpicSpell> (0x00798FE0, 0x00799240, 0x00794AE0 ...). Retail FrameUpdate keeps the empty object as one
// stack byte ([esp+13h], 0x0078FFBB). Declaration only.
#include "stlport/fable_containers.h"
class CThing;
class CThingFilter_CanBeHitByEpicSpell : public std::unary_function<CThing,bool> {
public:
    bool operator()(const CThing& thing) const;
};
#endif
