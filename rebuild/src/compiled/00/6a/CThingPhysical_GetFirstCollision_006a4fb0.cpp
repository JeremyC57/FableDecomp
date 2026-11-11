// CThingPhysical::GetFirstCollision(float radius, const C3DVector& from, const C3DVector& delta, C3DVector& hit_position, C3DVector& hit_normal) const -- sweep a sphere along delta: through the physics component's mesh object when it has mesh info (the end point pushed out 100x), else against the component's own sphere (closest point on the segment; hit point from + delta * t, normal the fast-normalised offset from the centre)
// Leaf: ?GetFirstCollision@CThingPhysical@@QBE_NMABVC3DVector@@0AAV2@1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A4FB0, 619 bytes, __fastcall, `ret 14h`. Ego_r.pdb
//   ?GetFirstCollision@CThingPhysical@@QBE_NMABVC3DVector@@0AAV2@1@Z.
// Callees: CTCPhysicsBase::GetPhysicsMeshInfo() const (virtual, +0x174), CTCPhysicsBase::GetPhysicsMeshObject() const 0x006B0850
//   (by value), CPhysicsMeshObject::GetSphereIntersectionFromTo(float, const C3DVector&, const C3DVector&, CMeshIntersectionData&)
//   const 0x00A120D0, ~CPhysicsMeshObject 0x00A127D0, GFGetShortestDistanceSquaredBetweenPointAndLine 0x00A442E0 (__fastcall,
//   the squared distance in st0, the segment parameter through the reference).
// Shape: one result bool (retail keeps it in bl: `xor bl,bl` at entry, every false exit `mov al,bl`); C3DVector::Normalise is
//   the fast GFFastOneOverSqrt expansion (table 0x013CE558).
// Header overlay (hdr/): FABLE_RETAIL_CTHINGPHYSICAL_FIRSTCOLLISION and FABLE_RETAIL_CPHYSICSMESHOBJECT_SPHEREINTERSECTION (declarations),
//   FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_GROUPED (engine/C3DVector.h): the Newton step fully grouped, which
//   puts the epilogue pops among the final Normalise multiplies as retail (0x006A51F1..0x006A51F6).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER      // Normalise loads X, Y, Z (0x006A5181..0x006A5186)
#define FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_GROUPED
#define FABLE_RETAIL_CTHINGPHYSICAL_FIRSTCOLLISION
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_SPHEREINTERSECTION
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingPhysical.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CMeshIntersectionData.h"

float __fastcall GFGetShortestDistanceSquaredBetweenPointAndLine(const C3DVector& start, const C3DVector& end,
                                                                 const C3DVector& point, float& t);   // 0x00A442E0

bool CThingPhysical::GetFirstCollision(float radius, const C3DVector& from, const C3DVector& delta, C3DVector& hit_position,
                                       C3DVector& hit_normal) const
{
    bool result = false;
    if (GetPhysicsTC()) {
        if (GetPhysicsTC()->GetPhysicsMeshInfo()) {
            CPhysicsMeshObject mesh_object = GetPhysicsTC()->GetPhysicsMeshObject();
            CMeshIntersectionData intersection;
            result = mesh_object.GetSphereIntersectionFromTo(radius, from, delta * 100.0f + from, intersection);
            hit_position = intersection.IntersectionPosition;
            hit_normal = intersection.Normal;
        } else {
            float t;
            float distance = (float)sqrt(GFGetShortestDistanceSquaredBetweenPointAndLine(from, from + delta, GetPhysicsTC()->GetPosition(), t));
            if (distance < radius + GetPhysicsTC()->GetRadius()) {
                hit_position = delta * t + from;
                hit_normal = hit_position - GetPhysicsTC()->GetPosition();
                hit_normal.Normalise();
                result = true;
            }
        }
    }
    return result;
}