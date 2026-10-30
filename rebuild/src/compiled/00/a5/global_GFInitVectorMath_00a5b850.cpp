// NVectorMath::GFInitVectorMath() -- detect SSE with CPUID (leaf 1, EDX bit 25) and set the g_SSEAvailable byte
// Leaf: ?GFInitVectorMath@NVectorMath@@YAXXZ
// Retail: Fable.exe 0x00A5B850, 25 bytes: push ebx ; mov eax,1 ; cpuid ; test edx,2000000h ; je ; mov byte [0x013D2880],1 ;
//   pop ebx ; ret. Name: both PDBs `?GFInitVectorMath@NVectorMath@@YAXXZ`, first in the NVectorMath block (GFClear4DVectors,
//   GFNormalise3DVectors, GFInterpolateVectors x3, GFFinishPartialSlerp follow it); g_SSEAvailable is the byte the landed
//   SSE kernels test (binding ?g_SSEAvailable@@3_NA from 0x00A5C300).
// Retail-asm: 0x00A5B856 `cpuid` -- VC7.1 has no __cpuid intrinsic (it arrived with VC8's intrin.h) and no C++ form emits the
//   instruction; the `push ebx`/`pop ebx` pair is the compiler saving ebx because the asm block's cpuid clobbers it.
extern bool g_SSEAvailable;                                 // 0x013D2880

namespace NVectorMath
{
    void GFInitVectorMath()
    {
        __asm
        {
            mov     eax, 1
            cpuid
            test    edx, 0x2000000
            je      no_sse
            mov     byte ptr [g_SSEAvailable], 1
        no_sse:
        }
    }
}
