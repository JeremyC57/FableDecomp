// Behaviour test for CMatrix4x4::SetInverseOfUniformScaleRotationTranslation (0x00A5D000): a model of the row -- for a uniform
// scale s times a rotation plus a translation, the result times the source is the identity, and the return is 1 / s^2.
#include <stdio.h>
#include <math.h>
struct M { float e[4][4]; };
static float Inverse(M& out, const M& in)
{
    float inv = 1.0f / (in.e[0][0] * in.e[0][0] + in.e[2][0] * in.e[2][0] + in.e[1][0] * in.e[1][0]);
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            out.e[r][c] = inv * in.e[c][r];
    for (int r = 0; r < 3; ++r)
        out.e[r][3] = -(out.e[r][0] * in.e[0][3] + out.e[r][1] * in.e[1][3] + out.e[r][2] * in.e[2][3]);
    out.e[3][0] = 0; out.e[3][1] = 0; out.e[3][2] = 0; out.e[3][3] = 1.0f;
    return inv;
}
int main()
{
    float s = 2.0f, a = 0.6f, b = 0.8f;   // rotation about z by (cos a', sin a') = (0.6, 0.8)
    M m = { { { s * a, -s * b, 0, 3.0f }, { s * b, s * a, 0, -1.0f }, { 0, 0, s, 5.0f }, { 0, 0, 0, 1.0f } } };
    M inv;
    float r = Inverse(inv, m);
    int ok = fabs(r - 0.25f) < 1e-6f;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            float v = 0;
            for (int k = 0; k < 4; ++k) v += inv.e[i][k] * m.e[k][j];
            ok = ok && fabs(v - (i == j ? 1.0f : 0.0f)) < 1e-5f;
        }
    printf(ok ? "00a5d000_TEST PASS\n" : "00a5d000_TEST FAIL\n");
    return ok ? 0 : 1;
}