// CopyBlock128MMX(void* dest, const void* src) -- copy one 128-byte block with sixteen MMX quadword moves, then leave MMX state (emms)
// Grade: DIFFER(144v141) -- retail is hand-written MMX (push esi/edi, esi/edi pointers, mm0..mm7 rotated twice, emms); the intrinsics form builds an aligned ebp frame and reuses mm0 with eax/ecx pointers; lane gap: inline-asm body
// Leaf: CopyBlock128MMX
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C70040, length 141 bytes, `__cdecl` (the caller at 0x00C70598 pops 8).
// Label: the catalog's CPreTransposedBoneMatrix::Set is propagated; the only caller is the sprite/text renderer group
// around 0x00C70240..0x00C70390 (descriptive name).
// Shape: retail rotates mm0..mm7 twice in strict load/store pairs -- the register naming of hand-written MMX; this
// source tries the <mmintrin.h> intrinsics form.
#include <mmintrin.h>

void __cdecl CopyBlock128MMX(void* dest, const void* src)
{
    __m64* d = (__m64*)dest;
    const __m64* s = (const __m64*)src;
    __m64 m0 = s[0];  d[0] = m0;
    __m64 m1 = s[1];  d[1] = m1;
    __m64 m2 = s[2];  d[2] = m2;
    __m64 m3 = s[3];  d[3] = m3;
    __m64 m4 = s[4];  d[4] = m4;
    __m64 m5 = s[5];  d[5] = m5;
    __m64 m6 = s[6];  d[6] = m6;
    __m64 m7 = s[7];  d[7] = m7;
    m0 = s[8];  d[8] = m0;
    m1 = s[9];  d[9] = m1;
    m2 = s[10]; d[10] = m2;
    m3 = s[11]; d[11] = m3;
    m4 = s[12]; d[12] = m4;
    m5 = s[13]; d[13] = m5;
    m6 = s[14]; d[14] = m6;
    m7 = s[15]; d[15] = m7;
    _mm_empty();
}
