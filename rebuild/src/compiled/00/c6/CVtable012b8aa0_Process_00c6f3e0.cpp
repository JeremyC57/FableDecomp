// CSalRenderPanDelayMMX::Process(const short* pSrc, int*(&ppDst)[4]) -- vtable slot 2 of the GameCODA MMX HRTF-delay panner: copy the 128-sample block into the aligned history, then under the effect lock either cross-fade (ramp the old taps down, swap the tap sets, ramp the new ones up) or mix each tap at its constant gain, then keep the last 64 history bytes in front of the buffer
// Leaf: ?Process@CSalRenderPanDelayMMX@@UAEXPBFAAY03PAH@Z
// Flags: /GS
// Retail-asm: 0x00C6F6A3 `movq mm0,[esi]` (0x00C6F69B..0x00C6F6DC `mov esi,[esp+14h] ; mov edi,[esp+18h] ; movq mm0,[esi] ; movq [edi],mm0 ; ... ; movq [edi+38h],mm7 ; emms`) -- the hand MMX copy of the last 64 history bytes
//   (LIBPK_MMX_COPY64 of libpk/MixKernelsSIMD.h) and, in the static kernel CUnprovenMixKernel_00c6f1a0 0x00C6F1A0 (landed, same text), LIBPK_MMX_SCALE_ACCUMULATE16_RAMP; the row has no asm of its own.
// Identity: slot 2 of vtable 0x012B8AA0 (the only reference: .rdata 0x012B8AA8; reloc_bindings ??_7CSalRenderPanDelayMMX@@6B@), the MMX derivative of the GameCODA HRTF-delay panner (CSalRenderPanDelay); the landed
//   SSE2 sibling 0x00C6D630 (CSalRenderPanDelaySSE2::Process) names this address as "the MMX slot 2"; same body over the MMX kernels ScaleAccumulate16MMX 0x00C6F0F0 / CUnprovenMixKernel_00c6f1a0.
// Name (NF447, two sources): the GameCODA 2.10 SDK CodeView declares the virtual CSalRenderPanDelayMMX::Process(const short*, int*(&)[4]), slot 2
//   of CSalRender (engine/CSalRender.h; class CLASS_TABLE tie through CSalSw3DVoice::CreateRenderer 0x00C55270, NF441), and retail pops 8
//   argument bytes (`ret 8`) = that signature (NF445 sigcheck_19.tsv); the body is the base CSalRenderPanDelay::Process 0x00C6CC60 over the
//   MMX kernels (outputs 0..3 only). Was ?Process@CVtable_012b8aa0@@UAEXPBFPBQAJ@Z (descriptive stand-in).
// Models: engine/CSalRenderPanDelayMMX.h (NF447).
// Retail: Fable.exe (TLC, VC7.1, /GS TU) @ 0x00C6F3E0, 834 bytes, thiscall `ret 8`.
#include "libpk/MixKernelsSIMD.h"
#include <string.h>
#pragma intrinsic(memcpy)
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#define FABLE_RETAIL_CSALMUTEX_ENTER_LEAVE
#include "engine/CSalRenderPanDelayMMX.h"   // CSalRenderPanDelayMMX : CSalRenderPanDelay (m_pAudio +0x178, m_pParamsCurrent +0x1DC)
struct CAudioMaster { unsigned char _pad[0x1bc]; unsigned long m_outputChannels; };
extern CAudioMaster* g_pAudioMaster;                                        // 0x0143BC2C
struct SDelayTap {                        // 8 bytes: the landed tap mixers' view of FilterPanDelay (itd, gain, gainDelta)
    long           m_delay;               // +0x00 samples back into the history
    unsigned short m_gain;                // +0x04 Q15 gain
    short          m_step;                // +0x06 Q15 gain step (ramps)
};
void __cdecl ScaleAccumulate16MMX(const short* src, unsigned long count, long* dst, short gain);   // 0x00C6F0F0
static __declspec(noinline) void __cdecl CUnprovenMixKernel_00c6f1a0(const short* src,unsigned long count,long* dst,short initial,short step){
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
}
void MixDelayTapRampUpMMX(const short* source, const SDelayTap* tap, long* accumulator)
{
    if (tap->m_step != 0)
        CUnprovenMixKernel_00c6f1a0(source - tap->m_delay, 16, accumulator, 0, tap->m_step);
}
void MixDelayTapRampDownMMX(const short* source, const SDelayTap* tap, long* accumulator)
{
    if (tap->m_gain != 0)
        CUnprovenMixKernel_00c6f1a0(source - tap->m_delay, 16, accumulator, tap->m_gain, -tap->m_step);
}
void MixDelayTapMMX(const short* source, unsigned long count, const SDelayTap* tap, long* accumulator)
{
    if (tap->m_gain != 0)
        ScaleAccumulate16MMX(source - tap->m_delay, count, accumulator, tap->m_gain);
}

// The tap mixers take the SDelayTap view of a FilterPanDelay and a long accumulator; these views only rename the types.
static inline const SDelayTap* TapOf(const FilterPanDelay& filter) { return reinterpret_cast<const SDelayTap*>(&filter); }
static inline long* AccumulatorOf(int* pDst) { return reinterpret_cast<long*>(pDst); }

void CSalRenderPanDelayMMX::Process(const short* pSrc, int*(&ppDst)[4])
{
    memcpy(m_pAudio, pSrc, 128 * sizeof(short));
    m_mutexUpdate.Enter();
    if (m_bUpdate) {
        m_bUpdate = false;
        MixDelayTapRampDownMMX(m_pAudio, TapOf(m_pParamsCurrent->pan[0]), AccumulatorOf(ppDst[0]));
        MixDelayTapRampDownMMX(m_pAudio, TapOf(m_pParamsCurrent->pan[1]), AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixDelayTapRampDownMMX(m_pAudio, TapOf(m_pParamsCurrent->pan[2]), AccumulatorOf(ppDst[2]));
            MixDelayTapRampDownMMX(m_pAudio, TapOf(m_pParamsCurrent->pan[3]), AccumulatorOf(ppDst[3]));
        }
        ParamsPanDelay* pParams = m_pParamsCurrent;
        m_pParamsCurrent = m_pParamsNew;
        m_pParamsNew = pParams;
        MixDelayTapRampUpMMX(m_pAudio, TapOf(m_pParamsCurrent->pan[0]), AccumulatorOf(ppDst[0]));
        MixDelayTapRampUpMMX(m_pAudio, TapOf(m_pParamsCurrent->pan[1]), AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixDelayTapRampUpMMX(m_pAudio, TapOf(m_pParamsCurrent->pan[2]), AccumulatorOf(ppDst[2]));
            MixDelayTapRampUpMMX(m_pAudio, TapOf(m_pParamsCurrent->pan[3]), AccumulatorOf(ppDst[3]));
        }
    } else {
        MixDelayTapMMX(m_pAudio, 16, TapOf(m_pParamsCurrent->pan[0]), AccumulatorOf(ppDst[0]));
        MixDelayTapMMX(m_pAudio, 16, TapOf(m_pParamsCurrent->pan[1]), AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixDelayTapMMX(m_pAudio, 16, TapOf(m_pParamsCurrent->pan[2]), AccumulatorOf(ppDst[2]));
            MixDelayTapMMX(m_pAudio, 16, TapOf(m_pParamsCurrent->pan[3]), AccumulatorOf(ppDst[3]));
        }
    }
    m_mutexUpdate.Leave();
    short* source = m_pAudio + 96;
    short* history = m_pAudio - 32;
    LIBPK_MMX_COPY64(source, history);
}