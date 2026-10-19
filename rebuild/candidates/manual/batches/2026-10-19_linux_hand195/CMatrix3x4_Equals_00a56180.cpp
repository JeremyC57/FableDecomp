// CMatrix3x4::Equals(const CMatrix3x4& other) const -- compare two matrices within the identity tolerance: (other + I - this) must be the identity
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A56180, length 238 bytes (`ret 4`).
// Callees: CMatrix3x4::IsIdentity() const @ 0x00A560A0 (on the frame temporary); 1.0f @ 0x0122DED8.
// Layout: four C3DVector rows (landed CMatrix3x4 0x00A56530 model).
// Shape: one 0x30-byte frame matrix filled element by element in order (`other` plus 1.0f on the diagonal; the plain elements copy
// through edx), then `this` subtracted row by row with the inline C3DVector `-=` (a matrix-level `-=` is emitted out of line).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector& operator-=(const C3DVector& o) { X -= o.X; Y -= o.Y; Z -= o.Z; return *this; }
};
struct CMatrix3x4 {
    C3DVector m_row0;                           // +0x00
    C3DVector m_row1;                           // +0x0c
    C3DVector m_row2;                           // +0x18
    C3DVector m_translation;                    // +0x24
    CMatrix3x4() { }
    bool IsIdentity() const;                    // 0x00A560A0
    bool Equals(const CMatrix3x4& other) const;
};

bool CMatrix3x4::Equals(const CMatrix3x4& other) const
{
    CMatrix3x4 difference;
    difference.m_row0.X = other.m_row0.X + 1.0f;
    difference.m_row0.Y = other.m_row0.Y;
    difference.m_row0.Z = other.m_row0.Z;
    difference.m_row1.X = other.m_row1.X;
    difference.m_row1.Y = other.m_row1.Y + 1.0f;
    difference.m_row1.Z = other.m_row1.Z;
    difference.m_row2.X = other.m_row2.X;
    difference.m_row2.Y = other.m_row2.Y;
    difference.m_row2.Z = other.m_row2.Z + 1.0f;
    difference.m_translation.X = other.m_translation.X;
    difference.m_translation.Y = other.m_translation.Y;
    difference.m_translation.Z = other.m_translation.Z;
    difference.m_row0 -= m_row0;
    difference.m_row1 -= m_row1;
    difference.m_row2 -= m_row2;
    difference.m_translation -= m_translation;
    return difference.IsIdentity();
}
