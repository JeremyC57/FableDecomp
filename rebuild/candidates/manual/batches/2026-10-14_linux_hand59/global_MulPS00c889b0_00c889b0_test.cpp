#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <xmmintrin.h>
struct SV { __m128 m; }; static SV Mul(const SV& a, const SV& b) { SV r; r.m = _mm_mul_ps(a.m, b.m); return r; }

int main() {
    SV a, b; float* fa = (float*)&a.m; float* fb = (float*)&b.m; for (int i = 0; i < 4; ++i) { fa[i] = (float)(i + 1); fb[i] = 2.0f; } SV r = Mul(a, b); float* fr = (float*)&r.m; if (fr[0] != 2 || fr[1] != 4 || fr[2] != 6 || fr[3] != 8) { std::printf("00c889b0_TEST FAIL\n"); return 1; }
    std::printf("00c889b0_TEST PASS\n"); return 0;
}
