// CTCPhysicsBase::GetInterpolatedPosition(float t) -- OldPosition + (Position - OldPosition) * t, returned by value
// Leaf: GetInterpolatedPosition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049D150, length 84 bytes; 7 callers (0x0049DB74, 0x00663E12, 0x006A1B50, ...).
// The chunk label "CTCPhysicsStandard::GetFacingVector" is a propagated BSim name: the operands are CTCPhysicsBase::Position
// (+0x0c) and OldPosition (+0x18) (Ego_r.pdb), the same interpolation GetFacingVector 0x00723C20 does on RHSet.Forward.
// Callees: none (all operators inline).
// Shape: three inline C3DVector operators (each `return C3DVector(x, y, z)` through a value constructor) on a 0x18-byte frame:
// `fld b ; fsub a` x3 (the difference temporary, Z spilled), `fmul t` per component (the scaled temporary, X/Y spilled), then
// `fld scaled ; fadd a` x3 straight into the hidden result (operator+ with the scaled vector on the LEFT: `(b - a) * t + a`).
// engine/CTCPhysicsBase.h is a flattened POD without virtuals, so the class is modelled locally with the same offsets.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};

struct CTCPhysicsBase {
    virtual void v00();
    void*     m_tcBase[2];                     // +0x04 CTCBase body
    C3DVector Position;                        // +0x0c
    C3DVector OldPosition;                     // +0x18
    long      OldPositionLastFrameSet;         // +0x24
    C3DVector GetInterpolatedPosition(float t);
};

C3DVector CTCPhysicsBase::GetInterpolatedPosition(float t)
{
    return (Position - OldPosition) * t + OldPosition;
}
