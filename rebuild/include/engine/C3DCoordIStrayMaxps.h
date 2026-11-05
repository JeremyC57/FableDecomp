// GF_STRAY_MAXPS_XMM0_XMM1() -- the three bytes 0F 5F C1 (`maxps xmm0,xmm1`) that retail's C3DCoordI::operator-=
// (0x00A13C20, ??ZC3DCoordI@@QAEABV0@ABVC3DVector@@@Z) holds between its last store and `pop edi`, on registers the
// function never loads.
// Retail-asm-helper: GF_STRAY_MAXPS_XMM0_XMM1
// Retail-asm: 0x00A13C4F `maxps xmm0, xmm1` (0F 5F C1) after the Z store of an otherwise plain x87 body. No C++ route emits an
//   SSE instruction on registers the function never loads: `_mm_max_ps` (<xmmintrin.h>) on uninitialised, unused or
//   discarded __m128 locals is dropped as dead code, and on live operands it loads them first and stores the result (the
//   batch ATTEMPTS.tsv, site 0x00A13C4F). The sequence is unique in Fable.exe; whether it was a source statement or a
//   post-build patch is open (the bytes are what retail ran).
// Semantics: none a C++ program can observe (xmm0 is dead after the instruction; the function returns in eax). MAXPS raises
//   no exception on any input under the default MXCSR (exceptions masked).
// Portable twin (FABLE_PORTABLE): nothing. Test: rebuild/tests/portable/test_C3DCoordIStrayMaxps.cpp.
// Use: write each macro call as a statement on a line of its own: the retail branch expands to `__asm` statements, and an
//   `__asm` statement runs to the end of its source line (anything after it on that line is read as assembler).
// Proven (2026-11-05 round 2): VC7.1 drops a `_mm_max_ps` whose result is dead and stores one that is live, and adds an
//   `and esp,-16` frame either way (10 intrinsic attempts, rebuild/candidates/manual/naming/2026-11-05_asm_helpers_r2/ATTEMPTS_SIMD_HELPERS.tsv); retail runs a bare
//   `maxps xmm0,xmm1` (0x00A13C4F) in a function with no realignment.
#ifndef FABLE_ENGINE_C3DCOORDISTRAYMAXPS_H
#define FABLE_ENGINE_C3DCOORDISTRAYMAXPS_H

#ifdef FABLE_PORTABLE
#define GF_STRAY_MAXPS_XMM0_XMM1() ((void)0)
#else
#define GF_STRAY_MAXPS_XMM0_XMM1() \
    __asm maxps xmm0, xmm1
#endif

#endif  // FABLE_ENGINE_C3DCOORDISTRAYMAXPS_H
