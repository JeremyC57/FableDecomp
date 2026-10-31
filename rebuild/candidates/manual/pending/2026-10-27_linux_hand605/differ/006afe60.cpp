// CTCPhysicsBase::RespondToCollision(const CPhysicsCollideResponse& response, C3DVector& velocity) const -- bounce the velocity off the collision
// Grade: DIFFER(145v145) 5 lines incl. 1 pool reloc (chunk 605: respelled to the PDB signature, residue unchanged): retail loads the velocity pointer before the first fld and computes the dot's Z term as `fld v.Z ; fmul n.Z` (X and Y as `fld n ; fmul v`); ours loads velocity after the first fld and computes n.Z*v.Z. Tried in chunk 605: five dot spellings (`X + (Y + Z)` both Z orders, `(Z + Y) + X`, all-velocity-first, `Y + Z + X`) and DotProduct(n, v) / DotProduct(v, n) -- all compile identically (VC7.1 canonicalises the commutative products and the 3-term sum); DotProduct(velocity, response.CollideNormal) (15).
// Leaf: ?RespondToCollision@CTCPhysicsBase@@IBEXABVCPhysicsCollideResponse@@AAVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AFE60, 145 bytes, thiscall `ret 8`. No callees; -1.0f from the pool constant 0x0122DEE0.
// Identity (proven): Ego_r.pdb ?RespondToCollision@CTCPhysicsBase@@IBEXABVCPhysicsCollideResponse@@AAVC3DVector@@@Z (protected, const,
//   both arguments by reference) next to ImpulseRespondToCollision; the earlier parked spelling (public, non-const, pointers) was wrong.
// Layout: engine/CTCPhysicsBase.h (layout-rename include) PhysicsDef_Object +0x38 (CDefPointer<const CPhysicsDef>), CPhysicsDef Friction
//   +0x2c / Elasticity +0x30 (local model, no header); CPhysicsCollideResponse (PDB): CollidePos +0x00, CollideNormal +0x0c, Friction +0x18.
// Shape: friction = -(def->Friction * response.Friction); k = dot * ((-1 - friction) - Elasticity); `velocity += velocity * friction +
//   normal * k` (the right operand evaluated first into the frame).
#define CTCPhysicsBase CTCPhysicsBase_Layout
#include "engine/CTCPhysicsBase.h"
#undef CTCPhysicsBase
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
};
class CPhysicsCollideResponse {
public:
    C3DVector CollidePos;                       // +0x00
    C3DVector CollideNormal;                    // +0x0c
    float     Friction;                         // +0x18
};
class CPhysicsDef {
public:
    unsigned char _pad00[0x2c];
    float Friction;                             // +0x2c
    float Elasticity;                           // +0x30
};
class CTCPhysicsBase : public CTCPhysicsBase_Layout {
protected:
    void RespondToCollision(const CPhysicsCollideResponse& response, C3DVector& velocity) const;
};

void CTCPhysicsBase::RespondToCollision(const CPhysicsCollideResponse& response, C3DVector& velocity) const
{
    const C3DVector& n = response.CollideNormal;
    const CPhysicsDef* def = (const CPhysicsDef*)PhysicsDef_Object;
    float friction = -(def->Friction * response.Friction);
    float k = (n.X * velocity.X + n.Y * velocity.Y + velocity.Z * n.Z) * (-1.0f - friction - def->Elasticity);
    velocity += velocity * friction + response.CollideNormal * k;
}
