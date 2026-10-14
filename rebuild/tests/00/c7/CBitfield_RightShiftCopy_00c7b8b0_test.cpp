#include <stdio.h>
#include <emmintrin.h>
struct __declspec(align(16)) CBitfield { __m128i m_bits; void RightShiftCopy(CBitfield* result, long count) const; };
void CBitfield::RightShiftCopy(CBitfield* result, long count) const { result->m_bits = _mm_sra_epi32(m_bits, _mm_cvtsi32_si128(count)); }
int main()
{
    CBitfield a, b; long* src = (long*)&a.m_bits; long* dst = (long*)&b.m_bits;
    src[0] = 0x100; src[1] = -64; src[2] = 7; src[3] = (long)0x80000000;
    dst[0] = dst[1] = dst[2] = dst[3] = 5;
    a.RightShiftCopy(&b, 3);
    bool ok = dst[0] == 0x20 && dst[1] == -8 && dst[2] == 0 && dst[3] == (long)0xF0000000 && src[0] == 0x100 && src[1] == -64;
    if (!ok) { printf("00c7b8b0_TEST FAIL\n"); return 1; }
    printf("00c7b8b0_TEST PASS\n"); return 0;
}