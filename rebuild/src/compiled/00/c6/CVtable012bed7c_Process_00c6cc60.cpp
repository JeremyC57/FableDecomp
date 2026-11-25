// CSalRenderPanDelay::Process(const short* pSrc, int*(&ppDst)[4]) -- vtable slot 2 of the GameCODA HRTF-delay panner, the plain C path:
//   copy the 128-sample block into the aligned buffer, then under m_mutexUpdate either cross-fade (ramp the current filters down, swap
//   the parameter sets, ramp the new ones up) or mix each channel's delayed tap at its constant gain; finally slide the last 32 samples
//   below the buffer for the next block
// Leaf: ?Process@CSalRenderPanDelay@@UAEXPBFAAY03PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6CC60, 648 bytes, thiscall `ret 8`; slot 2 of vtable 0x012BED7C (the only reference:
//   .rdata 0x012BED84). Same slot in the SIMD classes: SSE2 0x012B8A64 -> 0x00C6D630, MMX 0x012B8AA0 -> 0x00C6F3E0.
// Name (NF447, two sources): the GameCODA 2.10 SDK CodeView declares the virtual CSalRenderPanDelay::Process(const short*, int*(&)[4]),
//   slot 2 of CSalRender (engine/CSalRender.h); retail pops 8 argument bytes (`ret 8`) = that signature (NF445 sigcheck_19.tsv), indexes
//   the output array 0..3 only (outputs 2 / 3 behind the 4-speaker test), and calls the per-channel mixers in the SDK's MixFadeOut /
//   swap / MixFadeIn / Mix order; the class is the one its constructor 0x00C6CA20 names. Was ?Process@CVtable_012bed7c@@UAEXPBFPBQAJ@Z.
// Callees: CSalMutex::Enter / Leave (EnterCriticalSection / LeaveCriticalSection, IAT 0x0143FE0C / 0x0143FE10) on m_mutexUpdate (+0x04;
//   the critical section +0x08); the landed tap mixers MixDelayTapRampDown 0x00C6CB80, MixDelayTapRampUp 0x00C6CAF0, MixDelayTap
//   0x00C6CC10 (same TU, cdecl; the SDK's private statics MixFadeOut / MixFadeIn / Mix, kept under their landed spellings and their
//   SDelayTap view of FilterPanDelay until they are ported); g_pAudioMaster 0x0143BC2C (four or more output channels mix taps 2 and 3).
// Shape: the three landed tap mixers are written above Process with their landed bodies; VC7.1 keeps the ramps out of line and
//   expands MixDelayTap at the first two sites only (its inline budget), exactly as retail; the block copies are the memcpy
//   intrinsic (`rep movsd`, 0x40 and 0x10 dwords).
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#define FABLE_RETAIL_CSALMUTEX_ENTER_LEAVE
#include "engine/CSalRenderPanDelay.h"   // CSalRenderPanDelay (vtable 0x012BED7C) : CSalRender (m_mutexUpdate +0x04)
#include <string.h>
#pragma intrinsic(memcpy)
struct CAudioMaster { unsigned char _pad[0x1bc]; unsigned long m_outputChannels; };
extern CAudioMaster* g_pAudioMaster;                                        // 0x0143BC2C
struct SDelayTap {                     // the landed tap mixers' view of FilterPanDelay (itd, gain, gainDelta)
    long           m_delay;
    unsigned short m_gain;
    short          m_step;
};

void MixDelayTapRampUp(const short* source, const SDelayTap* tap, long* accumulator)
{
    short step = tap->m_step;
    if (step == 0)
        return;
    const short* samples = source - tap->m_delay;
    long gain = 0;
    for (int i = 0; i < 128; ++i) {
        accumulator[i] += (*samples++ * gain) >> 15;
        gain += step;
    }
}
void MixDelayTapRampDown(const short* source, const SDelayTap* tap, long* accumulator)
{
    unsigned short start = tap->m_gain;
    if (start == 0)
        return;
    short step = -tap->m_step;
    const short* samples = source - tap->m_delay;
    long gain = start;
    for (int i = 0; i < 128; ++i) {
        accumulator[i] += (*samples++ * gain) >> 15;
        gain += step;
    }
}
void MixDelayTap(const short* source, unsigned long count, const SDelayTap* tap, long* accumulator)
{
    unsigned short gain = tap->m_gain;
    if (gain == 0)
        return;
    long* out = accumulator;
    const short* samples = source - tap->m_delay;
    for (unsigned long i = 0; i < count; ++i)
        *out++ += (*samples++ * gain) >> 15;
}

// The landed mixers take the SDelayTap view of a FilterPanDelay and a long accumulator; these views only rename the types.
static inline const SDelayTap* TapOf(const FilterPanDelay& filter) { return reinterpret_cast<const SDelayTap*>(&filter); }
static inline long* AccumulatorOf(int* pDst) { return reinterpret_cast<long*>(pDst); }

void CSalRenderPanDelay::Process(const short* pSrc, int*(&ppDst)[4])
{
    memcpy(m_pAudio, pSrc, 128 * sizeof(short));
    m_mutexUpdate.Enter();
    if (m_bUpdate) {
        m_bUpdate = false;
        MixDelayTapRampDown(m_pAudio, TapOf(m_pParamsCurrent->pan[0]), AccumulatorOf(ppDst[0]));
        MixDelayTapRampDown(m_pAudio, TapOf(m_pParamsCurrent->pan[1]), AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixDelayTapRampDown(m_pAudio, TapOf(m_pParamsCurrent->pan[2]), AccumulatorOf(ppDst[2]));
            MixDelayTapRampDown(m_pAudio, TapOf(m_pParamsCurrent->pan[3]), AccumulatorOf(ppDst[3]));
        }
        ParamsPanDelay* pParams = m_pParamsCurrent;
        m_pParamsCurrent = m_pParamsNew;
        m_pParamsNew = pParams;
        MixDelayTapRampUp(m_pAudio, TapOf(m_pParamsCurrent->pan[0]), AccumulatorOf(ppDst[0]));
        MixDelayTapRampUp(m_pAudio, TapOf(m_pParamsCurrent->pan[1]), AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixDelayTapRampUp(m_pAudio, TapOf(m_pParamsCurrent->pan[2]), AccumulatorOf(ppDst[2]));
            MixDelayTapRampUp(m_pAudio, TapOf(m_pParamsCurrent->pan[3]), AccumulatorOf(ppDst[3]));
        }
    } else {
        MixDelayTap(m_pAudio, 128, TapOf(m_pParamsCurrent->pan[0]), AccumulatorOf(ppDst[0]));
        MixDelayTap(m_pAudio, 128, TapOf(m_pParamsCurrent->pan[1]), AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixDelayTap(m_pAudio, 128, TapOf(m_pParamsCurrent->pan[2]), AccumulatorOf(ppDst[2]));
            MixDelayTap(m_pAudio, 128, TapOf(m_pParamsCurrent->pan[3]), AccumulatorOf(ppDst[3]));
        }
    }
    m_mutexUpdate.Leave();
    memcpy(m_pAudio - 32, m_pAudio + 96, 32 * sizeof(short));
}
