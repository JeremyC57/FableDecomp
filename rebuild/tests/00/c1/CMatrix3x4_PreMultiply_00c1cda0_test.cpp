// Behaviour test for 0x00C1CDA0 CMatrix3x4::PreMultiply(const CMatrix3x4& matrix): this = matrix * this
#include <stdio.h>

struct M { float E[12]; };      // E11..E13, E21..E23, E31..E33, E41..E43 (row 4 = translation)
static void PreMultiply(M& t, const M& m)                            // the row's body
{
    M a = t;
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            t.E[r * 3 + c] = m.E[r * 3 + 0] * a.E[0 + c] + m.E[r * 3 + 1] * a.E[3 + c] + m.E[r * 3 + 2] * a.E[6 + c];
    for (int c = 0; c < 3; ++c)
        t.E[9 + c] = m.E[9] * a.E[0 + c] + m.E[10] * a.E[3 + c] + m.E[11] * a.E[6 + c] + a.E[9 + c];
}
// reference: 4x4 homogeneous row-vector product (m then this)
static void Ref(const M& m, const M& t, float out[12])
{
    float A[4][4], B[4][4], C[4][4];
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 3; ++c) { A[r][c] = m.E[r * 3 + c]; B[r][c] = t.E[r * 3 + c]; }
    for (int r = 0; r < 4; ++r) { A[r][3] = r == 3 ? 1.0f : 0.0f; B[r][3] = r == 3 ? 1.0f : 0.0f; }
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) { C[r][c] = 0; for (int k = 0; k < 4; ++k) C[r][c] += A[r][k] * B[k][c]; }
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 3; ++c) out[r * 3 + c] = C[r][c];
}

int main()
{
    M t = { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } };
    M m = { { 0, 1, 0, -1, 0, 0, 0, 0, 1, 5, -2, 3 } };
    float want[12];
    Ref(m, t, want);
    PreMultiply(t, m);
    int ok = 1;
    for (int i = 0; i < 12; ++i) ok &= t.E[i] == want[i];
    ok &= t.E[0] == 4.0f && t.E[9] == 5.0f * 1 - 2.0f * 4 + 3.0f * 7 + 10.0f;
    printf(ok ? "00c1cda0_TEST PASS\n" : "00c1cda0_TEST FAIL\n");
    return ok ? 0 : 1;
}