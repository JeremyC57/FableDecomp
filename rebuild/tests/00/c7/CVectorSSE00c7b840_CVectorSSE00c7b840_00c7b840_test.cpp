#include <cstdio>
#include <emmintrin.h>
struct CVectorSSE_00c7b840 {
    __m128i m_v;
    CVectorSSE_00c7b840(__m128i value) { m_v = value; }
};
int main()
{
    __m128i v = _mm_set_epi32(1, 2, 3, 4);
    CVectorSSE_00c7b840 o(v);
    const int* w = (const int*)&o.m_v;
    if (sizeof(o) != 16 || w[0] != 4 || w[1] != 3 || w[2] != 2 || w[3] != 1) { std::printf("00c7b840_TEST FAIL\n"); return 1; }
    std::printf("00c7b840_TEST PASS\n"); return 0;
}