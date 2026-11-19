// CSoundMixer::MixBlockRamped(const short* source, short initial, short step) -- marks the mixer as having mixed (+8) and runs this copy of the libpk MMX ramped scale-accumulate kernel over one 16-quad block from the source into the mixer's accumulator (+0x21c), the per-lane volume starting at `initial` and moving by `step`
// Leaf: ?MixBlockRamped@CSoundMixer@@QAEXPBFFF@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6AC90, 38 bytes, thiscall `ret 0Ch`.
// Retail-asm: 0x00C6AB43 `pmulhw mm0,mm7` -- in the same-TU kernel copy 0x00C6AAE0 this row calls (shared libpk/MixKernelsSIMD.h
//   LIBPK_MMX_SCALE_ACCUMULATE16_RAMP, as the landed kernel row); the row's own 38 bytes are compiled C++.
// Name: descriptive (libpk has no PDB; the class is the landed CSoundMixer of MixBlocks 0x00C6ACC0, same +0x08 flag / +0x21c
//   accumulator). Callers 0x00C54EF2 / 0x00C5C843 pass `this` = [[0x0143BC2C]+0x17C].
// Shape: the "LTCG convention" of the parked draft is VC7.1's own private convention for a same-TU static (the landed kernel row
//   0x00C6AAE0 established it): retail's kernel copy at 0x00C6AAE0 is this TU's static, so the call passes `initial` in ecx
//   (0x00C6ACA4 `mov ecx,[esp+10h]`) and pops the four stack words itself (0x00C6ACB0 `add esp,10h`). The kernel body below is the
//   landed row's text (shared libpk/MixKernelsSIMD.h ramp block; the kernel carries the retail asm, this row has none of its own).
//   Its /GS cookie needs the TU flag; this row's own bytes have no frame.
#include "libpk/MixKernelsSIMD.h"
#ifndef _MSC_VER
#define __cdecl
#endif
static __declspec(noinline) void __cdecl CUnprovenMixKernel_00c6aae0(const short* src,unsigned long count,long* dst,short initial,short step){
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
LIBPK_MMX_SCALE_ACCUMULATE16_RAMP(count, src, dst, gains, increments);
#else
int gains[4];for(int i=0;i<4;++i)gains[i]=(short)(initial+i*step);
for(unsigned long block=0;block<count*2;++block){for(int lane=0;lane<4;++lane){int product=(int)src[block*4+lane]*gains[lane];int high=product>>16;dst[block*4+lane]+=high*2;}for(int lane=0;lane<4;++lane){int next=gains[lane]+(short)(step*4);gains[lane]=next>32767?32767:(next<-32768?-32768:next);}}
#endif
}

class CSoundMixer {
public:
    void MixBlockRamped(const short* source, short initial, short step);
private:
    unsigned char _pad_0x00[0x08];
    unsigned char m_bMixed;                    // +0x08
    unsigned char _pad_0x09[0x213];
    long*         m_pAccumulator;              // +0x21c
};

void CSoundMixer::MixBlockRamped(const short* source, short initial, short step)
{
    m_bMixed = 1;
    CUnprovenMixKernel_00c6aae0(source, 0x10, m_pAccumulator, initial, step);
}