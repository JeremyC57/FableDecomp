// Is32vec4::operator>>(int count) -- dvec.h: the four signed 32-bit lanes arithmetic-shifted right by a run-time count (SSE2 psrad), returned by value
// Leaf: ??5Is32vec4@@QAE?AV0@H@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C7B8B0, 53 bytes (+ int3 padding), thiscall `ret 8` (hidden result pointer
//   at [ebp+8], count at [ebp+0Ch]; returns the result pointer).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 445 "0xC7B7D0..0xC88A40 neighbours are dvec/fvec ctors/operators"; agent445's
//   landed Is32vec4::operator+= 0x00C7B870 note: 0x00C7B8B0 / 0x00C7B8F0 are Is32vec4::operator>>(int) / operator>>=(int)):
//   the same `movd ; psrad` as the in-place operator>>= 0x00C7B8F0 (renamed this pass) but stored into the by-value result,
//   the Intel class library's `return _mm_srai_epi32(vec, count);`. Callers are the libpk software mixer's SSE2 kernels
//   (0x00C7C2E1 ..). The landed CBitfield::RightShiftCopy(result, count) const is a guess with the same bytes.
// Shape: the /GS cookie in the 16-byte aligned frame (`and esp,-16 ; sub esp,10h`, __security_check_cookie 0x00BFE9F9).
#include <emmintrin.h>

class M128 {
protected:
    __m128i vec;                                       // +0x00
public:
    M128() {}
    M128(__m128i mm) { vec = mm; }
    operator __m128i() const { return vec; }
};

class I32vec4 : public M128 {
public:
    I32vec4() {}
    I32vec4(__m128i mm) : M128(mm) {}
};

class Is32vec4 : public I32vec4 {
public:
    Is32vec4() {}
    Is32vec4(__m128i mm) : I32vec4(mm) {}
    Is32vec4 operator>>(int count);
};

Is32vec4 Is32vec4::operator>>(int count)
{
    return _mm_srai_epi32(vec, count);
}
