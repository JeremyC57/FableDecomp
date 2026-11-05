// GF_CPUID_DETECT_SSE(flag) -- NVectorMath::GFInitVectorMath's SSE probe (FableWin.pdb ?GFInitVectorMath@NVectorMath@@YAXXZ):
// CPUID leaf 1, EDX bit 25 (SSE); when set, the byte `flag` (g_SSEAvailable 0x013D2880) becomes 1. An inline-assembly block.
// Retail-asm-helper: GF_CPUID_DETECT_SSE
// Retail-asm: 0x00A5B851 `mov eax,1` + 0x00A5B856 `cpuid` + 0x00A5B858 `test edx,2000000h` + 0x00A5B860 `mov byte ptr
//   [013D2880h],1` (GFInitVectorMath 0x00A5B850; the `push ebx` / `pop ebx` around it is the compiler saving the ebx that
//   cpuid clobbers). VC7.1 has no __cpuid intrinsic (it arrived with VC8's <intrin.h>; no $VC71/include header declares one)
//   and no C++ form emits cpuid, so the original is inline asm.
// Semantics: flag = 1 when CPUID leaf 1 reports SSE (EDX bit 25); otherwise flag is left unchanged (retail's static is
//   zero-initialised).
// Portable twin (FABLE_PORTABLE): Strategy C, as engine/GFGetCycleCount.h -- no plain C++ runs CPUID: __get_cpuid (GCC / Clang
//   <cpuid.h>) or __cpuid (MSVC >= VC8 <intrin.h>) on x86 / x86-64; elsewhere the flag is left unchanged (no SSE: every
//   NVectorMath function takes its C++ loop, and the SSE helpers' portable twins compute the same results either way). VC7.1
//   has no intrinsic, so its portable build keeps the retail block. Test: rebuild/tests/portable/test_GFInitVectorMathCPUID.cpp
//   (asm path and twin report the same bit on the test host).
// Use: write each macro call as a statement on a line of its own: the retail branch expands to `__asm` statements, and an
//   `__asm` statement runs to the end of its source line (anything after it on that line is read as assembler).
#ifndef FABLE_ENGINE_GFINITVECTORMATHCPUID_H
#define FABLE_ENGINE_GFINITVECTORMATHCPUID_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
#if defined(_MSC_VER) && _MSC_VER >= 1400
#include <intrin.h>
#elif !defined(_MSC_VER) && (defined(__i386__) || defined(__x86_64__))
#include <cpuid.h>
#endif
namespace fable_portable
{
    inline void CpuidDetectSSE(bool& flag)
    {
#if defined(_MSC_VER) && _MSC_VER >= 1400
        int regs[4];
        __cpuid(regs, 1);
        if (regs[3] & 0x2000000)
            flag = 1;
#elif defined(_MSC_VER)
        __asm {                                         // VC7.1: no intrinsic, no other route to CPUID (see above)
            mov eax, 1
            cpuid
            test edx, 0x2000000
            je fable_portable_no_sse
            mov eax, flag
            mov byte ptr [eax], 1
        fable_portable_no_sse:
        }
#elif defined(__i386__) || defined(__x86_64__)
        unsigned int a, b, c, d;
        if (__get_cpuid(1, &a, &b, &c, &d) && (d & 0x2000000u))
            flag = 1;
#else
        (void)flag;
#endif
    }
}
#endif

#ifdef FABLE_PORTABLE
#define GF_CPUID_DETECT_SSE(flag) fable_portable::CpuidDetectSSE(flag)
#else
#define GF_CPUID_DETECT_SSE(flag) \
    __asm mov eax, 1 \
    __asm cpuid \
    __asm test edx, 0x2000000 \
    __asm je gf_cpuid_no_sse \
    __asm mov byte ptr [flag], 1 \
    __asm gf_cpuid_no_sse:
#endif

#endif  // FABLE_ENGINE_GFINITVECTORMATHCPUID_H
