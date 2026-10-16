// CTCPhysicsBase::RespondToCollision(const CPhysicsCollideResponse* response, C3DVector* velocity) -- bounce the velocity off the collision: add the normal scaled by (v.n) * (-1 - friction term - elasticity) plus the velocity scaled by the friction term
// Grade: DIFFER(ours v retail) -- 145v145: response/def register swap (retail edx=response, ecx=PhysicsDef) and the dot product's Z term loads v.Z then multiplies N.Z (ours N.Z then v.Z); tried DotProduct(n,v), DotProduct(v,n) and an explicit sum (lane gap: x87 term order)
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
    float friction = -(response->Friction * PhysicsDef->Friction);
    const C3DVector& n = response->CollideNormal;
    float k = (n.X * velocity->X + n.Y * velocity->Y + velocity->Z * n.Z) * (-1.0f - friction - PhysicsDef->Elasticity);
    *velocity += *velocity * friction + response->CollideNormal * k;
}
