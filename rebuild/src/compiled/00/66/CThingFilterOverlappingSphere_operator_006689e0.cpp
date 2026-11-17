// CThingFilter_OverlappingSphere::operator()(const CThing& thing) const -- true when the thing's physics bounding sphere overlaps the filter sphere (SpherePos, Radius)
// Leaf: ??RCThingFilter_OverlappingSphere@@QBE_NABVCThing@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006689E0, length 107 bytes (+ int3 padding).
// Callees: CTCPhysicsBase::GetBoundingInfo(C3DVector*, float*) @ 0x006AF640 (landed).
// Layout: PDB CThingFilter_OverlappingSphere { C3DVector SpherePos (+0); float Radius (+0xc); }; retail CThing PhysicsTC +0x60
// (no null test: the filter is only run on physical things).
// Shape: the bounding centre is a C3DVector local, the bounding radius lands in the dead thing argument slot; the deltas
// are thing minus SpherePos, the radii summed as `boundRadius + Radius`; `if (d2 < r*r) return true; return false;`
// gives the byte-constant exits.
// NF59 (round 7, queue q10522): the filter is engine/CThingFilter_OverlappingSphere.h (NF59 opt-in FABLE_RETAIL_CTHINGFILTER_OVERLAPPINGSPHERE_OUTOFLINE:
//   Ego_r.pdb 0x005451C0 ??RCThingFilter_OverlappingSphere@@QBE_NABVCThing@@@Z, the const CThing& overload its landed caller 0x00785DD0 binds)
//   and the physics component the retail engine/CTCPhysicsBase.h (?GetBoundingInfo@CTCPhysicsBase@@QBEXAAVC3DVector@@AAM@Z, Ego_r.pdb
//   0x006EC5B0); the local filter (CThing* overload) and CTCPhysicsBase (pointer-parameter, non-const GetBoundingInfo) models are gone.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHINGFILTER_OVERLAPPINGSPHERE_OUTOFLINE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThing.h"
#include "engine/CThingFilter_OverlappingSphere.h"

bool CThingFilter_OverlappingSphere::operator()(const CThing& thing) const
{
    C3DVector centre;
    float boundRadius;
    thing.GetPhysicsTC()->GetBoundingInfo(centre, boundRadius);
    float dx = centre.X - SpherePos.X;
    float dy = centre.Y - SpherePos.Y;
    float dz = centre.Z - SpherePos.Z;
    float r = boundRadius + Radius;
    if (dx * dx + dy * dy + dz * dz < r * r)
        return true;
    return false;
}