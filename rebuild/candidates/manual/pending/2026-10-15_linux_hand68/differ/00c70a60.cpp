// CopyAligned128(void*, const void*) -- copy one 128-byte, 16-byte-aligned block with eight SSE2 loads then eight stores
// Grade: DIFFER(94v91) -- retail is an inline-asm body (esi/edi saved and loaded from [esp+10h]/[esp+0Ch], xmm0..xmm7 in order); intrinsics give an ebp frame with `and esp,-16` and a rotated register order
// Leaf: CopyAligned128
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C70A60, length 91 bytes.
// Callees: none. Caller 0x00C70E50 (copies these 128 bytes after a 0x100-byte `rep movsd` block).
// Identity: the manifest label CPreTransposedBoneMatrix::Set is propagated; this is a free `__cdecl`
// (dst, src) helper. The retail register discipline (push esi/edi, no frame, `mov esi,src ; mov edi,dst`) is
// the inline-assembler signature, which byte purity forbids reproducing.
// Next: lane gap -- asm-only original (asm_bake at best).
#include <emmintrin.h>

void CopyAligned128(void* dst, const void* src)
{
    const __m128i* s = (const __m128i*)src;
    __m128i* d = (__m128i*)dst;
    __m128i r0 = _mm_load_si128(s + 0);
    __m128i r1 = _mm_load_si128(s + 1);
    __m128i r2 = _mm_load_si128(s + 2);
    __m128i r3 = _mm_load_si128(s + 3);
    __m128i r4 = _mm_load_si128(s + 4);
    __m128i r5 = _mm_load_si128(s + 5);
    __m128i r6 = _mm_load_si128(s + 6);
    __m128i r7 = _mm_load_si128(s + 7);
    _mm_store_si128(d + 0, r0);
    _mm_store_si128(d + 1, r1);
    _mm_store_si128(d + 2, r2);
    _mm_store_si128(d + 3, r3);
    _mm_store_si128(d + 4, r4);
    _mm_store_si128(d + 5, r5);
    _mm_store_si128(d + 6, r6);
    _mm_store_si128(d + 7, r7);
}
