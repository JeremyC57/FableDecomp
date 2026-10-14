// CBitfield::RightShiftCopy(CBitfield* result, long count) const -- arithmetic right shift of the 128-bit field into another field (SSE2)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C7B8B0, length 53 bytes (+ int3 padding).
// Callees: __security_check_cookie @ 0x00BFE9F9 (the /GS cookie @ 0x0139C8A8 lives in a 16-byte
// aligned frame: `and esp,-16 ; sub esp,10h`).
// Shape: `movdqa xmm1,[this] ; movd xmm0,count ; psrad xmm1,xmm0 ; movdqa [result],xmm1`, i.e.
// _mm_sra_epi32 on an aligned __m128i member with the count widened through _mm_cvtsi32_si128.
#include <emmintrin.h>

struct __declspec(align(16)) CBitfield {
    __m128i m_bits;
    void RightShiftCopy(CBitfield* result, long count) const;
};

void CBitfield::RightShiftCopy(CBitfield* result, long count) const
{
    result->m_bits = _mm_sra_epi32(m_bits, _mm_cvtsi32_si128(count));
}
