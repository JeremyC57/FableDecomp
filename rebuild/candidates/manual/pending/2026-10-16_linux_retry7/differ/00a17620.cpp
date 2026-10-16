// CPhysicsMesh::HasBoundingSphereIntersectedFromTo(float radius, const C3DVector& from, const C3DVector& to) const -- true when the segment from..to passes within (bounding radius + radius + 15) of the bounding-sphere centre
// Grade: DIFFER(99v99) -- only the reach sum's operand order: retail `fld [ecx+80h] ; fadd [esp+10h]` (bounding radius loaded first), ours `fld [esp+10h] ; fadd [ecx+80h]` whichever way the sum is written; tried: R + radius + 15, radius + R + 15, memberwise C3DVector copy ctor (fixed the centre copy), named reach local | retry 7 (Fable): 3 tries: (1) `radius = BoundingSphere.Radius + radius + 15.0f` and `radius * radius` reproduces the store into the parameter slot (only the fld/fadd operand order is left); (2) the sum built by `+=` statements and (3) the member read into a local before the centre copy both still load the parameter first. The x87 operand order for member + parameter stays a lane gap.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A17620, length 99 bytes (+ int3 padding).
// Leaf: HasBoundingSphereIntersectedFromTo
// Callees: the point-to-segment squared distance @ 0x00A442E0 (`__fastcall`: segment start in ecx, end in edx, the point
// and the out segment parameter on the stack; catalogued "Vector3_ClosestPointOnLineSegment").
// Constant: 15.0f @ 0x0124E640.
// Layout: Ego_r.pdb CPhysicsMesh with the retail 12-byte containers: BoundingSphere +0x74 (Centre +0x74, Radius +0x80;
// PDB +0x90).
// Shape: the centre is copied into a 12-byte frame local; the reach (radius sum) is parked in the dead `radius`
// argument slot and the segment parameter in the dead `to` slot; `reach*reach` compared with the distance (`test ah,1 ;
// jne` false) with separate `mov al,1` / `xor al,al` exits; `ret 0Ch`.
struct C3DVector {
    float x, y, z;
    C3DVector(const C3DVector& other) : x(other.x), y(other.y), z(other.z) { }
};
struct C3DBoundingSphere {
    C3DVector Centre;                              // +0x00
    float     Radius;                              // +0x0c
};
float __fastcall GetDistanceSquaredToLineSegment(const C3DVector& start, const C3DVector& end, const C3DVector& point, float* segmentParameter);   // 0x00A442E0

struct CPhysicsMesh {
    unsigned char     _pad00[0x74];
    C3DBoundingSphere BoundingSphere;              // +0x74
    bool HasBoundingSphereIntersectedFromTo(float radius, const C3DVector& from, const C3DVector& to) const;
};

bool CPhysicsMesh::HasBoundingSphereIntersectedFromTo(float radius, const C3DVector& from, const C3DVector& to) const
{
    C3DVector centre = BoundingSphere.Centre;
    radius = BoundingSphere.Radius + radius + 15.0f;
    float segmentParameter;
    if (GetDistanceSquaredToLineSegment(from, to, centre, &segmentParameter) <= radius * radius)
        return true;
    return false;
}
