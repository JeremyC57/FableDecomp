#include <stdio.h>
#include <emmintrin.h>
struct __declspec(align(16)) CBitfield { __m128i m_bits; void RightShift(long count); };
void CBitfield::RightShift(long count) { m_bits = _mm_sra_epi32(m_bits, _mm_cvtsi32_si128(count)); }
int main()
{
    CBitfield b; long* lanes = (long*)&b.m_bits;
    lanes[0] = 0x100; lanes[1] = -64; lanes[2] = 7; lanes[3] = (long)0x80000000;
    b.RightShift(3);
    bool ok = lanes[0] == 0x20 && lanes[1] == -8 && lanes[2] == 0 && lanes[3] == (long)0xF0000000;
    if (!ok) { printf("00c7b8f0_TEST FAIL\n"); return 1; }
    printf("00c7b8f0_TEST PASS\n"); return 0;
}