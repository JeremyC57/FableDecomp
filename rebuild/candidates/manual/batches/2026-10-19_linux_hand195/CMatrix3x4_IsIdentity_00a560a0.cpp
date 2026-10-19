// CMatrix3x4::IsIdentity() const -- true when every row (and the translation) is within the squared tolerance of the identity matrix
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A560A0, length 209 bytes (`ret`).
// Callees: none; constants: the tolerance extern float @ 0x0129BA3C (0.0001f), 1.0f @ 0x0122DED8.
// Layout: four C3DVector rows (landed CMatrix3x4 0x00A56530 model): rows +0x00/+0x0C/+0x18, translation +0x24.
// Shape: every test squares the tolerance (VC7.1 computes it once into the `push ecx` frame word); each row minus its identity row
// (the zero subtractions fold, the translation included) is compared by squared length; the four tests are one `&&` chain
// guarding `return true` (the shared `return false` exit after it).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
extern const float g_IdentityTolerance;         // 0x0129BA3C (0.0001f)
struct CMatrix3x4 {
    C3DVector m_row0;                           // +0x00
    C3DVector m_row1;                           // +0x0c
    C3DVector m_row2;                           // +0x18
    C3DVector m_translation;                    // +0x24
    bool IsIdentity() const;
};

bool CMatrix3x4::IsIdentity() const
{
    if (!((m_row0 - C3DVector(1.0f, 0.0f, 0.0f)).GetLengthSquared() > g_IdentityTolerance * g_IdentityTolerance) &&
        !((m_row1 - C3DVector(0.0f, 1.0f, 0.0f)).GetLengthSquared() > g_IdentityTolerance * g_IdentityTolerance) &&
        !((m_row2 - C3DVector(0.0f, 0.0f, 1.0f)).GetLengthSquared() > g_IdentityTolerance * g_IdentityTolerance) &&
        !((m_translation - C3DVector(0.0f, 0.0f, 0.0f)).GetLengthSquared() > g_IdentityTolerance * g_IdentityTolerance))
        return true;
    return false;
}
