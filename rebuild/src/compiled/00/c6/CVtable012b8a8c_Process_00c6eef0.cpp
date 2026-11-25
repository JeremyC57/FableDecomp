// CSalRenderDisableSSE2::Process(const short* pSrc, int*(&ppDst)[4]) -- vtable slot 2 of the GameCODA SSE2 plain-gain renderer: under the update
//   lock either cross-fade (ramp the current FilterPan down on every output, swap the parameter sets, ramp the new one up) or mix the input at
//   the current Q15 gain, through the SSE2 kernels (outputs 2 / 3 only with four or more output channels)
// Leaf: ?Process@CSalRenderDisableSSE2@@UAEXPBFAAY03PAH@Z
// Flags: /GS
// Retail-asm: 0x00C6EE14 `pmulhw xmm0,xmm7` -- the hand SSE2 ramp kernel Row00c6ed70::ScaleAccumulate16Ramp 0x00C6ED70 (landed retail_asm) is a static of this retail TU: VC7.1 passes its `initial` argument in ecx only to a same-TU caller (`mov cx,[eax]` / `xor ecx,ecx` before each call here), so its landed body is repeated verbatim; Process itself is plain C++ (precedent: the landed byte twin 0x00C6FE10 and SSE2 Process 0x00C6E410 / 0x00C6D630).
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared libpk/MixKernelsSIMD.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own.
// Retail: Fable.exe (TLC, VC7.1, /GS TU) @ 0x00C6EEF0, 507 bytes, thiscall `ret 8`; slot 2 of vtable 0x012B8A8C (constructor 0x00C550B0), the SSE2 class of the
//   plain-gain renderer CSalRenderDisable (base slot 2 = 0x00C6E8F0, no row).
// Name (NF452, two sources): (1) the GameCODA 2.10 SDK CodeView (CODAApiDbg.lib Debug/SalRenderDisableSSE2.obj) declares the virtual
//   CSalRenderDisableSSE2::Process(const short*, int*(&)[4]), slot 2 of CSalRender, and the class tie runs through CSalSw3DVoice::CreateRenderer
//   0x00C55270 (NF441 CLASS_TABLE / NAME_FIXES: SDK ??0CSalRenderDisableSSE2 = retail 0x00C550B0, whose vtable this slot is); (2) the Release body:
//   `ret 8` (8 argument bytes = the SDK signature; NF445 sigcheck_19.tsv), the same Enter / ramp-down x2(+2) / swap / ramp-up x2(+2) /
//   constant x2(+2) / Leave structure behind the 4-output test; retail 191 v SDK 174 instructions (retail expands the tap mixers with its
//   own kernels). Was ?Process@CVtable_012b8a8c@@UAEXPBFPBQAJ@Z (descriptive stand-in).
// Callees: EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10) through CSalMutex::Enter / Leave on m_mutexUpdate (+0x04;
//   the critical section +0x08), the SSE2 kernels Row00c6ed70::ScaleAccumulate16Ramp 0x00C6ED70 (a static of this TU, initial in ecx) and Row00c6ecd0::ScaleAccumulate16 0x00C6ECD0 (cdecl), g_pAudioMaster 0x0143BC2C (four or more output channels mix outputs 2 and 3; the landed local view of
//   the SDK CSalPlatform::m_nSWOutputChannels at +0x1BC, kept as HEAD text: follow-up port onto engine/CSalPlatform.h).
// Shape: the twin's. The three SSE2 tap mixers retail keeps out of line with no callers (0x00C6EE60 ramp up, 0x00C6EE90 ramp down,
//   0x00C6EEC0 constant gain; descriptive names, in this copy's namespace as the family's kernels) are plain functions above Process and
//   VC7.1 expands all twelve uses, as retail.
// CVtable_012b8a8c::Process(const short* input, long* const* outputs) -- vtable slot 2 of libpk's SSE2 gain-ramp mixer effect: under the effect lock either cross-fade (ramp the current gain record down on every output, swap the current / previous records, ramp the new one up) or mix the input at the record's constant Q15 gain, through the SSE2 kernels
// Leaf: ?Process@CVtable_012b8a8c@@UAEXPBFPBQAJ@Z
// Flags: /GS
// Retail-asm: 0x00C6EE14 `pmulhw xmm0,xmm7` -- the hand SSE2 ramp kernel Row00c6ed70::ScaleAccumulate16Ramp 0x00C6ED70 (landed retail_asm) is a static of this retail TU: VC7.1 passes its `initial` argument in ecx only to a same-TU caller (`mov cx,[eax]` / `xor ecx,ecx` before each call here), so its landed body is repeated verbatim; Process itself is plain C++ (precedent: the landed byte twin 0x00C6FE10 and SSE2 Process 0x00C6E410 / 0x00C6D630).
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared libpk/MixKernelsSIMD.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own.
// Retail: Fable.exe (TLC, VC7.1, /GS TU) @ 0x00C6EEF0, 507 bytes, thiscall `ret 8`; slot 2 of vtable 0x012B8A8C (the only reference: .rdata
//   0x012B8A94), the SSE2 derivative of the gain-ramp effect CVtable_012bedb8 (ctor ??0CVtable_012b8a8c@@QAE@XZ 0x00C550B0, bound).
// Identity: byte twin of the landed MMX Process 0x00C6FE10 (vtable 0x012B8AC8); the only differing relocations are the kernels: here the
//   SSE2 ramp kernel 0x00C6ED70 (Row00c6ed70::ScaleAccumulate16Ramp, landed) and the SSE2 constant-gain kernel 0x00C6ECD0
//   (?ScaleAccumulate16@Row00c6ecd0@@YAXPBFKPAJF@Z, landed) for the MMX 0x00C6FC80 / 0x00C6FBB0. The twin's header calls this address
//   "the SSE2 one (table 0x012B8A8C)".
// Name: descriptive (libpk: no PDB / RTTI): the class follows the bound ctor CVtable_012b8a8c, the method the family's slot-2 name.
// Callees: EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10) on the effect lock (+0x08), the two kernels,
//   g_pAudioMaster 0x0143BC2C (four or more output channels mix outputs 2 and 3).
// Shape: the twin's. The three SSE2 tap mixers retail keeps out of line with no callers (0x00C6EE60 ramp up, 0x00C6EE90 ramp down,
//   0x00C6EEC0 constant gain; descriptive names, in this copy's namespace as the family's kernels) are plain functions above Process and
//   VC7.1 expands all twelve uses, as retail.
#include "libpk/MixKernelsSIMD.h"
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#define FABLE_RETAIL_CSALMUTEX_ENTER_LEAVE
#include "engine/CSalRenderDisableSSE2.h"   // CSalRenderDisableSSE2 : CSalRenderDisable (m_pParamsCurrent +0x30, m_pParamsNew +0x34)
struct CAudioMaster { unsigned char _pad[0x1bc]; unsigned long m_outputChannels; };
extern CAudioMaster* g_pAudioMaster;                                        // 0x0143BC2C
namespace Row00c6ecd0 {
void __cdecl ScaleAccumulate16(const short* src, unsigned long count, long* dst, short gain);   // 0x00C6ECD0
}
namespace Row00c6ed70 {
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
namespace Row00c6eef0 {
void MixGainRampUpSSE2(const short* source, const FilterPan* ramp, long* accumulator)          // 0x00C6EE60 (no callers)
{
    if (ramp->gainDelta != 0)
        Row00c6ed70::ScaleAccumulate16Ramp(source, 16, accumulator, 0, ramp->gainDelta);
}
void MixGainRampDownSSE2(const short* source, const FilterPan* ramp, long* accumulator)        // 0x00C6EE90 (no callers)
{
    if (ramp->gain != 0)
        Row00c6ed70::ScaleAccumulate16Ramp(source, 16, accumulator, ramp->gain, -ramp->gainDelta);
}
void MixGainSSE2(const short* source, unsigned long count, const FilterPan* ramp, long* accumulator)   // 0x00C6EEC0 (no callers)
{
    if (ramp->gain != 0)
        Row00c6ecd0::ScaleAccumulate16(source, count, accumulator, ramp->gain);
}
}
using namespace Row00c6eef0;
// The mixers take a long accumulator (the landed kernels' signature); this view only renames the type.
static inline long* AccumulatorOf(int* pDst) { return reinterpret_cast<long*>(pDst); }

void CSalRenderDisableSSE2::Process(const short* pSrc, int*(&ppDst)[4])
{
    m_mutexUpdate.Enter();
    if (m_bUpdate) {
        m_bUpdate = false;
        MixGainRampDownSSE2(pSrc, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[0]));
        MixGainRampDownSSE2(pSrc, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainRampDownSSE2(pSrc, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[2]));
            MixGainRampDownSSE2(pSrc, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[3]));
        }
        ParamsDisable* pParams = m_pParamsCurrent;
        m_pParamsCurrent = m_pParamsNew;
        m_pParamsNew = pParams;
        MixGainRampUpSSE2(pSrc, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[0]));
        MixGainRampUpSSE2(pSrc, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainRampUpSSE2(pSrc, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[2]));
            MixGainRampUpSSE2(pSrc, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[3]));
        }
    } else {
        MixGainSSE2(pSrc, 16, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[0]));
        MixGainSSE2(pSrc, 16, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainSSE2(pSrc, 16, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[2]));
            MixGainSSE2(pSrc, 16, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[3]));
        }
    }
    m_mutexUpdate.Leave();
}