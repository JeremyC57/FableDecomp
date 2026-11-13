#ifndef FABLE_ENGINE_CTHINGFILTER_RADIUSINTERSECTSINFINITELINE_H
#define FABLE_ENGINE_CTHINGFILTER_RADIUSINTERSECTSINFINITELINE_H
// New header (chunk 2009, row 0x00745120 CTCPerceiveThing::UpdateLineOfSight). Ego_r.pdb TPI: C3DVector Start +0, C3DVector End +0xC
// (sizeof 24, struct_layouts_egor.tsv); ??0CThingFilter_RadiusIntersectsInfiniteLine@@QAE@ABVC3DVector@@0@Z (FableWin.pdb 0x0203FC10,
// inline in retail: the six dword copies 0x00745493..0x007454CC) and ??RCThingFilter_RadiusIntersectsInfiniteLine@@QBE_NABVCThing@@@Z
// (Ego_r.pdb 0x005A3220 / FableWin.pdb 0x01F932C0; retail 0x006A7050, called by the landed search 0x00746560).
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
class CThing;
class CThingFilter_RadiusIntersectsInfiniteLine : public std::unary_function<CThing,bool> {
public:
 CThingFilter_RadiusIntersectsInfiniteLine(const C3DVector& start,const C3DVector& end) : Start(start), End(end) {}
 bool operator()(const CThing&) const;                                   // 0x006A7050
private:
 C3DVector Start;
 C3DVector End;
};
#endif
