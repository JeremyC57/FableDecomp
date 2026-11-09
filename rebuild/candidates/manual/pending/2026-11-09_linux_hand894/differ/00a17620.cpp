// CPhysicsMesh::HasBoundingSphereIntersectedFromTo(float radius, const C3DVector& from, const C3DVector& to) const -- true when the segment from..to passes within (bounding radius + radius + 15) of the bounding-sphere centre
// Grade: DIFFER(99v99) -- 7 lines: the double cast flips the reach sum to member-first (retail `fld [ecx+80h] ; fadd [esp+10h]`) but the x87 loads move one scheduler slot
// Chunk 894 (32 grades): float spellings (17) all keep param-first fadd; double-cast spellings (12) flip to member-first and shift one slot; all-double (4 lines) needs a qword 15.0, and retail uses the shared 15.0f @ 0x0124E640 (50 refs). Two more float forms (two statements; const float& to the member) also load the param first. hand892 sched.sh dump of the double-cast form: the cast adds a no-code 0x269 tuple between the two fadds, which delays `fadd 15.0f` three cycles; retail fits the plain float DAG with member-first order, so the open lever is the commutative operand choice (candidate-id rule), not scheduling. Next: c2 operand-order probe on the fadd tuple (which operand c2 treats as the higher candidate id).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A17620, 99 bytes, __thiscall `ret 0Ch`; callee GetSquaredDistanceToInfiniteLine @ 0x00A442E0 (__fastcall), 15.0f @ 0x0124E640.
// Leaf: ?HasBoundingSphereIntersectedFromTo@CPhysicsMesh@@QBE_NMABVC3DVector@@0@Z
#include "engine/C3DBoundingSphere.h"
float __fastcall GetSquaredDistanceToInfiniteLine(const C3DVector& start, const C3DVector& end, const C3DVector& point, float& segmentParameter);   // 0x00A442E0
class CPhysicsMesh {
public:
    unsigned char     _pad00[0x74];
    C3DBoundingSphere BoundingSphere;              // +0x74
    bool HasBoundingSphereIntersectedFromTo(float radius, const C3DVector& from, const C3DVector& to) const;
};
bool CPhysicsMesh::HasBoundingSphereIntersectedFromTo(float radius, const C3DVector& from, const C3DVector& to) const
{
    C3DVector centre = BoundingSphere.Center;
    radius = (float)((double)BoundingSphere.Radius + radius) + 15.0f;
    float segmentParameter;
    if (GetSquaredDistanceToInfiniteLine(from, to, centre, segmentParameter) <= radius * radius)
        return true;
    return false;
}
