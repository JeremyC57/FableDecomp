// CThingPhysical::GetFirstCylinderCollisionBetweenPoints(float radius, const C3DVector& from, const C3DVector& to,
//   C3DVector& hit_position, C3DVector& hit_normal) const -- tests the segment from->to (a sphere of `radius`) against this
//   thing's upright collision cylinder: radius = the physics component's Radius (the combat radius for creatures, type flag 8),
//   bottom = up * MeshHeightOffset + GetPos(), top = up * MeshHeight + bottom (up = GetRHSet().GetUp()). The 2D closest approach
//   of the segment to the cylinder axis must lie inside the widened radius and between the segment ends; the hit point is
//   from + dir * length * t, the hit normal the offset from the bottom with its component along the axis removed (normalised);
//   it is a hit only when the hit point's Z lies between the ends.
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A5610, 818 bytes, thiscall, `ret 14h`. Ego_r.pdb / FableWin.pdb
//   ?GetFirstCylinderCollisionBetweenPoints@CThingPhysical@@QBE_NMABVC3DVector@@0AAV2@1@Z.
// Callees: CThingCreatureBase::GetCombatRadius 0x00661F90, CTCPhysicsBase::GetRHSet() const (virtual +0x120, 0x006B1290),
//   CThing::GetPos 0x004C73D0 (twice; the second result unused), C3DVector::GetMagnitudeAndNormalise 0x00A14480,
//   GFGetShortestDistance2DSquaredBetweenPointAndLine 0x00A443A0 (__fastcall), C3DVector::AccurateNormalise 0x00A14440 (x2).
// Globals: 0.0001f 0x0129BA3C, 1.0f 0x0122DED8.
// Shape: the GF 2D helper's REAL landed body is visible to the compiler (inline __declspec(noinline)): only then does VC7.1 know
//   `t` does not escape and home it in the dead `to` argument slot (frame 0x58, not 0x5C). The nested if-blocks give retail's
//   frame packing (the up*MeshHeight temporary in delta's slots) and the shared `fstp st(0)` exit; Dot is the default one-line
//   X+Y+Z sum (the ZYX running-statement opt-in leaves the hit_normal pointer load late).
// Header overlay: FABLE_RETAIL_CTHINGPHYSICAL_CYLINDERCOLLISION (engine/CThingPhysical.h, declaration only).
// Leaf: ?GetFirstCylinderCollisionBetweenPoints@CThingPhysical@@QBE_NMABVC3DVector@@0AAV2@1@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR_TO2D
#define FABLE_RETAIL_C3DVECTOR_GETMAGNITUDEANDNORMALISE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGCREATUREBASE_COMBATRADIUS
#define FABLE_RETAIL_CTHINGPHYSICAL_CYLINDERCOLLISION
#include <math.h>
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CThingPhysical.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingCreatureBase.h"

inline __declspec(noinline) float __fastcall GFGetShortestDistance2DSquaredBetweenPointAndLine(const C2DVector& line_start,
    const C2DVector& line_end, const C2DVector& point, float& line_param)   // 0x00A443A0 (landed body)
{
    C2DVector dir = line_start - line_end;
    C2DVector to_start = line_start - point;
    float t = to_start.Dot(dir) / dir.Dot(dir);
    C2DVector offset = dir * t - to_start;
    line_param = t;
    return offset.GetSquaredMagnitude();
}

bool CThingPhysical::GetFirstCylinderCollisionBetweenPoints(float radius, const C3DVector& from, const C3DVector& to,
                                                            C3DVector& hit_position, C3DVector& hit_normal) const
{
    float cylinder_radius = GetPhysicsTC()->GetRadius();
    if (ThingTypeFlags & 8)
        cylinder_radius = ((const CThingCreatureBase*)this)->GetCombatRadius();
    C3DVector up = GetPhysicsTC()->GetRHSet().GetUp();
    C3DVector bottom = up * PBaseDef->MeshHeightOffset + GetPos();
    C3DVector top = up * PBaseDef->MeshHeight + bottom;
    float dx = from.X - to.X;
    float dy = from.Y - to.Y;
    if (dy * dy + dx * dx > 0.0001f) {
        C3DVector delta = to - from;
        float length = delta.GetMagnitudeAndNormalise();
        float t;
        float distance_squared = GFGetShortestDistance2DSquaredBetweenPointAndLine(from.ToC2DVector(), to.ToC2DVector(),
                                                                                  bottom.ToC2DVector(), t);
        radius += cylinder_radius;
        if (t * length > -radius && (t - 1.0f) * length < radius && distance_squared < radius * radius) {
            hit_position = delta * length * t + from;
            C3DVector offset = hit_position - bottom;
            C3DVector axis = top - bottom;
            axis.AccurateNormalise();
            float along = (float)fabs(axis.Dot(offset));
            hit_normal = offset - axis * along;
            hit_normal.AccurateNormalise();
            GetPos();
            if (hit_position.Z < top.Z && hit_position.Z > bottom.Z)
                return true;
        }
    }
    return false;
}