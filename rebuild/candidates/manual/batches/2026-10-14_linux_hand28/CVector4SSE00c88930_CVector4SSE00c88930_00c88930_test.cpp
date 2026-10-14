#include <cstdio>
#include <xmmintrin.h>
struct CVector4SSE_00c88930 {
    __m128 m_v;
    CVector4SSE_00c88930(float x, float y, float z, float w) { m_v = _mm_set_ps(x, y, z, w); }
};
int main()
{
    CVector4SSE_00c88930 v(1.0f, 2.0f, 3.0f, 4.0f);
    const float* f = (const float*)&v.m_v;
    if (sizeof(v) != 16 || f[0] != 4.0f || f[1] != 3.0f || f[2] != 2.0f || f[3] != 1.0f) { std::printf("00c88930_TEST FAIL\n"); return 1; }
    std::printf("00c88930_TEST PASS\n"); return 0;
}
