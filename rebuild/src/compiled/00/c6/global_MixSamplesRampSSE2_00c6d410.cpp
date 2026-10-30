// MixSamplesRampSSE2(unsigned short startGain, const short* source, unsigned long groupCount, long* accumulator, short step) -- SSE2 ramped mix: the gain vector starts at startGain + k*step (k = 0..7) and rises by 8*step per group (paddsw, saturating); each group of 8 samples adds ((source[k] * gain[k]) >> 16) << 1 into the 32-bit accumulator
// Leaf: MixSamplesRampSSE2
// Flags: /GS
// Retail-asm: 0x00C6D4B4 `pmulhw xmm0,xmm7` -- retail runs this hand-written SSE2 loop (movdqu source / pmulhw / paddsw / punpcklwd / psrad /
//   paddd, `align 16` loop head) as inline asm in a C shell; the gain/increment vectors are built word by word in C.
// Shape (retry1): the parked "LTCG register argument" (startGain in ecx, the rest on the stack, caller pops) is VC7.1's own private
//   convention for a same-TU static: with the same-TU caller below it passes startGain in ecx itself. The body is the SSE2 ramp of
//   0x00C5D1F0 with an unaligned source load (movdqu).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6D410, 240 bytes, aligned /GS frame (`push ebx ; mov ebx,esp ; and esp,-16`), cookie 0x0139C8A8.
//   Callers: 0x00C6D590 (ramp up), 0x00C6D5C0 (ramp down), 0x00C6D68F..0x00C6D836 (the other ramp mixers). Module: libpk's software
//   mixer (descriptive name, no PDB).
#ifndef _MSC_VER
#define __cdecl
#endif
static __declspec(noinline) void __cdecl MixSamplesRampSSE2(unsigned short startGain,const short* source,unsigned long groupCount,long* accumulator,short step){
#ifdef _MSC_VER
__declspec(align(16)) short gains[8];__declspec(align(16)) short increments[8];
gains[0]=(short)(startGain+0*step);
gains[1]=(short)(startGain+1*step);
gains[2]=(short)(startGain+2*step);
gains[3]=(short)(startGain+3*step);
gains[4]=(short)(startGain+4*step);
gains[5]=(short)(startGain+5*step);
gains[6]=(short)(startGain+6*step);
gains[7]=(short)(startGain+7*step);
increments[0]=(short)(step*8);
increments[1]=(short)(step*8);
increments[2]=(short)(step*8);
increments[3]=(short)(step*8);
increments[4]=(short)(step*8);
increments[5]=(short)(step*8);
increments[6]=(short)(step*8);
increments[7]=(short)(step*8);
__asm {
 mov ecx,groupCount
 mov eax,source
 mov edx,accumulator
 movdqa xmm7,gains
 movdqa xmm6,increments
 align 16
 next_block:
 movdqu xmm0,[eax]
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
int gains[8];for(int i=0;i<8;++i)gains[i]=(short)(startGain+i*step);
for(unsigned long block=0;block<groupCount;++block){for(int lane=0;lane<8;++lane){int product=(int)source[block*8+lane]*gains[lane];int high=product/65536;if(product<0&&product%65536)--high;accumulator[block*8+lane]+=high*2;int next=gains[lane]+(short)(step*8);gains[lane]=next>32767?32767:(next < -32768?-32768:next);}}
#endif
}
// Same-TU caller (stand-in for the ramp mixers 0x00C6D590 / 0x00C6D5C0). Not graded.
void __stdcall MixBlock16(const short* src,long* dst,short initial,short step){
MixSamplesRampSSE2(initial,src,16,dst,step);
}