// Behaviour test for CMatrix3x4::SetFromVectorAndAngle (0x00C1D0B0): axis-angle rotation matrix.
#include <cstdio>
#include <cmath>
class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 {
public:
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    void SetFromVectorAndAngle(const C3DVector& axis, float angle)
    {
        float c = (float)std::cos(angle), s = (float)std::sin(angle);
        float xx = axis.X * axis.X, yy = axis.Y * axis.Y, zz = axis.Z * axis.Z;
        float xy = axis.X * axis.Y, xz = axis.X * axis.Z, yz = axis.Y * axis.Z;
        float sx = s * axis.X, sy = s * axis.Y, sz = s * axis.Z;
        E11 = (1.0f - xx) * c + xx; E21 = xy - xy * c + sz; E31 = xz - xz * c - sy;
        E12 = xy - xy * c - sz;     E22 = (1.0f - yy) * c + yy; E32 = yz - yz * c + sx;
        E13 = xz - xz * c + sy;     E23 = yz - yz * c - sx;     E33 = (1.0f - zz) * c + zz;
        E41 = E42 = E43 = 0.0f;
    }
};
static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    CMatrix3x4 m; m.E41 = m.E42 = m.E43 = 7.0f;
    C3DVector z = { 0.0f, 0.0f, 1.0f };
    m.SetFromVectorAndAngle(z, 1.5707963f);             // quarter turn about Z
    bool ok = near(m.E11, 0) && near(m.E12, -1) && near(m.E13, 0) && near(m.E21, 1) && near(m.E22, 0) &&
              near(m.E33, 1) && near(m.E31, 0) && near(m.E32, 0) && m.E41 == 0 && m.E42 == 0 && m.E43 == 0;
    // arbitrary axis: the matrix is orthonormal and leaves the axis fixed
    float k = 1.0f / (float)std::sqrt(14.0);
    C3DVector a = { 1 * k, 2 * k, 3 * k };
    m.SetFromVectorAndAngle(a, 0.7f);
    float r0 = m.E11 * a.X + m.E12 * a.Y + m.E13 * a.Z;
    float r1 = m.E21 * a.X + m.E22 * a.Y + m.E23 * a.Z;
    float r2 = m.E31 * a.X + m.E32 * a.Y + m.E33 * a.Z;
    ok = ok && near(r0, a.X) && near(r1, a.Y) && near(r2, a.Z);
    ok = ok && near(m.E11 * m.E11 + m.E12 * m.E12 + m.E13 * m.E13, 1) && near(m.E11 * m.E21 + m.E12 * m.E22 + m.E13 * m.E23, 0);
    ok = ok && near(m.E11 + m.E22 + m.E33, 1 + 2 * (float)std::cos(0.7));
    if (!ok) { std::printf("00c1d0b0_TEST FAIL\n"); return 1; }
    std::printf("00c1d0b0_TEST PASS\n");
    return 0;
}