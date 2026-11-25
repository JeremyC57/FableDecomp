// CSalRenderPanMMX::Process(const short* pSrc, int*(&ppDst)[4]) -- vtable slot 2 of the GameCODA MMX volume panner: under the update lock
//   either cross-fade (ramp each output's current FilterPan down, swap the parameter sets, ramp the new ones up) or mix the input into each
//   output at its Q15 gain, through the MMX kernels (outputs 2 / 3 only with four or more output channels)
// Leaf: ?Process@CSalRenderPanMMX@@UAEXPBFAAY03PAH@Z
// Flags: /GS
// Retail-asm: 0x00C6F863 `pmulhw mm0,mm7` (0x00C6F852..0x00C6F8C2 `movq mm7,[gains] ; movq mm6,[increments] ; movq mm0,[eax] ; pmulhw mm0,mm7 ; paddsw mm7,mm6 ; ... ; dec ecx ; jne ; emms`) -- the hand MMX ramp kernel CUnprovenMixKernel_00c6f800 (landed retail_asm) is a static of this retail TU: VC7.1 passes its `initial` argument in ecx only to a same-TU caller (`mov cx,[eax]` / `xor ecx,ecx` before each call here), so its landed body is repeated verbatim; Process itself is plain C++ (precedent: the landed MMX Process 0x00C6FE10 and SSE2 Process 0x00C6D630).
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared libpk/MixKernelsSIMD.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family SIMD_LIBPK.
// Retail: Fable.exe (TLC, VC7.1, /GS TU) @ 0x00C6F990, 533 bytes, thiscall `ret 8`; slot 2 of vtable 0x012B8AB4 (constructor 0x00C550F0), the MMX class of the
//   volume panner CSalRenderPan (base slot 2 = 0x00C6DBA0, no row).
// Name (NF452, two sources): (1) the GameCODA 2.10 SDK CodeView (CODAApiDbg.lib Debug/SalRenderPanMMX.obj) declares the virtual
//   CSalRenderPanMMX::Process(const short*, int*(&)[4]), slot 2 of CSalRender, and the class tie runs through CSalSw3DVoice::CreateRenderer
//   0x00C55270 (NF441 CLASS_TABLE / NAME_FIXES: SDK ??0CSalRenderPanMMX = retail 0x00C550F0, whose vtable this slot is); (2) the Release body:
//   `ret 8` (8 argument bytes = the SDK signature; NF445 sigcheck_19.tsv) and the same Enter / fade-out x2(+2) / swap / fade-in x2(+2) /
//   Leave, constant x2(+2) / Leave structure behind the 4-output test (8 ramp-kernel and 4 constant-kernel calls); retail 198 v SDK 183
//   instructions (retail expands the tap mixers over its own kernels). Was ?Process@CVtable_012b8ab4@@UAEXPBFPBQAJ@Z (descriptive stand-in).
// Callees: EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10) through CSalMutex::Enter / Leave on m_mutexUpdate (+0x04;
//   the critical section +0x08), the MMX kernels 0x00C6F800 (a static of this TU, initial in ecx) and 0x00C6F730 (cdecl), with their HEAD spellings, g_pAudioMaster 0x0143BC2C (four or more output channels mix outputs 2 and 3; the landed local view of
//   the SDK CSalPlatform::m_nSWOutputChannels at +0x1BC, kept as HEAD text: follow-up port onto engine/CSalPlatform.h).
// CVtable_012b8ab4::Process(const short* input, long* const* outputs) -- vtable slot 2 of libpk's MMX four-record gain mixer: under the effect lock either cross-fade (ramp each current gain record down on its output, swap the current / previous record arrays, ramp the new records up) or mix the input at each record's constant Q15 gain into outputs 0-1 (and 2-3 with four or more output channels), through the MMX kernels
// Leaf: ?Process@CVtable_012b8ab4@@UAEXPBFPBQAJ@Z
// Flags: /GS
// Retail-asm: 0x00C6F863 `pmulhw mm0,mm7` (0x00C6F852..0x00C6F8C2 `movq mm7,[gains] ; movq mm6,[increments] ; movq mm0,[eax] ; pmulhw mm0,mm7 ; paddsw mm7,mm6 ; ... ; dec ecx ; jne ; emms`) -- the hand MMX ramp kernel CUnprovenMixKernel_00c6f800 (landed retail_asm) is a static of this retail TU: VC7.1 passes its `initial` argument in ecx only to a same-TU caller (`mov cx,[eax]` / `xor ecx,ecx` before each call here), so its landed body is repeated verbatim; Process itself is plain C++ (precedent: the landed MMX Process 0x00C6FE10 and SSE2 Process 0x00C6D630).
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared libpk/MixKernelsSIMD.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family SIMD_LIBPK.
#include "libpk/MixKernelsSIMD.h"
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#define FABLE_RETAIL_CSALMUTEX_ENTER_LEAVE
#include "engine/CSalRenderPanMMX.h"   // CSalRenderPanMMX : CSalRenderDisable (m_pParamsCurrent +0x30, m_pParamsNew +0x34)
struct CAudioMaster { unsigned char _pad[0x1bc]; unsigned long m_outputChannels; };
extern CAudioMaster* g_pAudioMaster;                                        // 0x0143BC2C
void __cdecl ScaleAccumulate16MMX_00c6f730(const short* src, unsigned long count, long* dst, short gain);   // 0x00C6F730
static __declspec(noinline) void __cdecl CUnprovenMixKernel_00c6f800(const short* src,unsigned long count,long* dst,short initial,short step){
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
// The tap mixers live in this row's namespace (as the SSE2 sibling 0x00C6EEF0 does): the Disable MMX Process 0x00C6FE10 defines
// helpers of the same names and types, and two strong global definitions would collide at link time.
namespace Row00c6f990 {
void MixGainRampUpMMX(const short* source, const FilterPan* ramp, long* accumulator)          // 0x00C6F900 (no callers)
{
    if (ramp->gainDelta != 0)
        CUnprovenMixKernel_00c6f800(source, 16, accumulator, 0, ramp->gainDelta);
}
void MixGainRampDownMMX(const short* source, const FilterPan* ramp, long* accumulator)        // 0x00C6F930 (no callers)
{
    if (ramp->gain != 0)
        CUnprovenMixKernel_00c6f800(source, 16, accumulator, ramp->gain, -ramp->gainDelta);
}
void MixGainMMX(const short* source, unsigned long count, const FilterPan* ramp, long* accumulator)   // 0x00C6F960 (no callers)
{
    if (ramp->gain != 0)
        ScaleAccumulate16MMX_00c6f730(source, count, accumulator, ramp->gain);
}
}
using namespace Row00c6f990;
// The mixers take a long accumulator (the landed kernels' signature); this view only renames the type.
static inline long* AccumulatorOf(int* pDst) { return reinterpret_cast<long*>(pDst); }

void CSalRenderPanMMX::Process(const short* pSrc, int*(&ppDst)[4])
{
    m_mutexUpdate.Enter();
    if (m_bUpdate) {
        m_bUpdate = false;
        MixGainRampDownMMX(pSrc, &m_pParamsCurrent->pan[0], AccumulatorOf(ppDst[0]));
        MixGainRampDownMMX(pSrc, &m_pParamsCurrent->pan[1], AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainRampDownMMX(pSrc, &m_pParamsCurrent->pan[2], AccumulatorOf(ppDst[2]));
            MixGainRampDownMMX(pSrc, &m_pParamsCurrent->pan[3], AccumulatorOf(ppDst[3]));
        }
        ParamsPan* pParams = m_pParamsCurrent;
        m_pParamsCurrent = m_pParamsNew;
        m_pParamsNew = pParams;
        MixGainRampUpMMX(pSrc, &m_pParamsCurrent->pan[0], AccumulatorOf(ppDst[0]));
        MixGainRampUpMMX(pSrc, &m_pParamsCurrent->pan[1], AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainRampUpMMX(pSrc, &m_pParamsCurrent->pan[2], AccumulatorOf(ppDst[2]));
            MixGainRampUpMMX(pSrc, &m_pParamsCurrent->pan[3], AccumulatorOf(ppDst[3]));
        }
    } else {
        MixGainMMX(pSrc, 16, &m_pParamsCurrent->pan[0], AccumulatorOf(ppDst[0]));
        MixGainMMX(pSrc, 16, &m_pParamsCurrent->pan[1], AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainMMX(pSrc, 16, &m_pParamsCurrent->pan[2], AccumulatorOf(ppDst[2]));
            MixGainMMX(pSrc, 16, &m_pParamsCurrent->pan[3], AccumulatorOf(ppDst[3]));
        }
    }
    m_mutexUpdate.Leave();
}