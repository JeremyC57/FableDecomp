#ifndef FABLE_ENGINE_CTHINGFILTER_OVERLAPPINGCIRCLE_H
#define FABLE_ENGINE_CTHINGFILTER_OVERLAPPINGCIRCLE_H
// CThingFilter_OverlappingCircle (chunk 1011, row 0x005EB590 CTCTavernGame::GivePrize): Ego_r.pdb layout (struct_layouts_egor.tsv,
// sizeof 0x10): C3DVector SpherePos +0x00, float Radius +0x0C; GivePrize builds it in place from GetBoundingInfo's centre / radius
// (0x005EB76E..0x005EB796). ??RCThingFilter_OverlappingCircle@@QBE_NABVCThing@@@Z is out of line (retail 0x0063EF50, called by the
// landed GetAllThings<CThingAICreature, list, CThingFilter_OverlappingCircle> 0x005ED7D0). The constructor is inline (no Ego_r copy).
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
class CThing;
class CThingFilter_OverlappingCircle : public std::unary_function<CThing,bool> {
private:
    C3DVector SpherePos;                                                 // +0x00
    float Radius;                                                        // +0x0C
public:
    CThingFilter_OverlappingCircle(const C3DVector& pos, float radius) : SpherePos(pos), Radius(radius) {}
    bool operator()(const CThing& test_thing) const;                     // 0x0063EF50
};
#endif
