// CPackedVectorInterpolator_00afe320::GetInterpolated(float t) -- m_from + (m_to - m_from) * t over the packed vectors at +0x35 / +0x29
// Leaf: GetInterpolated
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AFE320, length 84 bytes; no direct callers found by xref. It sits among the
// CEnginePrimitive* constructors (0x00AFE2A0 particle sprite); the chunk label "CTCPhysicsStandard::GetFacingVector" is a
// propagated BSim name (vectors at odd offsets +0x29 / +0x35 = a packed record). Named by address with descriptive members.
// Callees: none (all operators inline).
// Shape: three inline C3DVector operators (each `return C3DVector(x, y, z)` through a value constructor) on a 0x18-byte frame:
// `fld b ; fsub a` x3 (the difference temporary, Z spilled), `fmul t` per component (the scaled temporary, X/Y spilled), then
// `fld scaled ; fadd a` x3 straight into the hidden result (operator+ with the scaled vector on the LEFT: `(b - a) * t + a`).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};

#pragma pack(push, 1)
struct CPackedVectorInterpolator_00afe320 {
    unsigned char _head[0x29];                 // +0x00
    C3DVector m_to;                            // +0x29
    C3DVector m_from;                          // +0x35
    C3DVector GetInterpolated(float t);
};
#pragma pack(pop)

C3DVector CPackedVectorInterpolator_00afe320::GetInterpolated(float t)
{
    return (m_to - m_from) * t + m_from;
}