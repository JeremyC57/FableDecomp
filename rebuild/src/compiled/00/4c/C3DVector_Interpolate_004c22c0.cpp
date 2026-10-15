// C3DVector::Interpolate(const C3DVector& to, float t) const -- *this + (to - *this) * t, returned by value
// Leaf: Interpolate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C22C0, length 83 bytes (`ret 0Ch`: hidden result, to, t). No direct callers
// found by xref (reached through an inlined/indirect use). The chunk label "CTCPhysicsStandard::GetFacingVector" is a
// propagated BSim name: `this` is the start vector itself (`fsub [ecx]` / `fadd [ecx]`), the end vector a reference argument.
// Callees: none (all operators inline).
// Shape: three inline C3DVector operators (each `return C3DVector(x, y, z)` through a value constructor) on a 0x18-byte frame:
// `fld b ; fsub a` x3 (the difference temporary, Z spilled), `fmul t` per component (the scaled temporary, X/Y spilled), then
// `fld scaled ; fadd a` x3 straight into the hidden result (operator+ with the scaled vector on the LEFT: `(b - a) * t + a`).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector Interpolate(const C3DVector& to, float t) const;
};

C3DVector C3DVector::Interpolate(const C3DVector& to, float t) const
{
    return (to - *this) * t + *this;
}