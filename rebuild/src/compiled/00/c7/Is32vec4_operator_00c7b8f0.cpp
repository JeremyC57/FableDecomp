// Is32vec4::operator>>=(int count) -- dvec.h: arithmetic right shift of the four signed 32-bit lanes in place by a run-time count (SSE2 psrad) and return *this
// Leaf: ??_2Is32vec4@@QAEAAV0@H@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C7B8F0, 52 bytes (+ int3 padding), thiscall `ret 4`, returns `this` in eax.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 445 "0xC7B8F0 Is32vec4::operator>>= is declared void but returns *this";
//   agent445's landed Is32vec4::operator+= 0x00C7B870 names this family): `mov eax,ecx` before the load keeps `this` as the
//   return value, which the landed `void CBitfield::RightShift(long)` did not model; the body is the Intel class library's
//   `return *this = (Is32vec4) _mm_srai_epi32(vec, count);` (a non-constant count gives `movd xmm0,count ; psrad`). 77
//   call sites in the libpk software mixer's SSE2 kernels (0x00C7E8A2..). The CBitfield label is a guess.
// Shape: the __m128i member makes /GS build its cookie in a 16-byte aligned frame (`and esp,-16 ; sub esp,10h`, cookie
//   0x0139C8A8 at +0Ch, __security_check_cookie 0x00BFE9F9). Library: libpk (no PDB/RTTI); the RTM VC7.1 include set has
//   no dvec.h, so the classes are the dvec.h spellings declared here.
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
    Is32vec4& operator>>=(int count);
};

Is32vec4& Is32vec4::operator>>=(int count)
{
    return *this = (Is32vec4)_mm_srai_epi32(vec, count);
}
