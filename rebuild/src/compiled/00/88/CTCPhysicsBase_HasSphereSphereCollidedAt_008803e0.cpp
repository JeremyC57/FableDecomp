// CTCPhysicsBase::HasSphereSphereCollidedAt(const CTCPhysicsBase* other, const C3DVector& pos, CPhysicsCollideResponse* response) const -- sphere/sphere test of this body placed at pos against other; on contact fill the response with the unit normal and the contact position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008803E0, length 301 bytes (+ int3 padding), `ret 0Ch`.
// Callees: none (C3DVector's inline fast Normalise: exponent (380 - e) / 2 with the 128-entry mantissa table 0x013CE558,
// one Newton step with 3.0f 0x0122DED4 and 0.5f 0x0122F59C).
// Shape: d is built component-wise with the radii folded into Z ((radius + pos.Z) - (otherRadius + other.Z)); the contact
// test is the single expression d.X*d.X + d.Y*d.Y + d.Z*d.Z against (radius + otherRadius) squared -- spelled out, not
// through an inline LengthSquared(), which keeps the reciprocal alive past the last Normalise product (`fld [d.Z];
// fmul st(1)` + a late `fstp st(0)` instead of retail's in-place `fmul [d.Z]`); Normalise copies X/Y/Z into locals and
// sums the squares as statements (reusing the stored products), then the normal is copied before the contact position.
// Layout: Ego_r.pdb CTCPhysicsBase (Position +0x0c, Radius +0x34) and CPhysicsCollideResponse (CollidePos +0x00,
// CollideNormal +0x0c, Friction +0x18); engine/CTCPhysicsBase.h flattens the vectors into floats, so the class is modelled
// locally with C3DVector members (as in the landed sibling 0x008802B0).
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
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
    bool HasSphereSphereCollidedAt(const CTCPhysicsBase* other, const C3DVector& pos, CPhysicsCollideResponse* response) const;
};

bool CTCPhysicsBase::HasSphereSphereCollidedAt(const CTCPhysicsBase* other, const C3DVector& pos, CPhysicsCollideResponse* response) const
{
    float otherRadius = other->Radius;
    float radius = Radius;
    C3DVector d(pos.X - other->Position.X, pos.Y - other->Position.Y, (radius + pos.Z) - (otherRadius + other->Position.Z));
    float radii = radius + otherRadius;
    if (d.X * d.X + d.Y * d.Y + d.Z * d.Z < radii * radii) {
        d.Normalise();
        response->CollideNormal = d;
        response->CollidePos = pos;
        return true;
    }
    return false;
}