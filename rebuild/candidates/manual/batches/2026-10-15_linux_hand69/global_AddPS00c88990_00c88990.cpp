// AddPS_00c88990(const SVector4_00c88990&, const SVector4_00c88990&) -- the SSE sum of two four-float vectors (both by reference, the result by hidden pointer)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C88990, length 31 bytes. No callees.
// Leaf: AddPS_00c88990
// The manifest label "CQuadric::operator+=" is a BSim port (0.85) and is rejected: the real CQuadric is eleven
// doubles; this is the `addps` twin of the landed MulPS_00c889b0 (0x00C889B0, `mulps`) right after it.
// Shape: `__cdecl` with the hidden result pointer at [ebp+8]; the aligned __m128 member makes VC build a
// frame-pointer prologue with `and esp,-16`. The __m128 is wrapped in a struct so it returns through the hidden
// pointer (a bare __m128 returns in xmm0).
#include <xmmintrin.h>

struct SVector4_00c88990 { __m128 m; };

SVector4_00c88990 __cdecl AddPS_00c88990(const SVector4_00c88990& a, const SVector4_00c88990& b)
{
    SVector4_00c88990 r;
    r.m = _mm_add_ps(a.m, b.m);
    return r;
}
