// CTCPhysicsBase::HasSphereCylinderCollidedAt(const CTCPhysicsBase* other, const C3DVector& pos, CPhysicsCollideResponse* response) const -- this body's sphere placed at pos against other's vertical cylinder; on contact fill the response with the horizontal unit normal and the contact position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008802B0, length 291 bytes (+ int3 padding), `ret 0Ch`.
// Callees: none (C3DVector's inline fast Normalise: exponent (380 - e) / 2 with the 128-entry mantissa table 0x013CE558,
// one Newton step with 3.0f 0x0122DED4 and 0.5f 0x0122F59C). Constants: 1.6f 0x0125A9A0 (cylinder half-height test on
// |dz|), 0.0f 0x0122DEDC (the zeroed Z multiplied by the reciprocal).
// Shape: `d = pos - other->Position` through C3DVector's inline operator- (a component-wise ctor loads `pos` before
// `other` and swaps the dead argument slots the two squared terms are homed in); the horizontal test is a single-expression
// X*X + Y*Y (VC7.1 computes Y*Y first) against (other radius + radius) squared, `&& fabsf(d.Z) < 1.6f`, both false exits
// share one `return false`; then d.Z = 0 and Normalise (statement-order length sum reusing the stored squares), the normal
// is copied before the contact position.
// Layout: Ego_r.pdb CTCPhysicsBase (Position +0x0c, Radius +0x34) and CPhysicsCollideResponse (CollidePos +0x00,
// CollideNormal +0x0c, Friction +0x18); engine/CTCPhysicsBase.h flattens the vectors into floats, so the class is modelled
// locally with C3DVector members.
#include <math.h>
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x;
        lengthSquared += y * y;
        lengthSquared += z * z;
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

struct CPhysicsCollideResponse {
    C3DVector CollidePos;       // +0x00
    C3DVector CollideNormal;    // +0x0c
    float     Friction;         // +0x18
};

struct CTCPhysicsBase {
    void*     __vftable;                       // +0x00
    void*     m_tcBase[2];                     // +0x04 CTCBase body
    C3DVector Position;                        // +0x0c
    C3DVector OldPosition;                     // +0x18
    long      OldPositionLastFrameSet;         // +0x24
    C3DVector Velocity;                        // +0x28
    float     Radius;                          // +0x34
    bool HasSphereCylinderCollidedAt(const CTCPhysicsBase* other, const C3DVector& pos, CPhysicsCollideResponse* response) const;
};

bool CTCPhysicsBase::HasSphereCylinderCollidedAt(const CTCPhysicsBase* other, const C3DVector& pos, CPhysicsCollideResponse* response) const
{
    C3DVector d = pos - other->Position;
    float radii = other->Radius + Radius;
    if (d.X * d.X + d.Y * d.Y < radii * radii && fabsf(d.Z) < 1.6f) {
        d.Z = 0.0f;
        d.Normalise();
        response->CollideNormal = d;
        response->CollidePos = pos;
        return true;
    }
    return false;
}