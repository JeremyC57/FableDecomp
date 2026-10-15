// CTCPhysicsStandard::GetFacingVector(float t) -- the facing (RHSet.Forward) interpolated from OldRHSet.Forward by t, returned by value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00723C20, length 94 bytes.
// Callees: none (all operators inline).
// Shape: the same inline-operator lerp as 0x0049D150 but the result goes through a NAMED local returned by C3DVector's
// memberwise user copy constructor: the sum's X is spilled to the frame (`fstp [esp+0Ch]`) and reloaded, then the three
// `fstp` into the hidden result with an `fxch` (VC7.1 has no NRVO; the copy of a fully computed local folds into x87 stores).
// The direct `return expr;` form keeps the 0x0049D150 schedule (84 bytes).
// Layout: Ego_r.pdb CTCPhysicsStandard RHSet +0x50 / OldRHSet +0x68 (CRightHandedSet = Up, Forward), so the Forward vectors sit at
// +0x5c / +0x74. engine/CTCPhysicsStandard.h is a flattened POD without virtuals, so the class is modelled locally.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };

struct CTCPhysicsStandard {
    virtual void v00();
    unsigned char   _base[0x3c];               // +0x04 CTCPhysicsBase body
    void*           PPhysicsMeshInfo;          // +0x40
    C3DVector       OffsetVector3D;            // +0x44
    CRightHandedSet RHSet;                     // +0x50
    CRightHandedSet OldRHSet;                  // +0x68
    C3DVector GetFacingVector(float t);
};

C3DVector CTCPhysicsStandard::GetFacingVector(float t)
{
    C3DVector v = (RHSet.Forward - OldRHSet.Forward) * t + OldRHSet.Forward;
    return v;
}