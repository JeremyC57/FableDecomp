// MulPS_00c889b0(__m128 a, __m128 b) -- the SSE product of two four-float vectors (both passed by hidden reference, the result by hidden pointer)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C889B0, length 31 bytes.
// Leaf: MulPS_00c889b0
// The chunk label (CMovie::GetSize) is propagated; named by effect. Shape: a frame-pointer prologue with `and esp,-16` for the aligned type, `movaps` / `mulps`, `ret`.
#include <xmmintrin.h>

struct SVector4_00c889b0 { __m128 m; };

SVector4_00c889b0 __cdecl MulPS_00c889b0(const SVector4_00c889b0& a, const SVector4_00c889b0& b)
{
    SVector4_00c889b0 r;
    r.m = _mm_mul_ps(a.m, b.m);
    return r;
}