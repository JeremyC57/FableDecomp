// CSalRenderPanSSE2::Process(const short* pSrc, int*(&ppDst)[4]) -- vtable slot 2 of the GameCODA SSE2 volume panner: under the update lock
//   either cross-fade (ramp each output's current FilterPan down, swap the parameter sets, ramp the new ones up) or mix the input into each
//   output at its Q15 gain, through the SSE2 kernels (outputs 2 / 3 only with four or more output channels)
// Leaf: ?Process@CSalRenderPanSSE2@@UAEXPBFAAY03PAH@Z
// Flags: /GS
// Retail-asm: 0x00C6E334 `pmulhw xmm0,xmm7` (0x00C6E317..0x00C6E36F `movdqa xmm7,[gains] ; movdqa xmm6,[increments] ; movdqa xmm0,[eax] ; pmulhw xmm0,xmm7 ; paddsw xmm7,xmm6 ; ... ; dec ecx ; jne`) -- the hand SSE2 ramp kernel Row00c6e290::ScaleAccumulate16Ramp (landed retail_asm) is a static of this retail TU: VC7.1 passes its `initial` argument in ecx only to a same-TU caller (`mov cx,[eax]` / `xor ecx,ecx` before each call here), so its landed body is repeated verbatim; Process itself is plain C++ (precedent: the landed SSE2 Process 0x00C6D630 and MMX Process 0x00C6FE10).
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared libpk/MixKernelsSIMD.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family SIMD_LIBPK.
// Retail: Fable.exe (TLC, VC7.1, /GS TU) @ 0x00C6E410, 533 bytes, thiscall `ret 8`; slot 2 of vtable 0x012B8A78 (constructor 0x00C55090), the SSE2 class of the
//   volume panner CSalRenderPan (base slot 2 = 0x00C6DBA0, no row).
// Name (NF452, two sources): (1) the GameCODA 2.10 SDK CodeView (CODAApiDbg.lib Debug/SalRenderPanSSE2.obj) declares the virtual
//   CSalRenderPanSSE2::Process(const short*, int*(&)[4]), slot 2 of CSalRender, and the class tie runs through CSalSw3DVoice::CreateRenderer
//   0x00C55270 (NF441 CLASS_TABLE / NAME_FIXES: SDK ??0CSalRenderPanSSE2 = retail 0x00C55090, whose vtable this slot is); (2) the Release body:
//   `ret 8` (8 argument bytes = the SDK signature; NF445 sigcheck_19.tsv) and the same Enter / fade-out x2(+2) / swap / fade-in x2(+2) /
//   Leave, constant x2(+2) / Leave structure behind the 4-output test (8 ramp-kernel and 4 constant-kernel calls); retail 198 v SDK 183
//   instructions (retail expands the tap mixers over its own kernels). Was ?Process@CVtable_012b8a78@@UAEXPBFPBQAJ@Z (descriptive stand-in).
// Callees: EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10) through CSalMutex::Enter / Leave on m_mutexUpdate (+0x04;
//   the critical section +0x08), the SSE2 kernels Row00c6e290::ScaleAccumulate16Ramp 0x00C6E290 (a static of this TU, initial in ecx) and Row00c6e1f0::ScaleAccumulate16 0x00C6E1F0 (cdecl), g_pAudioMaster 0x0143BC2C (four or more output channels mix outputs 2 and 3; the landed local view of
//   the SDK CSalPlatform::m_nSWOutputChannels at +0x1BC, kept as HEAD text: follow-up port onto engine/CSalPlatform.h).
// CVtable_012b8a78::Process(const short* input, long* const* outputs) -- vtable slot 2 of libpk's SSE2 four-record gain mixer: under the effect lock either cross-fade (ramp each current gain record down on its output, swap the current / previous record arrays, ramp the new records up) or mix the input at each record's constant Q15 gain into outputs 0-1 (and 2-3 with four or more output channels), through the SSE2 kernels
// Leaf: ?Process@CVtable_012b8a78@@UAEXPBFPBQAJ@Z
// Flags: /GS
// Retail-asm: 0x00C6E334 `pmulhw xmm0,xmm7` (0x00C6E317..0x00C6E36F `movdqa xmm7,[gains] ; movdqa xmm6,[increments] ; movdqa xmm0,[eax] ; pmulhw xmm0,xmm7 ; paddsw xmm7,xmm6 ; ... ; dec ecx ; jne`) -- the hand SSE2 ramp kernel Row00c6e290::ScaleAccumulate16Ramp (landed retail_asm) is a static of this retail TU: VC7.1 passes its `initial` argument in ecx only to a same-TU caller (`mov cx,[eax]` / `xor ecx,ecx` before each call here), so its landed body is repeated verbatim; Process itself is plain C++ (precedent: the landed SSE2 Process 0x00C6D630 and MMX Process 0x00C6FE10).
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared libpk/MixKernelsSIMD.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family SIMD_LIBPK.
#include "libpk/MixKernelsSIMD.h"
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#define FABLE_RETAIL_CSALMUTEX_ENTER_LEAVE
#include "engine/CSalRenderPanSSE2.h"   // CSalRenderPanSSE2 : CSalRenderDisable (m_pParamsCurrent +0x30, m_pParamsNew +0x34)
struct CAudioMaster { unsigned char _pad[0x1bc]; unsigned long m_outputChannels; };
extern CAudioMaster* g_pAudioMaster;                                        // 0x0143BC2C
namespace Row00c6e1f0 {
void __cdecl ScaleAccumulate16(const short* src, unsigned long count, long* dst, short gain);   // 0x00C6E1F0
}
namespace Row00c6e290 {
static __declspec(noinline) void __cdecl ScaleAccumulate16Ramp(const short* src,unsigned long count,long* dst,short initial,short step){
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
}
}
void MixGainRampUpSSE2(const short* source, const FilterPan* ramp, long* accumulator)          // 0x00C6E380 (no callers)
{
    if (ramp->gainDelta != 0)
        Row00c6e290::ScaleAccumulate16Ramp(source, 16, accumulator, 0, ramp->gainDelta);
}
void MixGainRampDownSSE2(const short* source, const FilterPan* ramp, long* accumulator)        // 0x00C6E3B0 (no callers)
{
    if (ramp->gain != 0)
        Row00c6e290::ScaleAccumulate16Ramp(source, 16, accumulator, ramp->gain, -ramp->gainDelta);
}
void MixGainSSE2(const short* source, unsigned long count, const FilterPan* ramp, long* accumulator)   // 0x00C6E3E0 (no callers)
{
    if (ramp->gain != 0)
        Row00c6e1f0::ScaleAccumulate16(source, count, accumulator, ramp->gain);
}
// The mixers take a long accumulator (the landed kernels' signature); this view only renames the type.
static inline long* AccumulatorOf(int* pDst) { return reinterpret_cast<long*>(pDst); }

void CSalRenderPanSSE2::Process(const short* pSrc, int*(&ppDst)[4])
{
    m_mutexUpdate.Enter();
    if (m_bUpdate) {
        m_bUpdate = false;
        MixGainRampDownSSE2(pSrc, &m_pParamsCurrent->pan[0], AccumulatorOf(ppDst[0]));
        MixGainRampDownSSE2(pSrc, &m_pParamsCurrent->pan[1], AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainRampDownSSE2(pSrc, &m_pParamsCurrent->pan[2], AccumulatorOf(ppDst[2]));
            MixGainRampDownSSE2(pSrc, &m_pParamsCurrent->pan[3], AccumulatorOf(ppDst[3]));
        }
        ParamsPan* pParams = m_pParamsCurrent;
        m_pParamsCurrent = m_pParamsNew;
        m_pParamsNew = pParams;
        MixGainRampUpSSE2(pSrc, &m_pParamsCurrent->pan[0], AccumulatorOf(ppDst[0]));
        MixGainRampUpSSE2(pSrc, &m_pParamsCurrent->pan[1], AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainRampUpSSE2(pSrc, &m_pParamsCurrent->pan[2], AccumulatorOf(ppDst[2]));
            MixGainRampUpSSE2(pSrc, &m_pParamsCurrent->pan[3], AccumulatorOf(ppDst[3]));
        }
    } else {
        MixGainSSE2(pSrc, 16, &m_pParamsCurrent->pan[0], AccumulatorOf(ppDst[0]));
        MixGainSSE2(pSrc, 16, &m_pParamsCurrent->pan[1], AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainSSE2(pSrc, 16, &m_pParamsCurrent->pan[2], AccumulatorOf(ppDst[2]));
            MixGainSSE2(pSrc, 16, &m_pParamsCurrent->pan[3], AccumulatorOf(ppDst[3]));
        }
    }
    m_mutexUpdate.Leave();
}