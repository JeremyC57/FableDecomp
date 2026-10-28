#include <stdio.h>
// Row00c6e1f0::ScaleAccumulate16(const short* src,unsigned long count,long* dst,short gain) -- Mix aligned eight-sample blocks into a signed accumulation buffer.
// Leaf: ?ScaleAccumulate16@Row00c6e1f0@@YAXPBFKPAJF@Z
// Flags: /GS
// Retail-asm: 0x00c6e1f0 aligned hand SIMD loop fixes xmm7, uses pmulhw/punpcklwd/psrad/paddd and align16 loop. Intrinsics allocate different registers (existing verified sibling00c6d370); preserve original kernel and ordinary C++ local initialization.
// Identity: own mixer callers and exact signed high-word product, then double sign extension. Descriptive helper name; separate namespace distinguishes original copies. Count must be positive, buffers aligned16.
#ifndef _MSC_VER
#define __cdecl
#endif
namespace Row00c6e1f0{
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
__asm {
 mov ecx,count
 mov eax,src
 mov edx,dst
 movdqa xmm7,gains
 align 16
 next_block:
 movdqa xmm0,[eax]
 pmulhw xmm0,xmm7
 pxor xmm1,xmm1
 pxor xmm2,xmm2
 punpcklwd xmm1,xmm0
 punpckhwd xmm2,xmm0
 psrad xmm1,15
 psrad xmm2,15
 paddd xmm1,[edx]
 paddd xmm2,[edx+10h]
 movdqa [edx],xmm1
 movdqa [edx+10h],xmm2
 add eax,10h
 add edx,20h
 dec ecx
 jne next_block
}
#else
int gains[8];for(int i=0;i<8;++i)gains[i]=gain;
for(unsigned long block=0;block<count;++block){for(int lane=0;lane<8;++lane){int product=(int)src[block*8+lane]*gains[lane];int high=product/65536;if(product<0&&product%65536)--high;dst[block*8+lane]+=high*2;}}
#endif
}
}

#ifdef _MSC_VER
#define ALIGNED __declspec(align(16))
#else
#define ALIGNED __attribute__((aligned(16)))
#endif
int main(){ALIGNED short input[16];ALIGNED long output[16];for(int i=0;i<16;++i){input[i]=(short)((i%2?-1:1)*(i+1)*1024);output[i]=100+i;}
Row00c6e1f0::ScaleAccumulate16(input,2,output,16384);bool ok=true;for(int j=0;j<16;++j){int product=(int)input[j]*16384;int high=product/65536;if(product<0&&product%65536)--high;ok=ok&&output[j]==100+j+high*2;}for(int k=0;k<8;++k){input[k]=(short)(k*4096-16384);output[k]=0;}Row00c6e1f0::ScaleAccumulate16(input,1,output,-16384);for(int m=0;m<8;++m){int product=(int)input[m]*-16384;int high=product/65536;if(product<0&&product%65536)--high;ok=ok&&output[m]==high*2;}puts(ok?"00c6e1f0_TEST PASS":"00c6e1f0_TEST FAIL");return ok?0:1;}