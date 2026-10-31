// Behaviour test for CMatrix4x4::AlignedSetInterpolate (0x00A5CEE0): the SSE path stores a*(1-t) + b*t row by row;
// the x87 fallback stores the transposed lerp m[r][c] = a[c][r] + (b[c][r] - a[c][r]) * t.
#include <cstdio>
#include <cmath>

struct M4 { float m[4][4]; };

static void InterpolateSSE(M4& out, const M4& a, const M4& b, float t)
{
    float u = 1.0f - t;
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
            out.m[r][c] = a.m[r][c] * u + b.m[r][c] * t;
}
static void InterpolateX87(M4& out, const M4& a, const M4& b, float t)
{
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c) {
            float from = a.m[c][r];
            out.m[r][c] = from + (b.m[c][r] - from) * t;
        }
}

int main()
{
    M4 a, b, s, x;
    for (int i = 0; i < 16; ++i) { a.m[i / 4][i % 4] = (float)i; b.m[i / 4][i % 4] = (float)(100 + 2 * i); }
    InterpolateSSE(s, a, b, 0.25f);
    InterpolateX87(x, a, b, 0.25f);
    int ok = 1;
    ok &= fabs(s.m[0][1] - (1.0f * 0.75f + 102.0f * 0.25f)) < 1e-4f;
    ok &= fabs(s.m[3][2] - (14.0f * 0.75f + 128.0f * 0.25f)) < 1e-4f;
    ok &= fabs(x.m[0][1] - (4.0f + (108.0f - 4.0f) * 0.25f)) < 1e-4f;   // transposed source element [1][0]
    ok &= fabs(x.m[2][2] - s.m[2][2]) < 1e-4f;                          // the diagonal agrees
    InterpolateSSE(s, a, b, 0.0f);
    ok &= s.m[1][3] == a.m[1][3];
    InterpolateSSE(s, a, b, 1.0f);
    ok &= s.m[2][0] == b.m[2][0];
    if (!ok) { std::printf("00a5cee0_TEST FAIL\n"); return 1; }
    std::printf("00a5cee0_TEST PASS\n");
    return 0;
}
