// ScaleAccumulate16MMX(const short* src, unsigned long count, long* dst, short gain) -- libpk mixer (MMX path): accumulate `count` blocks of eight 16-bit samples into the 32-bit mix buffer, each sample scaled by a Q15 gain (pmulhw, sign-extended, >> 15), two quadwords per iteration
// Leaf: ?ScaleAccumulate16MMX@@
// Retail-asm: 0x00C6F11C..0x00C6F18C `mov ecx,[ebx+0Ch] ; mov eax,[ebx+8] ; mov edx,[ebx+10h] ; movq mm7,[ebp-20h] ; movq mm0,[eax] ; pmulhw mm0,mm7 ; pxor mm1,mm1 ; pxor mm2,mm2 ; punpcklwd mm1,mm0 ; punpckhwd mm2,mm0 ; psrad mm1,0Fh ; psrad mm2,0Fh ; paddd mm1,[edx] ; paddd mm2,[edx+8] ; movq [edx],mm1 ; movq [edx+8],mm2 ; movq mm3,[eax+8] .. movq [edx+18h],mm5 ; add eax,10h ; add edx,20h ; dec ecx ; jne ; emms` -- a hand MMX kernel (34 instructions, the loop jne, `align 16` on the loop head: rasm1 family (f)) over the arguments and the aligned gain vector local; mmintrin gives different register allocation (rasm1 probe). The four word stores that fill the gain vector, the /GS cookie and the 16-byte frame alignment are C++ (an `__declspec(align(16))` local under /GS).
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared libpk/MixKernelsSIMD.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family SIMD_LIBPK.
// Retail: Fable.exe (TLC, VC7.1, /GS) function @ 0x00C6F0F0, 173 bytes (+3 int3), __cdecl, plain `ret`; the arguments are read
//   through ebx (the saved caller esp) because esp is realigned for the local. Cookie 0x0139C8A8, __security_check_cookie
//   0x00BFE9F9. Twin on the SSE2 path: 0x00C6D370. The catalog name "Audio_ProcessSSE64BitMultiply" is descriptive.
// Flags: /GS
#include "libpk/MixKernelsSIMD.h"
void __cdecl ScaleAccumulate16MMX(const short* src, unsigned long count, long* dst, short gain)
{
    __declspec(align(16)) short gains[4];
    gains[0] = gain;
    gains[1] = gain;
    gains[2] = gain;
    gains[3] = gain;
    LIBPK_MMX_SCALE_ACCUMULATE16(count, src, dst, gains);
}