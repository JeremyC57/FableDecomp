// Behaviour test for AddPS_00c88990 @ 0x00C88990 (SSE add of two four-float vectors)
#include <cstdio>
#include <xmmintrin.h>

struct SVector4_00c88990 { __m128 m; };

SVector4_00c88990 AddPS_00c88990(const SVector4_00c88990& a, const SVector4_00c88990& b)
{
    SVector4_00c88990 r;
    r.m = _mm_add_ps(a.m, b.m);
    return r;
}

int main()
{
    SVector4_00c88990 a, b;
    a.m = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
    b.m = _mm_set_ps(0.5f, 0.25f, -2.0f, 10.0f);
    SVector4_00c88990 r = AddPS_00c88990(a, b);
    float out[4];
    _mm_storeu_ps(out, r.m);
    if (out[0] != 11.0f || out[1] != 0.0f || out[2] != 3.25f || out[3] != 4.5f) { std::printf("00c88990_TEST FAIL\n"); return 1; }
    std::printf("00c88990_TEST PASS\n");
    return 0;
}