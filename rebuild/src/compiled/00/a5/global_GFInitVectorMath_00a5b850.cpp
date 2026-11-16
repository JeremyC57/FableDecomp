// NVectorMath::GFInitVectorMath() -- detect SSE with CPUID (leaf 1, EDX bit 25) and set the GEnableSIMD byte
// Leaf: ?GFInitVectorMath@NVectorMath@@YAXXZ
// Retail: Fable.exe 0x00A5B850, 25 bytes: push ebx ; mov eax,1 ; cpuid ; test edx,2000000h ; je ; mov byte [0x013D2880],1 ;
//   pop ebx ; ret. Name: both PDBs `?GFInitVectorMath@NVectorMath@@YAXXZ`, first in the NVectorMath block (GFClear4DVectors,
//   GFNormalise3DVectors, GFInterpolateVectors x3, GFFinishPartialSlerp follow it); GEnableSIMD is the byte the landed
//   SSE kernels test (binding ?GEnableSIMD@@3_NA from 0x00A5C300).
// Retail-asm: 0x00A5B856 `cpuid` -- VC7.1 has no __cpuid intrinsic (it arrived with VC8's intrin.h) and no C++ form emits the
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared engine/GFInitVectorMathCPUID.h (byte-matching path + FABLE_PORTABLE twin);
//   the row has no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family CPUID.
//   instruction; the `push ebx`/`pop ebx` pair is the compiler saving ebx because the asm block's cpuid clobbers it.
#include "engine/GFInitVectorMathCPUID.h"
extern bool GEnableSIMD;                                 // 0x013D2880

namespace NVectorMath
{
    void GFInitVectorMath()
    {
        GF_CPUID_DETECT_SSE(GEnableSIMD);       // portable build: GEnableSIMD stays false (the C++ paths)
    }
}
