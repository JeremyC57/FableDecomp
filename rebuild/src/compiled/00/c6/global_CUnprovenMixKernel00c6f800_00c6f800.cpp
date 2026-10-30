// CUnprovenMixKernel_00c6f800(const short* src, unsigned long count, long* dst, short initial, short step) -- libpk MMX mixer: scale 8 16-bit samples per pass by a 4-lane volume ramp (pmulhw; the ramp advances by 4*step after each quad, saturating) and add them, widened to 32 bits, into the accumulator
// Leaf: CUnprovenMixKernel_00c6f800
// Flags: /GS
// Retail-asm: 0x00C6F863 `pmulhw mm0,mm7` -- retail runs this hand-written MMX loop (movq/pmulhw/paddsw/punpcklwd/psrad/paddd, `align 16` loop head, `emms`) as inline asm in a C shell; the ramp/increment words are built in C.
// Shape (retry1): the parked "ecx input, caller-cleaned ret" convention is VC7.1's own private convention for a same-TU static: with
//   the same-TU caller below it passes `initial` in ecx and the caller pops the four stack words. `initial` is declared before
//   `step` (the canonical operand order of gains[1] = `lea edx,[ecx+eax]`). The MMX twin of the SSE2 ScaleAccumulate16Ramp (0x00C5D1F0).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6F800, 211 bytes, aligned /GS frame (`and esp,-16` through ebx), plain `ret`.
// Name: the parked descriptive CUnprovenMixKernel_<addr> (no libpk PDB).
// Identity (descriptive): one of five byte-identical copies (0x00C5BEF0, 0x00C6AAE0, 0x00C6F1A0, 0x00C6F800, 0x00C6FC80) of one libpk
//   kernel, each called only from small register-loading thunks (this copy's own); libpk has no PDB.
#ifndef _MSC_VER
#define __cdecl
#endif
static __declspec(noinline) void __cdecl CUnprovenMixKernel_00c6f800(const short* src,unsigned long count,long* dst,short initial,short step){
#ifdef _MSC_VER
__declspec(align(16)) short gains[4];__declspec(align(16)) short increments[4];
gains[0]=(short)(initial+0*step);
gains[1]=(short)(initial+1*step);
gains[2]=(short)(initial+2*step);
gains[3]=(short)(initial+3*step);
increments[0]=(short)(step*4);
increments[1]=(short)(step*4);
increments[2]=(short)(step*4);
increments[3]=(short)(step*4);
__asm {
 mov ecx,count
 mov eax,src
 mov edx,dst
 movq mm7,gains
 movq mm6,increments
 align 16
 next_block:
 movq mm0,[eax]
 pmulhw mm0,mm7
 paddsw mm7,mm6
 pxor mm1,mm1
 pxor mm2,mm2
 punpcklwd mm1,mm0
 punpckhwd mm2,mm0
 psrad mm1,15
 psrad mm2,15
 paddd mm1,[edx]
 paddd mm2,[edx+8]
 movq [edx],mm1
 movq [edx+8],mm2
 movq mm3,[eax+8]
 pmulhw mm3,mm7
 paddsw mm7,mm6
 pxor mm4,mm4
 pxor mm5,mm5
 punpcklwd mm4,mm3
 punpckhwd mm5,mm3
 psrad mm4,15
 psrad mm5,15
 paddd mm4,[edx+10h]
 paddd mm5,[edx+18h]
 movq [edx+10h],mm4
 add eax,10h
 movq [edx+18h],mm5
 add edx,20h
 dec ecx
 jne next_block
 emms
}
#else
int gains[4];for(int i=0;i<4;++i)gains[i]=(short)(initial+i*step);
for(unsigned long block=0;block<count*2;++block){for(int lane=0;lane<4;++lane){int product=(int)src[block*4+lane]*gains[lane];int high=product>>16;dst[block*4+lane]+=high*2;}for(int lane=0;lane<4;++lane){int next=gains[lane]+(short)(step*4);gains[lane]=next>32767?32767:(next<-32768?-32768:next);}}
#endif
}
// Same-TU caller (stand-in for the this copy's register-loading thunk). Not graded.
void __cdecl MixQuadsMMX_00c6f800(const short* src,unsigned long count,long* dst,short initial,short step){
CUnprovenMixKernel_00c6f800(src,count,dst,initial,step);
}