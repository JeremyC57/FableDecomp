// CSalRenderDisableMMX::Process(const short* pSrc, int*(&ppDst)[4]) -- vtable slot 2 of the GameCODA MMX plain-gain renderer: under the update
//   lock either cross-fade (ramp the current FilterPan down on every output, swap the parameter sets, ramp the new one up) or mix the input at
//   the current Q15 gain, through the MMX kernels (outputs 2 / 3 only with four or more output channels)
// Leaf: ?Process@CSalRenderDisableMMX@@UAEXPBFAAY03PAH@Z
// Flags: /GS
// Retail-asm: 0x00C6FCE3 `pmulhw mm0,mm7` (0x00C6FCD0..0x00C6FD4C `movq mm7,[gains] ; movq mm6,[increments] ; movq mm0,[eax] ; pmulhw mm0,mm7 ; paddsw mm7,mm6 ; ... ; emms`) -- the hand MMX ramp kernel CUnprovenMixKernel_00c6fc80 (landed retail_asm) is a static of this retail TU: VC7.1 passes its `initial` argument in ecx only to a same-TU caller (`mov cx,[eax]` / `xor ecx,ecx` before each call here), so its landed body is repeated verbatim; Process itself is plain C++ (same precedent as the landed SSE2 Process 0x00C6D630 repeating MixSamplesRampSSE2).
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared libpk/MixKernelsSIMD.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family SIMD_LIBPK.
// Retail: Fable.exe (TLC, VC7.1, /GS TU) @ 0x00C6FE10, 507 bytes, thiscall `ret 8`; slot 2 of vtable 0x012B8AC8 (constructor 0x00C55110), the MMX class of the
//   plain-gain renderer CSalRenderDisable (base slot 2 = 0x00C6E8F0, no row).
// Name (NF452, two sources): (1) the GameCODA 2.10 SDK CodeView (CODAApiDbg.lib Debug/SalRenderDisableMMX.obj) declares the virtual
//   CSalRenderDisableMMX::Process(const short*, int*(&)[4]), slot 2 of CSalRender, and the class tie runs through CSalSw3DVoice::CreateRenderer
//   0x00C55270 (NF441 CLASS_TABLE / NAME_FIXES: SDK ??0CSalRenderDisableMMX = retail 0x00C55110, whose vtable this slot is); (2) the Release body:
//   `ret 8` (8 argument bytes = the SDK signature; NF445 sigcheck_19.tsv), the same Enter / ramp-down x2(+2) / swap / ramp-up x2(+2) /
//   constant x2(+2) / Leave structure behind the 4-output test; retail 191 v SDK 174 instructions (retail expands the tap mixers with its
//   own kernels). Was ?Process@CVtable_012b8ac8@@UAEXPBFPBQAJ@Z (descriptive stand-in).
// Callees: EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10) through CSalMutex::Enter / Leave on m_mutexUpdate (+0x04;
//   the critical section +0x08), the MMX kernels CUnprovenMixKernel_00c6fc80 0x00C6FC80 (a static of this TU, initial in ecx) and ScaleAccumulate16MMX_00c6fbb0 0x00C6FBB0 (cdecl), g_pAudioMaster 0x0143BC2C (four or more output channels mix outputs 2 and 3; the landed local view of
//   the SDK CSalPlatform::m_nSWOutputChannels at +0x1BC, kept as HEAD text: follow-up port onto engine/CSalPlatform.h).
// Shape: the three MMX tap mixers retail keeps out of line with no callers (0x00C6FD80 ramp up, 0x00C6FDB0 ramp down,
//   0x00C6FDE0 constant gain; descriptive names) are plain functions above Process and VC7.1 expands all twelve uses, as retail;
//   the lock pointer is a local (retail homes it in the `push ecx` slot and reloads it for Leave); the tap-set swap loads the
//   current record into a named local first (as the SSE2 sibling).
// CVtable_012b8ac8::Process(const short* input, long* const* outputs) -- vtable slot 2 of libpk's MMX gain-ramp mixer effect: under the effect lock either cross-fade (ramp the current gain record down on every output, swap the current / previous records, ramp the new one up) or mix the input at the record's constant Q15 gain, through the MMX kernels
// Leaf: ?Process@CVtable_012b8ac8@@UAEXPBFPBQAJ@Z
// Flags: /GS
// Retail-asm: 0x00C6FCE3 `pmulhw mm0,mm7` (0x00C6FCD0..0x00C6FD4C `movq mm7,[gains] ; movq mm6,[increments] ; movq mm0,[eax] ; pmulhw mm0,mm7 ; paddsw mm7,mm6 ; ... ; emms`) -- the hand MMX ramp kernel CUnprovenMixKernel_00c6fc80 (landed retail_asm) is a static of this retail TU: VC7.1 passes its `initial` argument in ecx only to a same-TU caller (`mov cx,[eax]` / `xor ecx,ecx` before each call here), so its landed body is repeated verbatim; Process itself is plain C++ (same precedent as the landed SSE2 Process 0x00C6D630 repeating MixSamplesRampSSE2).
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared libpk/MixKernelsSIMD.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family SIMD_LIBPK.
// Retail: Fable.exe (TLC, VC7.1, /GS TU) @ 0x00C6FE10, 507 bytes, thiscall `ret 8`; slot 2 of vtable 0x012B8AC8 (the only
//   reference: .rdata 0x012B8AD0), the MMX derivative of the gain-ramp effect CVtable_012bedb8 (base ctor 0x00C6E760 landed; MMX
//   ctor 0x00C55110; slots 3 / 4 shared with the base: SetVolume 0x00C6EC50, SetParameters 0x00C6EC80). The base C path of the
//   same slot is 0x00C6E8F0, the SSE2 one (table 0x012B8A8C) 0x00C6EEF0.
// Name: descriptive (libpk: no PDB / RTTI). The class name follows the catalog's CVtable_012b8ac8 (its ??_G 0x00C555B0), the
//   method name the landed family's slot-2 name (Process 0x00C6D630 / 0x00C6CC60).
// Callees: EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10) on the effect lock (+0x08),
//   CUnprovenMixKernel_00c6fc80 0x00C6FC80 (static, initial in ecx), ScaleAccumulate16MMX_00c6fbb0 0x00C6FBB0 (cdecl, landed),
//   g_pAudioMaster 0x0143BC2C (four or more output channels mix outputs 2 and 3).
// Shape: the three MMX tap mixers retail keeps out of line with no callers (0x00C6FD80 ramp up, 0x00C6FDB0 ramp down,
//   0x00C6FDE0 constant gain; descriptive names) are plain functions above Process and VC7.1 expands all twelve uses, as retail;
//   the lock pointer is a local (retail homes it in the `push ecx` slot and reloads it for Leave); the tap-set swap loads the
//   current record into a named local first (as the SSE2 sibling).
#include "libpk/MixKernelsSIMD.h"
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#define FABLE_RETAIL_CSALMUTEX_ENTER_LEAVE
#include "engine/CSalRenderDisableMMX.h"   // CSalRenderDisableMMX : CSalRenderDisable (m_pParamsCurrent +0x30, m_pParamsNew +0x34)
struct CAudioMaster { unsigned char _pad[0x1bc]; unsigned long m_outputChannels; };
extern CAudioMaster* g_pAudioMaster;                                        // 0x0143BC2C
void __cdecl ScaleAccumulate16MMX_00c6fbb0(const short* src, unsigned long count, long* dst, short gain);   // 0x00C6FBB0
static __declspec(noinline) void __cdecl CUnprovenMixKernel_00c6fc80(const short* src,unsigned long count,long* dst,short initial,short step){
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
void __cdecl MixQuadsMMX_00c6fc80(const short* src,unsigned long count,long* dst,short initial,short step){
CUnprovenMixKernel_00c6fc80(src,count,dst,initial,step);
}
void MixGainRampUpMMX(const short* source, const FilterPan* ramp, long* accumulator)
{
    if (ramp->gainDelta != 0)
        CUnprovenMixKernel_00c6fc80(source, 16, accumulator, 0, ramp->gainDelta);
}
void MixGainRampDownMMX(const short* source, const FilterPan* ramp, long* accumulator)
{
    if (ramp->gain != 0)
        CUnprovenMixKernel_00c6fc80(source, 16, accumulator, ramp->gain, -ramp->gainDelta);
}
void MixGainMMX(const short* source, unsigned long count, const FilterPan* ramp, long* accumulator)
{
    if (ramp->gain != 0)
        ScaleAccumulate16MMX_00c6fbb0(source, count, accumulator, ramp->gain);
}
// The mixers take a long accumulator (the landed kernels' signature); this view only renames the type.
static inline long* AccumulatorOf(int* pDst) { return reinterpret_cast<long*>(pDst); }

void CSalRenderDisableMMX::Process(const short* pSrc, int*(&ppDst)[4])
{
    m_mutexUpdate.Enter();
    if (m_bUpdate) {
        m_bUpdate = false;
        MixGainRampDownMMX(pSrc, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[0]));
        MixGainRampDownMMX(pSrc, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainRampDownMMX(pSrc, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[2]));
            MixGainRampDownMMX(pSrc, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[3]));
        }
        ParamsDisable* pParams = m_pParamsCurrent;
        m_pParamsCurrent = m_pParamsNew;
        m_pParamsNew = pParams;
        MixGainRampUpMMX(pSrc, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[0]));
        MixGainRampUpMMX(pSrc, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainRampUpMMX(pSrc, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[2]));
            MixGainRampUpMMX(pSrc, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[3]));
        }
    } else {
        MixGainMMX(pSrc, 16, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[0]));
        MixGainMMX(pSrc, 16, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[1]));
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainMMX(pSrc, 16, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[2]));
            MixGainMMX(pSrc, 16, &m_pParamsCurrent->pan, AccumulatorOf(ppDst[3]));
        }
    }
    m_mutexUpdate.Leave();
}