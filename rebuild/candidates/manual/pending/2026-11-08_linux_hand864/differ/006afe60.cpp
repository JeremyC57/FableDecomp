// CTCPhysicsBase::RespondToCollision(const CPhysicsCollideResponse* response, C3DVector* velocity) -- bounce the velocity off the collision: add the normal scaled by (v.n) * (-1 - friction term - elasticity) plus the velocity scaled by the friction term
// Grade: DIFFER(145v145) -- 2 masked lines: the velocity parameter load `mov eax,[esp+2Ch]` one slot late (ours after `fld [ecx+2Ch]`, retail before); chunk 864 (1 grade, probe): the only integer candidate is `this` (prio 10 -> eax), the velocity pointer is a 3-step scratch rotation (ecx, edx, ecx) -- the slot is a may-alias load order (/Oa moves it), not a register choice; 814 earlier grades
// Next idea: alias-driven load order; no plain-C++ lever known (research 2.5); documented structural residue
// Leaf: RespondToCollision
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AFE60, 145 bytes. No callees; -1.0f from the pool constant 0x0122DEE0.
// Layout: CTCPhysicsBase PhysicsDef +0x38 (CDefPointer<CPhysicsDef>; CPhysicsDef Friction +0x2c, Elasticity +0x30);
//   CPhysicsCollideResponse (PDB): CollidePos +0x00, CollideNormal +0x0c, Friction +0x18.
// Shape: friction = -(response->Friction * def->Friction); k = dot * ((-1 - friction) - Elasticity); the delta is
//   `*velocity * friction + normal * k` (the right operand evaluated first into the frame) and added with +=.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
};
inline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
struct CPhysicsCollideResponse {
    C3DVector CollidePos;                       // +0x00
    C3DVector CollideNormal;                    // +0x0c
    float     Friction;                         // +0x18
};
struct CPhysicsDef {
    unsigned char _pad00[0x2c];
    float Friction;                             // +0x2c
    float Elasticity;                           // +0x30
};
struct CTCPhysicsBase {
    void*     __vftable;                        // +0x00
    void*     ParentThing;                      // +0x04
    unsigned char _pad08[0x30];
    const CPhysicsDef* PhysicsDef;              // +0x38 CDefPointer
    void RespondToCollision(const CPhysicsCollideResponse* response, C3DVector* velocity);
};

void CTCPhysicsBase::RespondToCollision(const CPhysicsCollideResponse* response, C3DVector* velocity)
{
    const C3DVector& n = response->CollideNormal;
    const CPhysicsDef* def = PhysicsDef;
    const C3DVector& v = *velocity;
    const float* pn = &n.X;
    const float* pv = &v.X;
    float friction = -(def->Friction * response->Friction);
    float k = (n.X * v.X + n.Y * v.Y + pn[2] * pv[2]) * (-1.0f - friction - def->Elasticity);
    *velocity += v * friction + n * k;
}
