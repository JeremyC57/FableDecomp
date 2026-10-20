// CTCPhysicsBase::HasSphereSphereCollidedAt(const CTCPhysicsBase* other, const C3DVector& pos, CPhysicsCollideResponse* response) const -- sphere/sphere test of this body placed at pos against other; on contact fill the response with the unit normal and the contact position
// Grade: DIFFER(305v301) -- one x87 form: the last Normalise product is fld [d.Z]; fmul st(1) + a late fstp st(0) where retail consumes the reciprocal in place (fmul [d.Z])
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float LengthSquared() const { return X * X + Y * Y + Z * Z; }
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
    if (d.LengthSquared() < radii * radii) {
        d.Normalise();
        response->CollideNormal = d;
        response->CollidePos = pos;
        return true;
    }
    return false;
}
