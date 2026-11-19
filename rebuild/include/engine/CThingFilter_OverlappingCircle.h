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
#ifdef FABLE_RETAIL_CTHINGFILTER_OVERLAPPINGCIRCLE_BODY
// Opt-in (hand4c 2026-11-19, row 0x0063FD40 GetAllThingsInRoughArea<CThing, list<CThing*>, CFilter_Bind2<CThingFilter_OverlappingCircle,
// CWorldMap::CThingFilterBase> >): the operator() body, visible to every includer (an inline header function: Ego_r.pdb keeps its out-of-line
// copy in thing_creature_base.obj while the world_map_tools.obj instance 0x0063FD40 and the thing_creature_base.obj ForEachInRoughArea
// 0x0066A1E0 expand it; retail's COMDAT copy is 0x0063EF50). Text: FableWin.pdb ??RCThingFilter_OverlappingCircle@@QBE_NABVCThing@@@Z
// (debug 0x01DAEBE0, 114 bytes: GetPhysicsTC, GetMapwhoPosition - SpherePos, GetSquaredMagnitude2D, GetRadius + Radius, GFSqr, one `xor al,al`
// exit) with Ego_d.pdb's one local ptc_physics. The includer must see CThing::GetPhysicsTC, a public CTCPhysicsBase::GetMapwhoPosition
// declaration and GetRadius, and C3DVector's GFSqr (FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER) and one-expression GetSquaredMagnitude2D.
// Measured: the out-of-line copy this text emits squares dy first where retail's 0x0063EF50 squares dx first (0x0063EF7E..84); every
// spelling of the sum (six) and of the body (five) flips the inline and out-of-line sites together, so the split is c2 context, not text.
inline bool CThingFilter_OverlappingCircle::operator()(const CThing& test_thing) const
{
    const CTCPhysicsBase* ptc_physics = test_thing.GetPhysicsTC();
    if (ptc_physics && (ptc_physics->GetMapwhoPosition() - SpherePos).GetSquaredMagnitude2D() < GFSqr(ptc_physics->GetRadius() + Radius))
        return true;
    return false;
}
#endif
#endif
