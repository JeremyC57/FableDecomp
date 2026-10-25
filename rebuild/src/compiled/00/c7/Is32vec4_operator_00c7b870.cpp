// Is32vec4::operator+=(const I32vec4& a) -- dvec.h: add the four 32-bit lanes of a into this (SSE2 paddd) and return *this
// Leaf: ??YIs32vec4@@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C7B870, length 54 bytes (+ int3 padding), `ret 4`. 461 call sites in
// the libpk software mixer's SSE2 convolution kernels (0x00C7C053..). The manifest label "operator+" (__fastcall) is a
// guess; the shape is the Intel class library's `return *this = (Is32vec4) _mm_add_epi32(vec, a);`.
// Shape: `movdqa xmm0,[a] ; movdqa xmm1,[this] ; paddd xmm1,xmm0 ; movdqa [this],xmm1` with this in eax; the
// __m128i union-of-arrays member makes /GS build its cookie in a 16-byte aligned frame (`and esp,-16 ; sub esp,10h`,
// cookie 0x0139C8A8 at +0Ch, __security_check_cookie 0x00BFE9F9). The MMX twin is Is32vec2::operator+= 0x00C78630.
// Family: the landed 0x00C7B7D0 / 0x00C7B810 / 0x00C7B840 ("CVectorSSE" constructors) are the M128 / I32vec4 /
// Is32vec4 (__m128i) constructors and 0x00C7B8B0 / 0x00C7B8F0 ("CBitfield::RightShiftCopy/RightShift") are
// Is32vec4::operator>>(int) / operator>>=(int) of the same dvec.h class.
// Library: libpk software mixer (no PDB/RTTI); class names are the Intel dvec.h spellings (RTM VC7.1 has no dvec.h).
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
    Is32vec4& operator+=(const I32vec4& a);
};

Is32vec4& Is32vec4::operator+=(const I32vec4& a)
{
    return *this = (Is32vec4)_mm_add_epi32(vec, a);
}