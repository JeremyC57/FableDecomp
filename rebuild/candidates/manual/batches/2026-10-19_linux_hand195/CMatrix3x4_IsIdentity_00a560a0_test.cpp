#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
static const float g_IdentityTolerance = 0.0001f;
struct CMatrix3x4 {
    C3DVector m_row0, m_row1, m_row2, m_translation;
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
int main()
{
    CMatrix3x4 m;
    m.m_row0 = C3DVector(1, 0, 0); m.m_row1 = C3DVector(0, 1, 0); m.m_row2 = C3DVector(0, 0, 1); m.m_translation = C3DVector(0, 0, 0);
    bool ok = m.IsIdentity();
    m.m_row0.X = 1.00001f; ok = ok && m.IsIdentity();
    m.m_row1.Z = 0.01f; ok = ok && !m.IsIdentity();
    m.m_row1.Z = 0; m.m_translation.Y = 0.5f; ok = ok && !m.IsIdentity();
    m.m_translation.Y = 0; m.m_row2.Z = 2.0f; ok = ok && !m.IsIdentity();
    m.m_row2.Z = 1.0f; m.m_row0.Y = -0.2f; ok = ok && !m.IsIdentity();
    if (!ok) { std::printf("00a560a0_TEST FAIL\n"); return 1; }
    std::printf("00a560a0_TEST PASS\n");
    return 0;
}
