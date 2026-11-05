// AddInt128Block_00c5d3f0(const long* src, long* dst) -- libpk mixer: add 128 packed 32-bit integers from src into dst with SSE2 (eight iterations of four aligned 16-byte paddd)
// Leaf: ?AddInt128Block_00c5d3f0@@
// Retail-asm: 0x00C5D3F3..0x00C5D440 `mov ecx,8 ; mov eax,[ebp+8] ; mov edx,[ebp+0Ch] ; movdqa xmm0,[eax] .. movdqa xmm3,[eax+30h] ; paddd xmm0,[edx] .. paddd xmm3,[edx+30h] ; movdqa [edx],xmm0 .. movdqa [edx+30h],xmm3 ; add eax,40h ; add edx,40h ; dec ecx ; jne` -- a hand SSE2 accumulate loop (19 instructions, the loop jne, an `align 16` on the loop head: rasm1 family (f)): fixed eax/edx/ecx, memory-operand paddd, no spill; the emmintrin spelling (rasm1 probe) allocates registers and the loop counter differently and gives no bare `push ebp ; mov ebp,esp` frame without locals. The whole function is the block; the frame is compiler-emitted.
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared libpk/MixKernelsSIMD.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family SIMD_LIBPK.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D3F0, 86 bytes, __stdcall (`ret 8`); no direct callers (a mixer kernel selected through a table).
//   The __cdecl twin is 0x00C5D370 (the other __stdcall copy: 0x00C5D4B0); named per address because each copy is a separate retail function.
//   The catalog name "IntBlock128_AddInPlace_c5d3f0" is descriptive.
#include "libpk/MixKernelsSIMD.h"
void __stdcall AddInt128Block_00c5d3f0(const long* src, long* dst)
{
    LIBPK_SSE2_ADD_INT128_BLOCK(src, dst);
}