// Row00c6ed70::ScaleAccumulate16Ramp(const short* src,unsigned long count,long* dst,short initial,short step) -- Mix aligned eight-sample blocks into a signed accumulation buffer (gain ramp: per-lane initial + i*step, +8*step per block, saturating).
// Leaf: ScaleAccumulate16Ramp
// Flags: /GS
// Retail-asm: 0x00C6EE14 `pmulhw xmm0,xmm7` -- retail runs this aligned hand-written SSE2 loop (movdqa/pmulhw/paddsw/punpcklwd/psrad/paddd, `align 16` loop head) as inline asm; intrinsics allocate other xmm registers (verified sibling 0x00C6D370).
// Identity: own mixer callers and exact signed high-word product, then double sign extension. Descriptive helper name; separate namespace distinguishes original copies. Count must be positive, buffers aligned16.
// Shape (retry1): the parked "ABI gap" (initial gain in ecx, four stack arguments, caller cleans) is VC7.1's own private convention
//   for a same-TU static: with the same-TU caller below it passes `initial` in ecx itself. `initial` is declared before `step` --
//   the declaration order fixes the operand order of gains[1] (`lea edx,[ecx+eax]`); step-first gives `[eax+ecx]`.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6ED70, 240 bytes, aligned /GS frame (`and esp,-16` through ebx), plain `ret`.
#ifndef _MSC_VER
#define __cdecl
#endif
namespace Row00c6ed70{
static __declspec(noinline) void __cdecl ScaleAccumulate16Ramp(const short* src,unsigned long count,long* dst,short initial,short step){
#ifdef _MSC_VER
__declspec(align(16)) short gains[8];__declspec(align(16)) short increments[8];
gains[0]=(short)(initial+0*step);
gains[1]=(short)(initial+1*step);
gains[2]=(short)(initial+2*step);
gains[3]=(short)(initial+3*step);
gains[4]=(short)(initial+4*step);
gains[5]=(short)(initial+5*step);
gains[6]=(short)(initial+6*step);
gains[7]=(short)(initial+7*step);
increments[0]=(short)(step*8);
increments[1]=(short)(step*8);
increments[2]=(short)(step*8);
increments[3]=(short)(step*8);
increments[4]=(short)(step*8);
increments[5]=(short)(step*8);
increments[6]=(short)(step*8);
increments[7]=(short)(step*8);
__asm {
 mov ecx,count
 mov eax,src
 mov edx,dst
 movdqa xmm7,gains
 movdqa xmm6,increments
 align 16
 next_block:
 movdqa xmm0,[eax]
 pmulhw xmm0,xmm7
 paddsw xmm7,xmm6
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
int gains[8];for(int i=0;i<8;++i)gains[i]=(short)(initial+i*step);
for(unsigned long block=0;block<count;++block){for(int lane=0;lane<8;++lane){int product=(int)src[block*8+lane]*gains[lane];int high=product/65536;if(product<0&&product%65536)--high;dst[block*8+lane]+=high*2;int next=gains[lane]+(short)(step*8);gains[lane]=next>32767?32767:(next < -32768?-32768:next);}}
#endif
}
// Same-TU caller (stand-in for this copy's caller: four stack arguments, `ret 10h`, the block count fixed at 16). Not graded.
void __stdcall MixBlock16(const short* src,long* dst,short initial,short step){
ScaleAccumulate16Ramp(src,16,dst,initial,step);
}
}