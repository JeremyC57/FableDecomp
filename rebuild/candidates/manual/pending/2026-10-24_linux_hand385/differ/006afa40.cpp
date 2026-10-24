// Grade: DIFFER(170v170) -- x87 load order only: retail loads the velocity for the inline length as z, y, x (`fld [ecx+8] ; fld [ecx+4] ; fld [ecx]`, squaring x first), ours x, y, z (squaring z first); the rest is identical. Declaring the locals z, y, x fixes the loads but flips the dot product to Y-then-Z.
// CTCPhysicsBase::ImpulseRespondToCollisonWithCylinder(const CPhysicsCollideResponse& response, C3DVector& velocity) const -- remove the velocity's component into the collision normal when it points into the surface
// Leaf: ?ImpulseRespondToCollisonWithCylinder@CTCPhysicsBase@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AFA40, length 170 bytes (+ int3 padding), thiscall `ret 8`
//   (`this` unused). Constant: 0.0f @ 0x0122DEDC. No calls.
// Identity: Ego_r.pdb `?ImpulseRespondToCollisonWithCylinder@CTCPhysicsBase@@IBE_NABVCPhysicsCollideResponse@@AAVC3DVector@@@Z`
//   (name set only; the PDB's "Collison" spelling). Retail neighbours agree: ImpulseRespondToCollision 0x006AF7D0
//   before, ImpulseRespondToCollisonWithImpassableObject 0x006AFAF0 after, and all three callers
//   (CTCPhysicsControlled::PerformImpulseBasedMovement, CTCPhysicsNavigator::PerformForcedMovement,
//   CTCPhysicsStandard::PerformStepBasedMovement) call it right after IsPhysicsMeshTypeCylinder 0x006B0170 succeeds.
// Layout: CPhysicsCollideResponse (Ego_r) CollidePos +0x00, CollideNormal +0x0c, Friction +0x18 (the landed
//   ModifyResponseForNearVerticalSlopes 0x006B0610 model). No member of the TC is read, so `this` is an empty model.
// Shape: the speed is the inline length (x, y, z copies, flat square sum, `fsqrt`) homed in the dead velocity
//   argument slot; the normalised normal component d = Dot(velocity, normal) / speed; when d < 0 the velocity
//   loses normal * d * speed through two C3DVector temporaries (the 0x18-byte frame) and the result is true.
#include <math.h>
#pragma intrinsic(sqrt)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetLength() const
    {
        float x = X;
        float y = Y;
        float z = Z;
        return (float)sqrt(x * x + y * y + z * z);
    }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator-=(const C3DVector& o)
    {
        X -= o.X;
        Y -= o.Y;
        Z -= o.Z;
        return *this;
    }
};
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

class CPhysicsCollideResponse {
public:
    C3DVector CollidePos;                                   // +0x00
    C3DVector CollideNormal;                                // +0x0c
    float     Friction;                                     // +0x18
};

class CTCPhysicsBase {
public:
    bool ImpulseRespondToCollisonWithCylinder(const CPhysicsCollideResponse& response, C3DVector& velocity) const;
};

bool CTCPhysicsBase::ImpulseRespondToCollisonWithCylinder(const CPhysicsCollideResponse& response, C3DVector& velocity) const
{
    float speed = velocity.GetLength();
    float into = Dot(velocity, response.CollideNormal) / speed;
    if (into < 0.0f) {
        velocity -= response.CollideNormal * into * speed;
        return true;
    }
    return false;
}
