// Row00c5d150::ScaleAccumulate16(const short* src,unsigned long count,long* dst,short gain) -- Mix aligned eight-sample blocks into a signed accumulation buffer.
// Leaf: ?ScaleAccumulate16@Row00c5d150@@YAXPBFKPAJF@Z
// Flags: /GS
// Retail-asm: 0x00c5d150 aligned hand SIMD loop fixes xmm7, uses pmulhw/punpcklwd/psrad/paddd and align16 loop. Intrinsics allocate different registers (existing verified sibling00c6d370); preserve original kernel and ordinary C++ local initialization.
// Identity: own mixer callers and exact signed high-word product, then double sign extension. Descriptive helper name; separate namespace distinguishes original copies. Count must be positive, buffers aligned16.
#ifndef _MSC_VER
#define __cdecl
#endif
namespace Row00c5d150{
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