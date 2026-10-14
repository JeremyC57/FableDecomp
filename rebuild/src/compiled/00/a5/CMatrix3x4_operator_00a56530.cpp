// CMatrix3x4::operator*(const C3DVector& v) const -- transform a vector by the 3x3 part of the matrix (rows at +0, +0xc, +0x18): the three dot products, returned by hidden pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A56530, length 83 bytes.
// Leaf: ??DCMatrix3x4@@QBE?AVC3DVector@@ABV1@@Z
// Shape: the hidden-pointer result built in a named local and returned through the memberwise copy constructor (the three sums stay on the x87 stack, `fxch st(2)` then three stores); a value constructor evaluates the rows right to left, a POD copies through the frame, `ret 8`.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }   // the memberwise copy into the hidden result: VC7.1 has no NRVO, but the copy of a fully computed local folds into three x87 stores
};

struct CMatrix3x4 {
    C3DVector m_row0;                              // +0x00
    C3DVector m_row1;                              // +0x0c
    C3DVector m_row2;                              // +0x18
    C3DVector m_translation;                       // +0x24
    C3DVector operator*(const C3DVector& v) const;
};

C3DVector CMatrix3x4::operator*(const C3DVector& v) const
{
    C3DVector result;
    result.X = m_row0.X * v.X + m_row0.Y * v.Y + m_row0.Z * v.Z;
    result.Y = m_row1.X * v.X + m_row1.Y * v.Y + m_row1.Z * v.Z;
    result.Z = m_row2.X * v.X + m_row2.Y * v.Y + m_row2.Z * v.Z;
    return result;
}