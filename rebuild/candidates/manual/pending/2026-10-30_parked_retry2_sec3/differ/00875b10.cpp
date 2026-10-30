// CTools::TestMeshForOccupationAndGetType(CPhysicsMeshObject* mesh, const C3DVector& position, const C3DVector& up, float* height) -- probe the mesh around position: a vertical segment up and down (both must hit; the hit height goes to *height), then four segments rotating around the up axis; type 1 when five or six probes hit, else 0
// Grade: DIFFER(771v838) -- 208 difflines (parked 281): __forceinline memberwise C3DVector copy ctor + Normalise taking its vector BY VALUE (retail copies up into x87 registers with Z through an int slot). Residue: the extent length-squared sum (retail loads both returned vectors onto the x87 stack with b.X through the dead mesh-arg slot), frame 0x68 v 0x78. Tried: named extent copies (790/310, 844/226)
// Leaf: TestMeshForOccupationAndGetType
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00875B10, length 838 bytes (`ret 0x10`).
// Callees: two CPhysicsMeshObject getters 0x00A11C20 / 0x00A11C90 (C3DVector by hidden pointer; the manifest's
//   CEngineInternalPrimitiveParticleLine labels are propagated), CPhysicsMeshObject::GetPointIntersectionFromTo 0x00A12450,
//   __ftol2. Constants: the fast inverse-sqrt table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 0.0f 0x0122DEDC,
//   0.25f 0x0123078C, 2*pi (double) 0x0124F2B8.
#include <math.h>
#pragma intrinsic(sin, cos)
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    __forceinline float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    __forceinline C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
};
__forceinline C3DVector operator*(const C3DVector& v, const float& s) { return C3DVector(v.X * s, v.Y * s, v.Z * s); }
__forceinline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
__forceinline C3DVector Normalise(C3DVector v) { return v * FastInvSqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z); }
struct CPhysicsMeshObject {
    C3DVector GetExtentA() const;                                   // 0x00A11C20
    C3DVector GetExtentB() const;                                   // 0x00A11C90
    bool GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, C3DVector& intersection) const;   // 0x00A12450
};
enum EMeshType { MESH_TYPE_NONE = 0, MESH_TYPE_OCCUPIED = 1 };
struct CTools {
    EMeshType TestMeshForOccupationAndGetType(CPhysicsMeshObject* mesh, const C3DVector& position, const C3DVector& up, float* height);
};

EMeshType CTools::TestMeshForOccupationAndGetType(CPhysicsMeshObject* mesh, const C3DVector& position, const C3DVector& up, float* height)
{
    float size = mesh->GetExtentA().GetLengthSquared() + mesh->GetExtentB().GetLengthSquared();
    C3DVector normal = Normalise(up);
    C3DVector side = Normalise(C3DVector(normal.Y, -normal.X, 0.0f));
    normal *= size;
    side *= size;
    long hits = 0;
    C3DVector intersection;
    for (long i = 0; i < 2; ++i) {
        long direction = (long)((0.5f - (float)i) * 2.0f);
        C3DVector to(position.X, position.Y, position.Z + (float)direction * size * 3.0f);
        if (!mesh->GetPointIntersectionFromTo(position, to, intersection))
            return MESH_TYPE_NONE;
        *height = intersection.Z;
        ++hits;
    }
    for (long j = 0; j < 4; ++j) {
        double angle = (float)j * 0.25f * 6.283185307179586;
        C3DVector to = position + normal * (float)cos(angle) + side * (float)sin(angle);
        if (mesh->GetPointIntersectionFromTo(position, to, intersection))
            ++hits;
    }
    if (hits == 5 || hits == 6)
        return MESH_TYPE_OCCUPIED;
    return MESH_TYPE_NONE;
}
