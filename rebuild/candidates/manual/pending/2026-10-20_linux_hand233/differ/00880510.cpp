// CTCPhysicsBase::HasSphereSphereCollidedAt(const CTCPhysicsBase& other, const C3DVector& from, const C3DVector& to, CPhysicsCollideResponse& response) const -- the sphere test at 'to' (0x008803E0); on contact move the contact point back along the from->to direction by both radii and drop it by this radius
// Grade: DIFFER(299v301) -- x87 spill only: retail stores the Normalise reciprocal with fstp and multiplies X/Y/Z from memory (fxch; fmul [r]); VC7.1 here keeps it on the stack (fst; fmul st(2); fstp st(1)). Same shape as parked 006afaf0's dot.
#include <math.h>
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
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
    bool HasSphereSphereCollidedAt(const CTCPhysicsBase& other, const C3DVector& pos, CPhysicsCollideResponse& response) const;   // 0x008803E0
    bool HasSphereSphereCollidedAt(const CTCPhysicsBase& other, const C3DVector& from, const C3DVector& to, CPhysicsCollideResponse& response) const;
};

bool CTCPhysicsBase::HasSphereSphereCollidedAt(const CTCPhysicsBase& other, const C3DVector& from, const C3DVector& to, CPhysicsCollideResponse& response) const
{
    if (HasSphereSphereCollidedAt(other, to, response)) {
        C3DVector dir = to - from;
        dir.Normalise();
        dir *= other.Radius + Radius;
        response.CollidePos = to - dir;
        response.CollidePos.Z -= Radius;
        return true;
    }
    return false;
}
