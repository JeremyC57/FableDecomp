// CVectorPairInterpolator_004c2ef0::GetInterpolated(float t) -- m_from + (m_to - m_from) * t over the two vectors at +0x04 / +0x10
// Leaf: GetInterpolated
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C2EF0, length 84 bytes; no direct callers found by xref. It sits between the
// C3DAnimationInterpolator destructor (0x004C2E90) and CEngineMeshEffect code; the chunk label
// "CTCPhysicsStandard::GetFacingVector" is a propagated BSim name (the owner is not a physics component: vectors at +4/+0x10).
// The class is unidentified, so it is named by address with descriptive members.
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
};

struct CVectorPairInterpolator_004c2ef0 {
    long      m_header;                        // +0x00
    C3DVector m_from;                          // +0x04
    C3DVector m_to;                            // +0x10
    C3DVector GetInterpolated(float t);
};

C3DVector CVectorPairInterpolator_004c2ef0::GetInterpolated(float t)
{
    return (m_to - m_from) * t + m_from;
}