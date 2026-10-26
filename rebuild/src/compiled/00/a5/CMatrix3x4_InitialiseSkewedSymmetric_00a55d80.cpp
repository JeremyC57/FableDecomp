// CMatrix3x4::InitialiseSkewedSymmetric(const C3DVector& v) -- make the matrix the skew-symmetric (cross-product) matrix of v: row0 (0,-z,y), row1 (z,0,-x), row2 (-y,x,0), zero translation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A55D80, length 97 bytes. No callees.
// Shape: an inlined identity setter runs first; its chained zero assignment stores right to left (+0x1c down to +0x04),
//        the chained 1.0f assignment stores +0x20, +0x10 (the +0x00 store merges with the later 0), then the three
//        separate translation zeroes. The skew entries then overwrite the identity in row order (fchs for the negated ones).
class C3DVector { public:
    float X, Y, Z;
};
class CMatrix3x4 { public:
    C3DVector m_row0;                              // +0x00
    C3DVector m_row1;                              // +0x0c
    C3DVector m_row2;                              // +0x18
    C3DVector m_translation;                       // +0x24
    __forceinline void SetIdentity()
    {
        m_row0.Y = m_row0.Z = m_row1.X = m_row1.Z = m_row2.X = m_row2.Y = 0.0f;
        m_row0.X = m_row1.Y = m_row2.Z = 1.0f;
        m_translation.X = 0.0f; m_translation.Y = 0.0f; m_translation.Z = 0.0f;
    }
    void InitialiseSkewedSymmetric(const C3DVector& v);
};

void CMatrix3x4::InitialiseSkewedSymmetric(const C3DVector& v)
{
    SetIdentity();
    m_row0.X = 0.0f;
    m_row0.Y = -v.Z;
    m_row0.Z = v.Y;
    m_row1.X = v.Z;
    m_row1.Y = 0.0f;
    m_row1.Z = -v.X;
    m_row2.X = -v.Y;
    m_row2.Y = v.X;
    m_row2.Z = 0.0f;
}