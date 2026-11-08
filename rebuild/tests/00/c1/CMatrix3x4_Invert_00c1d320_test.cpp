// Behaviour test for CMatrix3x4::Invert (0x00C1D320): an affine matrix times its inverse is the identity (rotation part)
// and maps the translated origin back to zero; a singular matrix uses the clamped +epsilon determinant.
#include <cstdio>
#include <cmath>

static float g_fMatrixInverseEpsilon = 1e-10f;
class CMatrix3x4 { public:
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    void Invert();
};
void CMatrix3x4::Invert()
{
    CMatrix3x4 m = *this;
    float det = m.E11 * (m.E22 * m.E33 - m.E32 * m.E23) + m.E21 * (m.E13 * m.E32 - m.E12 * m.E33) + m.E31 * (m.E12 * m.E23 - m.E13 * m.E22);
    if (std::fabs(det) < g_fMatrixInverseEpsilon)
        det = det < 0.0f ? -g_fMatrixInverseEpsilon : g_fMatrixInverseEpsilon;
    float inv = 1.0f / det;
    E11 = (m.E22 * m.E33 - m.E23 * m.E32) * inv;
    E21 = (m.E23 * m.E31 - m.E21 * m.E33) * inv;
    E31 = (m.E21 * m.E32 - m.E22 * m.E31) * inv;
    E12 = (m.E32 * m.E13 - m.E33 * m.E12) * inv;
    E22 = (m.E33 * m.E11 - m.E13 * m.E31) * inv;
    E32 = (m.E31 * m.E12 - m.E32 * m.E11) * inv;
    E13 = (m.E23 * m.E12 - m.E22 * m.E13) * inv;
    E23 = (m.E21 * m.E13 - m.E23 * m.E11) * inv;
    E33 = (m.E22 * m.E11 - m.E21 * m.E12) * inv;
    E41 = -(E11 * m.E41 + E21 * m.E42 + E31 * m.E43);
    E42 = -(E12 * m.E41 + E22 * m.E42 + E32 * m.E43);
    E43 = -(E13 * m.E41 + E23 * m.E42 + E33 * m.E43);
}
// row-vector convention: p' = p * R + T
static void Apply(const CMatrix3x4& a, const float p[3], float o[3])
{
    o[0] = p[0] * a.E11 + p[1] * a.E21 + p[2] * a.E31 + a.E41;
    o[1] = p[0] * a.E12 + p[1] * a.E22 + p[2] * a.E32 + a.E42;
    o[2] = p[0] * a.E13 + p[1] * a.E23 + p[2] * a.E33 + a.E43;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    CMatrix3x4 a = { 2, 0, 1,  0, 3, 0,  1, 0, 1,  5, -2, 7 };
    CMatrix3x4 b = a; b.Invert();
    bool ok = true;
    const float pts[3][3] = { { 1, 2, 3 }, { -4, 0.5f, 2 }, { 0, 0, 0 } };
    for (int i = 0; i < 3; ++i) {
        float q[3], r[3];
        Apply(a, pts[i], q); Apply(b, q, r);
        ok = ok && Near(r[0], pts[i][0]) && Near(r[1], pts[i][1]) && Near(r[2], pts[i][2]);
    }
    CMatrix3x4 s = { 1, 2, 3,  2, 4, 6,  0, 0, 0,  0, 0, 0 };   // det 0 -> 1/1e-10 scale
    s.Invert();
    ok = ok && s.E11 == 0.0f && std::fabs(s.E33) == 0.0f && s.E41 == 0.0f;
    CMatrix3x4 u = { 1, 0, 0,  0, 1, 0,  0, 0, 0,  0, 0, 0 };   // det 0 -> clamped to +epsilon
    u.Invert();
    ok = ok && u.E33 == 1.0f / 1e-10f && u.E11 == 0.0f;
    if (!ok) { std::printf("00c1d320_TEST FAIL\n"); return 1; }
    std::printf("00c1d320_TEST PASS\n");
    return 0;
}