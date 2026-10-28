// CThingPhysical::GetFirstCollisionBetweenPoints(float radius, const C3DVector& from, const C3DVector& to, C3DVector& hitPosition, C3DVector& hitNormal) const -- sweep a sphere from `from` to `to`: through the physics component's mesh object when it has one, else through GetFirstCollision along the delta, accepting a hit only ahead of `from` and nearer than `to`
// Leaf: GetFirstCollisionBetweenPoints
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A5460, length 425 bytes (`ret 14h`). Mangled (Ego_r.pdb):
//   ?GetFirstCollisionBetweenPoints@CThingPhysical@@QBE_NMABVC3DVector@@0AAV2@1@Z.
// Callees: C3DVector length 0x00A14480 (manifest "C3DVector::GetScaled" is a propagated label; it returns the float length in st0);
//   the physics component's slot 93 (+0x174) and CTCPhysicsBase::GetPhysicsMeshObject 0x006B0850 (by value);
//   CPhysicsMeshObject::GetSphereIntersectionFromTo 0x00A120D0 and ~CPhysicsMeshObject 0x00A127D0; CThingPhysical::GetFirstCollision
//   0x006A4FB0.
// Data: 0.0001f 0x0129BA3C, 0.0f 0x0122DEDC.
// Layout: CThingPhysical physics component +0x60 (retail CThing view; local projection). The hit record is 0x1c bytes (position,
//   normal, one more dword: the 0x40-byte mesh object starts 0x1c above it); the mesh object is 0x40 bytes.
// Shape: the physics component goes through an inline getter (each use loads straight into ecx); the hit test recomputes
//   `hitPosition - from` in both the dot product and the length (VC7.1 CSEs it, spilling Y/Z into dead argument slots and keeping a
//   copy of X on the x87 stack), with `dot >= 0 && length < limit` as one condition; the length limit is `!(length > 0.0001f)`.
#include <math.h>
#pragma intrinsic(sqrt)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetMagnitudeAndNormalise();                                // 0x00A14480 ?GetMagnitudeAndNormalise@C3DVector@@QAEMXZ (normalises in place, returns the old magnitude)
    float GetLengthInline() const { return (float)sqrt(X * X + (Y * Y + Z * Z)); }
};
__forceinline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.Z * b.Z + a.X * b.X + a.Y * b.Y; }
struct CPhysicsHit {
    C3DVector Position;
    C3DVector Normal;
    float     Time;
};
class CPhysicsMeshObject { public:
    unsigned char Data[0x40 - 4];
    virtual ~CPhysicsMeshObject();                                  // 0x00A127D0
    bool GetSphereIntersectionFromTo(float radius, const C3DVector& from, const C3DVector& to, CPhysicsHit& hit) const;   // 0x00A120D0
};
class CTCPhysicsBase { public:
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6) VF10(7) VF10(8)
    VF(90) VF(91) VF(92)
    virtual void* GetPhysicsMesh() const;                   // slot 93 (+0x174)
    CPhysicsMeshObject GetPhysicsMeshObject() const;        // 0x006B0850
};
class CThingPhysical { public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                              // +0x60
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    bool GetFirstCollision(float radius, const C3DVector& from, const C3DVector& delta, C3DVector& hitPosition, C3DVector& hitNormal) const;   // 0x006A4FB0
    bool GetFirstCollisionBetweenPoints(float radius, const C3DVector& from, const C3DVector& to, C3DVector& hitPosition, C3DVector& hitNormal) const;
};

bool CThingPhysical::GetFirstCollisionBetweenPoints(float radius, const C3DVector& from, const C3DVector& to, C3DVector& hitPosition, C3DVector& hitNormal) const
{
    C3DVector delta = to - from;
    float length = delta.GetMagnitudeAndNormalise();
    if (!(length > 0.0001f))
        return false;
    if (GetPhysicsTC() && GetPhysicsTC()->GetPhysicsMesh()) {
        CPhysicsMeshObject meshObject = GetPhysicsTC()->GetPhysicsMeshObject();
        CPhysicsHit hit;
        if (meshObject.GetSphereIntersectionFromTo(radius, from, to, hit)) {
            hitPosition = hit.Position;
            hitNormal = hit.Normal;
            return true;
        }
        return false;
    }
    if (!GetFirstCollision(radius, from, delta, hitPosition, hitNormal))
        return false;
    if (DotProduct(delta, hitPosition - from) >= 0.0f && (hitPosition - from).GetLengthInline() < length)
        return true;
    return false;
}