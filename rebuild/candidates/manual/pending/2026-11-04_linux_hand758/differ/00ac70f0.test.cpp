// Behaviour test for QuadricOptimiserInternals::CQuadric::GetInverseMatrixPart (0x00AC70F0), g++ model: the cofactor inverse of the symmetric 3x3 part.
#include <cstdio>
#include <cmath>
struct Q { double AA, AB, AC, BB, BC, CC; };
static void Inverse(const Q& q, float m[3][3])
{
    double c13 = q.AC * q.BC - q.CC * q.AB, c23 = q.BC * q.AB - q.AC * q.BB, c33 = q.CC * q.BB - q.BC * q.BC;
    double det = c33 * q.AA + c23 * q.AC + c13 * q.AB, inv = 1.0 / det;
    m[0][0] = (float)(c33 * inv); m[1][0] = m[0][1] = (float)(c13 * inv); m[2][0] = m[0][2] = (float)(c23 * inv);
    m[1][1] = (float)((q.CC * q.AA - q.AC * q.AC) * inv); m[2][1] = m[1][2] = (float)((q.AC * q.AB - q.AA * q.BC) * inv);
    m[2][2] = (float)((q.AA * q.BB - q.AB * q.AB) * inv);
}
int main()
{
    Q q = { 4, 1, 2, 3, 0.5, 5 };
    double a[3][3] = { { q.AA, q.AB, q.AC }, { q.AB, q.BB, q.BC }, { q.AC, q.BC, q.CC } };
    float m[3][3]; Inverse(q, m);
    int ok = 1;
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) {
        double s = 0; for (int k = 0; k < 3; ++k) s += a[i][k] * m[k][j];
        ok = ok && std::fabs(s - (i == j ? 1.0 : 0.0)) < 1e-5;
    }
    std::printf(ok ? "00ac70f0_TEST PASS\n" : "00ac70f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
