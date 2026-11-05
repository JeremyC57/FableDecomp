// ConvertInt128BlockToFloat(float* dst, const long* src) -- libpk mixer: convert 128 32-bit integers to floats, with SSE2 (cvtdq2ps, 8 x 4 per loop), else SSE+MMX (cvtpi2ps, 16 x 4), else the x87 fild/fstp loop; detects the CPU first when that has not been done
// Leaf: ?ConvertInt128BlockToFloat@@
// Retail-asm: 0x00C6B3BA..0x00C6B3F5 `mov ecx,8 ; mov eax,[ebp+0Ch] ; mov edx,[ebp+8] ; jmp ; cvtdq2ps xmm0,[eax] .. cvtdq2ps xmm3,[eax+30h] ; movaps [edx],xmm0 .. movaps [edx+30h],xmm3 ; add eax,40h ; add edx,40h ; dec ecx ; jne` and 0x00C6B402..0x00C6B437 `mov ecx,10h ; mov eax,[ebp+0Ch] ; mov edx,[ebp+8] ; cvtpi2ps xmm0,[eax] .. cvtpi2ps xmm3,[eax+18h] ; movlps [edx],xmm0 .. movlps [edx+18h],xmm3 ; add eax,20h ; add edx,20h ; dec ecx ; jne ; emms` -- two hand SIMD conversion loops (15 + 16 instructions, a loop jne each, `align 16` on each loop head: the 9-byte pad behind a `jmp` and the 3-byte `lea ecx,[ecx]`: rasm1 family (f)); the xmmintrin/emmintrin spellings allocate registers and the counter differently (rasm1 probe). The CPU-flag tests, the detection call and the scalar x87 fallback loop are C++.
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared libpk/MixKernelsSIMD.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family SIMD_LIBPK.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6B3A0, 237 bytes (+3 int3), __cdecl, plain `ret`, ebp frame. Globals: the
//   CPU-detected flag 0x0143BC78, SSE2 flag 0x0143BC7D, SSE flag 0x0143BC7C; callee: the CPU detection 0x00C52E40.
//   The catalog name "LongToFloatConvert_Vectorized" is descriptive.
#include "libpk/MixKernelsSIMD.h"
extern bool g_cpuDetected;                                  // 0x0143BC78
extern bool g_cpuHasSSE;                                    // 0x0143BC7C
extern bool g_cpuHasSSE2;                                   // 0x0143BC7D
void __cdecl DetectCpu();                                   // 0x00C52E40
#pragma optimize("y", off)
void __cdecl ConvertInt128BlockToFloat(float* dst, const long* src)
{
    if (!g_cpuDetected)
        DetectCpu();
    if (g_cpuHasSSE2) {
        LIBPK_SSE2_CONVERT_INT128_TO_FLOAT(src, dst);
    } else if (g_cpuHasSSE) {
        LIBPK_SSE_CONVERT_INT128_TO_FLOAT(src, dst);
    } else {
        for (long i = 0; i < 128; ++i)          // the compiler unrolls it eight times (16 iterations in esi)
            dst[i] = (float)src[i];
    }
}