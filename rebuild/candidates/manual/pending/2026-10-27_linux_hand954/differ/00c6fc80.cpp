// CUnprovenMixKernel_00c6fc80(start, source, quadPairs, accumulator, step) -- libpk MMX mixer: scale 8 16-bit samples per pass by a volume ramp (pmulhw, the ramp advanced by 4 x step after each quad) and add them, widened to 32 bits, into the accumulator
// Grade: DIFFER(170v211) -- libpk MMX kernel: inline-assembly block in a C shell (ecx input, caller-cleaned ret)
// Leaf: ?CUnprovenMixKernel_00c6fc80@@YAXFPB_JHPA_JF@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6FC80, 211 bytes; aligned frame (`and esp,-16`), /GS cookie, `emms`, plain `ret`.
// Identity (descriptive): one of five byte-identical copies (0x00C5BEF0, 0x00C6AAE0, 0x00C6F1A0, 0x00C6F800, 0x00C6FC80) of
//   one libpk kernel, each called only from the thunks whose calls sit at 0x00C6FD6C / 0x00C6FD9C; libpk has no PDB.
// Not reconstructed: retail reads the ramp start from ecx before any assignment and returns with `ret` while the
//   callers pop four stack words, which no VC7.1 calling convention produces -- the body is an inline-assembler block in
//   a C shell. This intrinsics version computes the same result (the differ copy, never a landing candidate).
#include <mmintrin.h>
void CUnprovenMixKernel_00c6fc80(short start, const __int64* source, int quadPairs, __int64* accumulator, short step)
{
    __m64 ramp = _mm_set_pi16((short)(start + 3 * step), (short)(start + 2 * step), (short)(start + step), start);
    __m64 inc = _mm_set1_pi16((short)(4 * step));
    const __m64* src = (const __m64*)source;
    __m64* acc = (__m64*)accumulator;
    for (; quadPairs != 0; --quadPairs) {
        for (int q = 0; q < 2; ++q) {
            __m64 s = _mm_mulhi_pi16(src[q], ramp);
            ramp = _mm_adds_pi16(ramp, inc);
            __m64 lo = _mm_srai_pi32(_mm_unpacklo_pi16(_mm_setzero_si64(), s), 15);
            __m64 hi = _mm_srai_pi32(_mm_unpackhi_pi16(_mm_setzero_si64(), s), 15);
            acc[2 * q] = _mm_add_pi32(lo, acc[2 * q]);
            acc[2 * q + 1] = _mm_add_pi32(hi, acc[2 * q + 1]);
        }
        src += 2;
        acc += 4;
    }
    _mm_empty();
}
