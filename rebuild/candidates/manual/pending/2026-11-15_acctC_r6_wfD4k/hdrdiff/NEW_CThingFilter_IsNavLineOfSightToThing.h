#ifndef FABLE_ENGINE_CTHINGFILTER_ISNAVLINEOFSIGHTTOTHING_H
#define FABLE_ENGINE_CTHINGFILTER_ISNAVLINEOFSIGHTTOTHING_H
// New header (wfD4, row 0x0090D030 CAIStateGroup_MinionGossip::Update). Ego_r TPI CThingFilter_IsNavLineOfSightToThing, 8 bytes:
// Pos (C3DVector*, +0x00) and PTCNav (CTCCreatureNavigation*, +0x04) (struct_layouts_egor.tsv). Ego_r.pdb / FableWin.pdb
// ??0CThingFilter_IsNavLineOfSightToThing@@QAE@ABVCThing@@@Z (landed 0x006A8710, `ret 4`) and
// ??RCThingFilter_IsNavLineOfSightToThing@@QBE_NABVCThing@@@Z (landed 0x006A7640). Declarations only.
#include "stlport/fable_containers.h"
class CThing;
class C3DVector;
class CTCCreatureNavigation;
class CThingFilter_IsNavLineOfSightToThing : public std::unary_function<CThing,bool> {
public:
    CThingFilter_IsNavLineOfSightToThing(const CThing& thing);       // 0x006A8710
    bool operator()(const CThing& thing) const;                       // 0x006A7640
private:
    const C3DVector* Pos;                                             // +0x00
    CTCCreatureNavigation* PTCNav;                                    // +0x04
};
#endif
