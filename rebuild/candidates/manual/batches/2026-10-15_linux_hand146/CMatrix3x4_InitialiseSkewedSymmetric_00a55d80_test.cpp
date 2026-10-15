// Behaviour test for 0x00A55D80 CMatrix3x4::InitialiseSkewedSymmetric: M*w must equal v x w for the rotation part, translation zero.
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 {
    C3DVector m_row0, m_row1, m_row2, m_translation;
    void SetIdentity()
    {
        m_row0.Y = m_row0.Z = m_row1.X = m_row1.Z = m_row2.X = m_row2.Y = 0.0f;
        m_row0.X = m_row1.Y = m_row2.Z = 1.0f;
        m_translation.X = 0.0f; m_translation.Y = 0.0f; m_translation.Z = 0.0f;
    }
    void InitialiseSkewedSymmetric(const C3DVector& v)
    {
        SetIdentity();
        m_row0.X = 0.0f; m_row0.Y = -v.Z; m_row0.Z = v.Y;
        m_row1.X = v.Z; m_row1.Y = 0.0f; m_row1.Z = -v.X;
        m_row2.X = -v.Y; m_row2.Y = v.X; m_row2.Z = 0.0f;
    }
};
static float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
int main()
{
    CMatrix3x4 m;
    m.m_translation.X = 5.0f; m.m_translation.Y = 6.0f; m.m_translation.Z = 7.0f;
    C3DVector v = { 1.0f, 2.0f, 3.0f };
    C3DVector w = { 4.0f, -5.0f, 6.0f };
    m.InitialiseSkewedSymmetric(v);
    // v x w = (vy*wz - vz*wy, vz*wx - vx*wz, vx*wy - vy*wx) = (12+15, 12-6, -5-8) = (27, 6, -13)
    if (Dot(m.m_row0, w) != 27.0f || Dot(m.m_row1, w) != 6.0f || Dot(m.m_row2, w) != -13.0f ||
        m.m_translation.X != 0.0f || m.m_translation.Y != 0.0f || m.m_translation.Z != 0.0f ||
        m.m_row0.X != 0.0f || m.m_row1.Y != 0.0f || m.m_row2.Z != 0.0f) {
        std::printf("00a55d80_TEST FAIL\n");
        return 1;
    }
    std::printf("00a55d80_TEST PASS\n");
    return 0;
}
