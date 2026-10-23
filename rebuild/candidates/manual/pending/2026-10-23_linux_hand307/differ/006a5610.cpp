// Grade: DIFFER(849v818) -- frame layout: retail packs the up axis, the 2D segment points and the normalised cylinder axis into one reused frame region (0x58 frame) and builds up*bottom/up*height into the slots later holding top; ours gives every vector its own slot (0x78 frame), so every x87 spill offset differs; the operation order (bottom/top, 2D length test, closest point, three widened-radius tests, hit point/normal, end-cap tests) matches
// CThingPhysical::GetFirstCylinderCollisionBetweenPoints(float radius, const C3DVector& from, const C3DVector& to,
// C3DVector& hitPosition, C3DVector& hitNormal) const -- tests the segment against this thing's upright collision
// cylinder (physics radius, or the combat radius for creatures; ends from the base def along the physics up axis): the
// closest approach of the 2D segment to the cylinder axis must be inside the widened radius and between the ends
// Leaf: GetFirstCylinderCollisionBetweenPoints
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A5610, `ret 0x14`.
// Callees: CThingCreatureBase::GetCombatRadius 0x00661F90, the physics component's virtual +0x120 (its up axis),
//   CThing::GetPosition 0x004C73D0, C3DVector::GetLength 0x00A14480, GetClosestPointOnLineSegment (2D, distance squared)
//   0x00A443A0 (__fastcall), C3DVector::Normalise 0x00A14440.
// Globals: 0.0001f 0x0129BA3C, 1.0f 0x0122DED8.
// Layout: CThing physics component +0x60, ThingTypeFlags +0x6c, PBaseDef +0x70; CTCPhysicsBase radius +0x34; the base
//   def's cylinder bottom/top offsets +0x58 / +0x5c (descriptive).
#include <math.h>
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}

    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetLength() const;                                // 0x00A14480
    void Normalise();                                       // 0x00A14440
};
__forceinline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.Z * b.Z + a.Y * b.Y + a.X * b.X; }
float __fastcall GetClosestPointOnLineSegment(const C2DVector& start, const C2DVector& end, const C2DVector& point,
                                              float* t);    // 0x00A443A0
struct CTCPhysicsBase {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    VF(70) VF(71)
    virtual const C3DVector& GetUp() const;                 // slot 72 (+0x120)
    unsigned char _pad04[0x34 - 0x04];
    float Radius;                                           // +0x34
};
struct CThingBaseDef {
    unsigned char _pad00[0x58];
    float CylinderBottom;                                   // +0x58
    float CylinderHeight;                                   // +0x5c
};
struct CThingPhysical {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                              // +0x60
    unsigned char _pad64[0x6c - 0x64];
    unsigned long ThingTypeFlags;                           // +0x6c
    const CThingBaseDef* PBaseDef;                          // +0x70
    const C3DVector& GetPosition() const;                   // 0x004C73D0
    bool GetFirstCylinderCollisionBetweenPoints(float radius, const C3DVector& from, const C3DVector& to, C3DVector& hitPosition, C3DVector& hitNormal) const;
};
struct CThingCreatureBase : CThingPhysical {
    float GetCombatRadius() const;                          // 0x00661F90
};

bool CThingPhysical::GetFirstCylinderCollisionBetweenPoints(float radius, const C3DVector& from, const C3DVector& to, C3DVector& hitPosition, C3DVector& hitNormal) const
{
    float cylinderRadius = PhysicsTC->Radius;
    if (ThingTypeFlags & 8)
        cylinderRadius = static_cast<const CThingCreatureBase*>(this)->GetCombatRadius();
    C3DVector up = PhysicsTC->GetUp();
    C3DVector bottom = up * PBaseDef->CylinderBottom + GetPosition();
    C3DVector top = up * PBaseDef->CylinderHeight + bottom;
    float dx = from.X - to.X;
    float dy = from.Y - to.Y;
    if (!(dy * dy + dx * dx > 0.0001f))
        return false;
    C3DVector delta = to - from;
    float length = delta.GetLength();
    C2DVector axis2D(bottom.X, bottom.Y);
    C2DVector to2D(to.X, to.Y);
    C2DVector from2D(from.X, from.Y);
    float t;
    float distanceSquared = GetClosestPointOnLineSegment(from2D, to2D, axis2D, &t);
    radius += cylinderRadius;
    if (!(-radius < t * length))
        return false;
    if (!((t - 1.0f) * length < radius))
        return false;
    if (!(distanceSquared < radius * radius))
        return false;
    hitPosition = delta * length * t + from;
    C3DVector offset = hitPosition - bottom;
    C3DVector axis = top - bottom;
    axis.Normalise();
    float along = (float)fabs(DotProduct(axis, offset));
    hitNormal = offset - axis * along;
    hitNormal.Normalise();
    GetPosition();
    if (!(hitPosition.Z < top.Z))
        return false;
    if (!(hitPosition.Z > bottom.Z))
        return false;
    return true;
}
