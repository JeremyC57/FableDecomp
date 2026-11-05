// Row00c6ecd0::ScaleAccumulate16(const short* src,unsigned long count,long* dst,short gain) -- Mix aligned eight-sample blocks into a signed accumulation buffer.
// Leaf: ?ScaleAccumulate16@Row00c6ecd0@@YAXPBFKPAJF@Z
// Flags: /GS
// Retail-asm: 0x00c6ecd0 aligned hand SIMD loop fixes xmm7, uses pmulhw/punpcklwd/psrad/paddd and align16 loop. Intrinsics allocate different registers (existing verified sibling00c6d370); preserve original kernel and ordinary C++ local initialization.
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared libpk/MixKernelsSIMD.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family SIMD_LIBPK.
// Identity: own mixer callers and exact signed high-word product, then double sign extension. Descriptive helper name; separate namespace distinguishes original copies. Count must be positive, buffers aligned16.
#include "libpk/MixKernelsSIMD.h"
#ifndef _MSC_VER
#define __cdecl
#endif
namespace Row00c6ecd0{
void __cdecl ScaleAccumulate16(const short* src,unsigned long count,long* dst,short gain){
#ifdef _MSC_VER
__declspec(align(16)) short gains[8];
gains[0]=gain;
gains[1]=gain;
gains[2]=gain;
gains[3]=gain;
gains[4]=gain;
gains[5]=gain;
gains[6]=gain;
gains[7]=gain;
LIBPK_SSE2_SCALE_ACCUMULATE16(count, src, dst, gains);
#else
int gains[8];for(int i=0;i<8;++i)gains[i]=gain;
for(unsigned long block=0;block<count;++block){for(int lane=0;lane<8;++lane){int product=(int)src[block*8+lane]*gains[lane];int high=product/65536;if(product<0&&product%65536)--high;dst[block*8+lane]+=high*2;}}
#endif
}
}