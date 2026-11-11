// CVtable_012b5de0::MixAdd16Ramp(const short* src, long* dst, short initial, short step) -- vtable slot 9 of the SSE2 limited mix buffer: add the sixteen-block 16-bit source into the long accumulator under a gain ramp (initial gain, per-sample step) through the SSE2 kernel
// Leaf: ?MixAdd16Ramp@CVtable_012b5de0@@UAEXPBFPAJFF@Z
// Flags: /GS
// Retail-asm: 0x00C5D294 `pmulhw xmm0,xmm7` -- the kernel this thunk calls (0x00C5D1F0, landed) runs retail's hand-written SSE2 loop as inline asm; it sits in this TU only
//   because VC7.1 gives a same-TU static its private convention (below). The graded thunk 0x00C5D530 itself holds no asm.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5D530, 32 bytes, thiscall `ret 10h` (`this` unused): step and dst pushed, `initial` loaded into
//   ecx (`mov ecx,[esp+14h]`), the block count 16 and src pushed, `call 0x00C5D1F0; add esp,10h`.
// Identity (descriptive; libpk has no PDB or RTTI): slot 9 (cell 0x012B5DE0 + 0x24) of the vtable 0x012B5DE0, the CLimitedMixBuffer-derived
//   table whose kernel slots point at the SSE2 copies (the class is named by its vtable as the landed 0x00C5D490 row does;
//   the CMixBuffer / CLimitedMixBuffer model is that row's). `MixAdd16Ramp` is a descriptive name (NOTES).
// Shape: the callee is the same-TU static kernel 0x00C5D1F0 (landed row; body = the shared libpk/MixKernelsSIMD.h helper), which VC7.1
//   calls with its own private convention for a non-escaping static: `initial` in ecx, four stack words, caller cleanup.
#include "libpk/MixKernelsSIMD.h"
#ifndef _MSC_VER
#define __cdecl
#define __declspec(x)
#endif
// The kernel 0x00C5D1F0 exactly as its landed row spells it (not graded here).
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
LIBPK_SSE2_SCALE_ACCUMULATE16_RAMP(count, src, dst, gains, increments);
#else
int gains[8];for(int i=0;i<8;++i)gains[i]=(short)(initial+i*step);
for(unsigned long block=0;block<count;++block){for(int lane=0;lane<8;++lane){int product=(int)src[block*8+lane]*gains[lane];int high=product/65536;if(product<0&&product%65536)--high;dst[block*8+lane]+=high*2;int next=gains[lane]+(short)(step*8);gains[lane]=next>32767?32767:(next < -32768?-32768:next);}}
#endif
}

class CMixBuffer {                                  // vtable 0x012B8F38 (the landed CMixBuffer model)
public:
    virtual ~CMixBuffer();                                                            // slot 0
    virtual void Reset();                                                             // slot 1
    virtual void Clear();                                                             // slot 2
    virtual void MixAdd16(const short* src, long* dst, unsigned short gain);         // slot 3
    virtual void MixAdd32Gain(const long* src, long* dst, unsigned short gain);      // slot 4
    virtual void MixAdd32(const long* src, long* dst);                                // slot 5
    virtual void MixAddFloatGain(const float* src, long* dst, float gain);           // slot 6
    virtual void MixAddFloatGain16(const float* src, long* dst, unsigned short gain); // slot 7
    virtual void MixAddFloat(const float* src, long* dst);                            // slot 8
    virtual void MixAdd16Ramp(const short* src, long* dst, short initial, short step); // slot 9
    long*         m_pChannel[6];        // +0x04 planar 128-sample accumulators
    long          m_active;             // +0x1c
    unsigned long m_numChannels;        // +0x20
    unsigned char m_storage[0xc10];     // +0x24
};
class CLimitedMixBuffer : public CMixBuffer {      // vtable 0x012B5BE0 (ctor 0x00C3EFA0)
public:
    unsigned char m_limiters[6 * 0x17c];            // +0xc34 six CLimiters
};
class CVtable_012b5de0 : public CLimitedMixBuffer { // vtable 0x012B5DE0
public:
    virtual void MixAdd16Ramp(const short* src, long* dst, short initial, short step);   // slot 9 (0x00C5D530)
};

void CVtable_012b5de0::MixAdd16Ramp(const short* src, long* dst, short initial, short step)
{
    ScaleAccumulate16Ramp(src, 16, dst, initial, step);
}