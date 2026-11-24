// CThingFilter_RadiusIntersectsInfiniteLine::operator()(const CThing& thing) const -- true when the thing's physics bounding sphere reaches the filter's infinite line
// Leaf: ??RCThingFilter_RadiusIntersectsInfiniteLine@@QBE_NABVCThing@@@Z
// Name fix (NF382, QUEUE q2): was CThingFilter_BoundingRadiusIntersectsInfiniteLine::operator(). Retail calls this address from
//   0x007466D4 in the landed CThingSearchTools::GetAllThingsInRoughArea<..., CFilter_Bind3<..., CThingFilter_RadiusIntersectsInfiniteLine,
//   ...>> 0x00746560 (table witness for ??RCThingFilter_RadiusIntersectsInfiniteLine) and from 0x00745EF8 (no catalog row); the filter
//   is built by the landed CTCPerceiveThing::UpdateLineOfSight 0x00745120 (engine/CThingFilter_RadiusIntersectsInfiniteLine.h), whose TU
//   also names it. Ego_r.pdb 0x005A3220 / FableWin.pdb 0x01F932C0 define it. The BoundingRadius variant is a separate FableWin
//   function (edit_control_centre); nothing in retail binds that name to this address. Same bytes.
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7050, length 89 bytes (+ int3 padding), thiscall `ret 4`.
// Callees: CTCPhysicsBase::GetBoundingInfo(C3DVector& centre, float& radius) const @ 0x006AF640, the squared
//   point-to-infinite-line distance helper @ 0x00A442E0 (__fastcall: ecx = line start, edx = line end, then the
//   point and a float& line parameter on the stack, result on st0, `ret 8`; unnamed in the PDBs, the catalog
//   says "Vector3_ClosestPointOnLineSegment" -- the name below is descriptive).
// Identity: see the name fix above. The catalog's "CThingFilter_CheckBoundingIntersection" is a descriptive auto label.
// Layout: filter = {Start, End} C3DVectors (Ego_r, 24 bytes) from the shared engine/CThingFilter_RadiusIntersectsInfiniteLine.h.
// Shape: no physics component -> false; else the bounding radius is returned into the dead `thing` argument slot,
//   the centre and the line parameter are frame locals, and the test is `distance_sq < radius * radius`
//   (`fld r ; fmul r ; fxch ; fcompp`), false on the `jp`. The distance must be a NAMED float local: the call
//   written inside the compare lets VC flip it to `r*r > d` (`test ah,41h ; jne`, no fxch: DIFFER 87v89).
#include "engine/C3DVector.h"

class CTCPhysicsBase {
public:
    void GetBoundingInfo(C3DVector& centre, float& radius) const;   // 0x006AF640
};

float __fastcall GFGetShortestDistanceSquaredBetweenPointAndLine(const C3DVector& start, const C3DVector& end,
                                                 const C3DVector& point, float& t);   // 0x00A442E0

#include "engine/CThing.h"

#include "engine/CThingFilter_RadiusIntersectsInfiniteLine.h"

bool CThingFilter_RadiusIntersectsInfiniteLine::operator()(const CThing& thing) const
{
    if (thing.GetPhysicsTC()) {
        C3DVector centre;
        float radius;
        thing.GetPhysicsTC()->GetBoundingInfo(centre, radius);
        float t;
        float distance_sq = GFGetShortestDistanceSquaredBetweenPointAndLine(Start, End, centre, t);
        if (distance_sq < radius * radius)
            return true;
    }
    return false;
}