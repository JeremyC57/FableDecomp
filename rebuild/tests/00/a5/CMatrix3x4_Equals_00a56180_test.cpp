#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector& operator-=(const C3DVector& o) { X -= o.X; Y -= o.Y; Z -= o.Z; return *this; }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
static int g_isIdentityCalls = 0;
struct CMatrix3x4 {
    C3DVector m_row0, m_row1, m_row2, m_translation;
    CMatrix3x4() { }
    bool IsIdentity() const
    {
        ++g_isIdentityCalls;
        const float t = 0.0001f * 0.0001f;
        C3DVector a = m_row0; a.X -= 1.0f; C3DVector b = m_row1; b.Y -= 1.0f; C3DVector c = m_row2; c.Z -= 1.0f;
        return a.GetLengthSquared() <= t && b.GetLengthSquared() <= t && c.GetLengthSquared() <= t && m_translation.GetLengthSquared() <= t;
    }
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
int main()
{
    CMatrix3x4 a, b;
    a.m_row0 = C3DVector(0.5f, 2, 3); a.m_row1 = C3DVector(4, 5, 6); a.m_row2 = C3DVector(7, 8, 9); a.m_translation = C3DVector(10, 11, 12);
    b = a;
    bool ok = a.Equals(b) && b.Equals(a);
    b.m_row2.Z = 9.5f; ok = ok && !a.Equals(b);
    b = a; b.m_translation.X = 10.00001f; ok = ok && a.Equals(b);
    b = a; b.m_row0.Y = 1.0f; ok = ok && !a.Equals(b) && g_isIdentityCalls == 5;
    if (!ok) { std::printf("00a56180_TEST FAIL\n"); return 1; }
    std::printf("00a56180_TEST PASS\n");
    return 0;
}