// CSalRenderPanDelaySSE2::Process(const short* pSrc, int*(&ppDst)[4]) -- vtable slot 2 of the GameCODA SSE2 HRTF-delay panner: copy the 128-sample block into the aligned history, then under the effect lock either cross-fade (ramp the old taps down, swap the tap sets, ramp the new ones up) or mix each tap at its constant gain, through the SSE2 kernels; finally slide the last 32 samples below the buffer for the next block with a hand SSE2 copy
// Leaf: ?Process@CSalRenderPanDelaySSE2@@UAEXPBFAAY03PAH@Z
// Flags: /GS
// Retail-asm: 0x00C6D923..0x00C6D94C `mov esi,[esp+14h] ; mov edi,[esp+18h] ; movdqa xmm0,[esi] ; movdqa xmm1,[esi+10h] ; movdqa xmm2,[esi+20h] ; movdqa xmm3,[esi+30h] ; movdqa [edi],xmm0 ; movdqa [edi+10h],xmm1 ; movdqa [edi+20h],xmm2 ; movdqa [edi+30h],xmm3` -- a hand SSE2 copy of the last 64 history bytes on fixed esi/edi loaded from two C locals that VC7.1 homed in the dead argument slots (`mov [esp+14h],edx ; mov [esp+18h],ebx`); VC7.1 RTM has no intrinsic that names esi/edi or reloads them from frame slots, and the C path of the same slot (0x00C6CC60) is `rep movsd`; same family as the landed ClearHistory 0x00C6D560 (slot 1 of this vtable) and the SIMD kernels 0x00C6D370 / 0x00C6D410. The rest of the body is C++.
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared libpk/MixKernelsSIMD.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family SIMD_LIBPK.
// Retail: Fable.exe (TLC, VC7.1, /GS TU) @ 0x00C6D630, 808 bytes, thiscall `ret 8`; slot 2 of vtable 0x012B8A64 (the only
//   reference: .rdata 0x012B8A6C), the SSE2 derivative of the GameCODA HRTF-delay panner (CSalRenderPanDelay) (ctor 0x00C55070); the base slot 2 is
//   the C Process 0x00C6CC60 (same body over MixDelayTapRampDown / MixDelayTapRampUp / MixDelayTap 0x00C6CB80 / 0x00C6CAF0 /
//   0x00C6CC10), the MMX slot 2 is 0x00C6F3E0.
// Name (NF447, two sources): the GameCODA 2.10 SDK CodeView declares the virtual CSalRenderPanDelaySSE2::Process(const short*, int*(&)[4]), slot 2
//   of CSalRender (engine/CSalRender.h; class CLASS_TABLE tie through CSalSw3DVoice::CreateRenderer 0x00C55270, NF441), and retail pops 8
//   argument bytes (`ret 8`) = that signature (NF445 sigcheck_19.tsv); the body is the base CSalRenderPanDelay::Process 0x00C6CC60 over the
//   SSE2 kernels (outputs 0..3 only). Was ?Process@CVtable_012b8a64@@UAEXPBFPBQAJ@Z (descriptive stand-in).
// Callees: EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10) through CSalMutex::Enter / Leave on m_mutexUpdate (+0x04; the critical section +0x08), memcpy
//   intrinsic (128 samples into the aligned history +0x178), the SSE2 kernels MixSamplesRampSSE2 0x00C6D410 (a static of this
//   TU: VC7.1 passes its first argument in ecx, so its landed body is repeated here) and ScaleAccumulate16 0x00C6D370
//   (cdecl), g_pAudioMaster 0x0143BC2C (four or more output channels mix taps 2 and 3).
// Shape: the three tap mixers retail keeps out of line with no callers (0x00C6D590 ramp up, 0x00C6D5C0 ramp down,
//   0x00C6D600 constant gain) are written as plain functions above Process and VC7.1 expands all twelve uses, as retail;
//   the tap-set swap loads the current set into a named local first (the other two spellings store +0x1dc first).
//   The TU is /GS (the kernels carry cookies), which also decides the static's register convention (without /GS VC7.1
//   passes `step` in eax as well).
#include "libpk/MixKernelsSIMD.h"
#include <string.h>
#pragma intrinsic(memcpy)
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#define FABLE_RETAIL_CSALMUTEX_ENTER_LEAVE
#include "engine/CSalRenderPanDelaySSE2.h"   // CSalRenderPanDelaySSE2 : CSalRenderPanDelay (m_pAudio +0x178, m_pParamsCurrent +0x1DC)
struct CAudioMaster { unsigned char _pad[0x1bc]; unsigned long m_outputChannels; };
extern CAudioMaster* g_pAudioMaster;                                        // 0x0143BC2C
struct SDelayTap {                        // 8 bytes: the landed tap mixers' view of FilterPanDelay (itd, gain, gainDelta)
    long           m_delay;               // +0x00 samples back into the history
    unsigned short m_gain;                // +0x04 Q15 gain
    short          m_step;                // +0x06 Q15 gain step (ramps)
};
void __cdecl ScaleAccumulate16(const short* src, unsigned long count, long* dst, short gain);   // 0x00C6D370

// The SSE2 ramp kernel 0x00C6D410 (landed): same retail TU, a static whose first argument VC7.1 passes in ecx.
static __declspec(noinline) void __cdecl MixSamplesRampSSE2(unsigned short startGain,const short* source,unsigned long groupCount,long* accumulator,short step){
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
LIBPK_SSE2_SCALE_ACCUMULATE16_RAMP_UNALIGNED(groupCount, source, accumulator, gains, increments);
}
// The three SSE2 tap mixers of this TU (retail 0x00C6D590 ramp up, 0x00C6D5C0 ramp down, 0x00C6D600 constant gain; no callers:
// VC7.1 expanded them into Process and kept the out-of-line copies).
void MixDelayTapRampUpSSE2(const short* source, const SDelayTap* tap, long* accumulator)
{
    if (tap->m_step != 0)
        MixSamplesRampSSE2(0, source - tap->m_delay, 16, accumulator, tap->m_step);
}
void MixDelayTapRampDownSSE2(const short* source, const SDelayTap* tap, long* accumulator)
{
    if (tap->m_gain != 0)
        MixSamplesRampSSE2(tap->m_gain, source - tap->m_delay, 16, accumulator, -tap->m_step);
}
void MixDelayTapSSE2(const short* source, unsigned long count, const SDelayTap* tap, long* accumulator)
{
    if (tap->m_gain != 0)
        ScaleAccumulate16(source - tap->m_delay, count, accumulator, tap->m_gain);
}

// The tap mixers take the SDelayTap view of a FilterPanDelay and a long accumulator; these views only rename the types.
static inline const SDelayTap* TapOf(const FilterPanDelay& filter) { return reinterpret_cast<const SDelayTap*>(&filter); }
static inline long* AccumulatorOf(int* pDst) { return reinterpret_cast<long*>(pDst); }

void CSalRenderPanDelaySSE2::Process(const short* pSrc, int*(&ppDst)[4])
{
    memcpy(m_pAudio, pSrc, 128 * sizeof(short));
    m_mutexUpdate.Enter();
    if (m_bUpdate) {
        m_bUpdate = false;
        MixDelayTapRampDownSSE2(m_pAudio, TapOf(m_pParamsCurrent->pan[0]), AccumulatorOf(ppDst[0]));
        MixDelayTapRampDownSSE2(m_pAudio, TapOf(m_pParamsCurrent->pan[1]), AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixDelayTapRampDownSSE2(m_pAudio, TapOf(m_pParamsCurrent->pan[2]), AccumulatorOf(ppDst[2]));
            MixDelayTapRampDownSSE2(m_pAudio, TapOf(m_pParamsCurrent->pan[3]), AccumulatorOf(ppDst[3]));
        }
        ParamsPanDelay* pParams = m_pParamsCurrent;
        m_pParamsCurrent = m_pParamsNew;
        m_pParamsNew = pParams;
        MixDelayTapRampUpSSE2(m_pAudio, TapOf(m_pParamsCurrent->pan[0]), AccumulatorOf(ppDst[0]));
        MixDelayTapRampUpSSE2(m_pAudio, TapOf(m_pParamsCurrent->pan[1]), AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixDelayTapRampUpSSE2(m_pAudio, TapOf(m_pParamsCurrent->pan[2]), AccumulatorOf(ppDst[2]));
            MixDelayTapRampUpSSE2(m_pAudio, TapOf(m_pParamsCurrent->pan[3]), AccumulatorOf(ppDst[3]));
        }
    } else {
        MixDelayTapSSE2(m_pAudio, 16, TapOf(m_pParamsCurrent->pan[0]), AccumulatorOf(ppDst[0]));
        MixDelayTapSSE2(m_pAudio, 16, TapOf(m_pParamsCurrent->pan[1]), AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixDelayTapSSE2(m_pAudio, 16, TapOf(m_pParamsCurrent->pan[2]), AccumulatorOf(ppDst[2]));
            MixDelayTapSSE2(m_pAudio, 16, TapOf(m_pParamsCurrent->pan[3]), AccumulatorOf(ppDst[3]));
        }
    }
    m_mutexUpdate.Leave();
    short* source = m_pAudio + 96;
    short* history = m_pAudio - 32;
    LIBPK_SSE2_COPY64(source, history);
}