// CTCPhysicsBase::RespondToCollision(const CPhysicsCollideResponse* response, C3DVector* velocity) -- bounce the velocity off the collision: add the normal scaled by (v.n) * (-1 - friction term - elasticity) plus the velocity scaled by the friction term
// Grade: DIFFER(145v145) -- velocity parameter load `mov eax,[esp+2Ch]` one slot late (ours after `fld [ecx+2Ch]`, retail before); 2 masked lines, 419 attempts this pass (chunk 847) after 393 earlier; /Oa fixes it (alias-driven), so the residue is a may-alias ordering of the velocity read against the def loads; chunk 859: 2 more (814 total): the Ego_r signature `RespondToCollision(const CPhysicsCollideResponse&, C3DVector&) const` with `class` keys (V mangling) compiles to the same bytes; the may-alias ordering of the velocity read is not steerable from the signature; chunk 859 total 112 grades, cross-product grids of behaviour-identical spellings (friction forms (3) x k forms (4) x result forms (5) x PhysicsDef local/direct x Z pointer/member x Ego_r reference-const signature (110 of 480)): best unchanged
// Next idea: the 240-cell operand grid shows the velocity-load slot and the Y-pair order exclude each other over all operand spellings, so change the IR: take the dot through C3DVector::DotProduct (Ego_r member) on the velocity reference, or compute `*velocity = v + v*friction + n*k` as a three-term sum; check Ego_r CTCPhysicsBase::RespondToCollision signature for a by-value velocity
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
