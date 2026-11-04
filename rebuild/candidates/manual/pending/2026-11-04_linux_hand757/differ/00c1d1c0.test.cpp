// Behaviour test for 0x00C1D1C0 InvertMatrix3x4: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <cmath>
struct M { float e[12]; };
static void Invert(M& r, const M& m)
{
    const float* a = m.e;
    float det = (a[2] * a[7] - a[8] * a[1]) * a[3]; det += (a[5] * a[1] - a[2] * a[4]) * a[6]; det += (a[8] * a[4] - a[7] * a[5]) * a[0];
    if (std::fabs(det) < 1e-10f) det = det < 0.0f ? -1e-10f : 1e-10f;
    float inv = 1.0f / det;
    r.e[0] = (a[8] * a[4] - a[7] * a[5]) * inv; r.e[3] = (a[6] * a[5] - a[8] * a[3]) * inv; r.e[6] = (a[7] * a[3] - a[6] * a[4]) * inv;
    r.e[1] = (a[2] * a[7] - a[8] * a[1]) * inv; r.e[4] = (a[8] * a[0] - a[6] * a[2]) * inv; r.e[7] = (a[6] * a[1] - a[0] * a[7]) * inv;
    r.e[2] = (a[5] * a[1] - a[2] * a[4]) * inv; r.e[5] = (a[2] * a[3] - a[0] * a[5]) * inv; r.e[8] = (a[0] * a[4] - a[3] * a[1]) * inv;
    for (int c = 0; c < 3; ++c) r.e[9 + c] = -(a[9] * r.e[c] + a[10] * r.e[3 + c] + a[11] * r.e[6 + c]);
}
int main() {
    M m = { { 2, 0, 0, 0, 4, 0, 0, 0, 5, 1, 2, 3 } }, r; Invert(r, m);
    int ok = r.e[0] == 0.5f && r.e[4] == 0.25f && r.e[8] == 0.2f && r.e[9] == -0.5f && r.e[10] == -0.5f && std::fabs(r.e[11] + 0.6f) < 1e-6f;
    std::printf(ok ? "00c1d1c0_TEST PASS\n" : "00c1d1c0_TEST FAIL\n");
    return ok ? 0 : 1;
}
