// Grade: DIFFER(619v619) 13 difflines: one exit (nested ifs, result = true on the sphere hit) keeps the bool in bl like retail; residue only the epilogue pops interleaved with the final Normalise fmul/fstp at different points (retry wave 2 sec2b)
// Was: DIFFER(618v619) -- the bool result: retail keeps it in bl (xor bl,bl at entry, ebp = this) and every false exit returns mov al,bl, the null-component exit sharing the mesh path's epilogue; ours constant-folds the false exits (xor al,al, ebx = this), which shifts the frame by one word; the sphere test, fast Normalise and the mesh sweep otherwise match
// CThingPhysical::GetFirstCollision(float radius, const C3DVector& from, const C3DVector& delta, C3DVector& hitPosition,
// C3DVector& hitNormal) const -- sweep a sphere along `delta`: through the physics component's mesh object when it has
// one (the end point pushed out by a small factor), else against the component's bounding sphere (closest point on the
// segment; the hit point is from + delta * t and the normal the fast-normalised offset from the sphere centre)
// Leaf: GetFirstCollision
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A4FB0, `ret 0x14`. Mangled (Ego_r.pdb):
//   ?GetFirstCollision@CThingPhysical@@QBE_NMABVC3DVector@@0AAV2@1@Z.
// Callees: the physics component's slot 93 (+0x174), CTCPhysicsBase::GetPhysicsMeshObject 0x006B0850 (by value),
//   CPhysicsMeshObject::GetSphereIntersectionFromTo 0x00A120D0 / ~CPhysicsMeshObject 0x00A127D0,
//   ClosestPointOnLineSegment 0x00A442E0 (__fastcall, returns the squared distance in st0).
// Globals: 100.0f 0x01230A08, the reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4,
//   0.5f 0x0122F59C.
// Layout: CThingPhysical physics component +0x60 (retail CThing view); CTCPhysicsBase position +0x0c, bounding radius
//   +0x34 (descriptive).
#include <math.h>
#pragma intrinsic(sqrt)
extern unsigned long g_InvSqrtMantissaTable[128];          // 0x013CE558
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = z * z + y * y + x * x;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
float __fastcall ClosestPointOnLineSegment(const C3DVector& start, const C3DVector& end, const C3DVector& point,
                                           float* t);      // 0x00A442E0
struct CPhysicsHit {
    C3DVector Position;
    C3DVector Normal;
    float     Time;
};
struct CPhysicsMeshObject {
    unsigned char Data[0x40];
    ~CPhysicsMeshObject();                                  // 0x00A127D0
    bool GetSphereIntersectionFromTo(float radius, const C3DVector& from, const C3DVector& to, CPhysicsHit& hit) const;   // 0x00A120D0
};
struct CTCPhysicsBase {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6) VF10(7) VF10(8)
    VF(90) VF(91) VF(92)
    virtual void* GetPhysicsMesh() const;                   // slot 93 (+0x174)
    CPhysicsMeshObject GetPhysicsMeshObject() const;        // 0x006B0850
};
struct CThingPhysical {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                              // +0x60
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    const struct SPhysicsSphere* GetSphere() const { return (const SPhysicsSphere*)PhysicsTC; }
    bool GetFirstCollision(float radius, const C3DVector& from, const C3DVector& delta, C3DVector& hitPosition, C3DVector& hitNormal) const;
};
struct SPhysicsSphere {
    unsigned char _pad00[0x0c];
    C3DVector     Centre;                                   // +0x0c
    unsigned char _pad18[0x34 - 0x18];
    float         Radius;                                   // +0x34
};

bool CThingPhysical::GetFirstCollision(float radius, const C3DVector& from, const C3DVector& delta, C3DVector& hitPosition, C3DVector& hitNormal) const
{
    bool result = false;
    if (GetPhysicsTC()) {
        if (GetPhysicsTC()->GetPhysicsMesh()) {
            CPhysicsMeshObject meshObject = GetPhysicsTC()->GetPhysicsMeshObject();
            CPhysicsHit hit;
            result = meshObject.GetSphereIntersectionFromTo(radius, from, delta * 100.0f + from, hit);
            hitPosition = hit.Position;
            hitNormal = hit.Normal;
        } else {
            float t;
            float distance = (float)sqrt(ClosestPointOnLineSegment(from, from + delta, GetSphere()->Centre, &t));
            if (distance < radius + GetSphere()->Radius) {
                hitPosition = delta * t + from;
                hitNormal = hitPosition - GetSphere()->Centre;
                hitNormal.Normalise();
                result = true;
            }
        }
    }
    return result;
}
