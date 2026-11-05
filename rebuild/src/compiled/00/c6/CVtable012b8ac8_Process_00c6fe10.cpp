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
struct CRITICAL_SECTION { void* DebugInfo; long LockCount, RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE10
class CUnprovenLibraryObject_012B5868 { public: virtual ~CUnprovenLibraryObject_012B5868() {} };
class CUnprovenLock_012B8FC8 { public: virtual ~CUnprovenLock_012B8FC8() {} CRITICAL_SECTION Section; };
class CVtable_012beda4 : public CUnprovenLibraryObject_012B5868 {
public:
    virtual ~CVtable_012beda4() {}
    virtual void Clear() {}
    virtual void Process(const short*, long* const*) {}
    virtual void SetVolume(float, unsigned long, bool) = 0;
    virtual void SetParameters(long, long, long, float, long, bool, bool) {}
    CUnprovenLock_012B8FC8 Lock;
    float m_volume;
    bool  m_crossfade;
};
struct CAudioMaster { unsigned char _pad[0x1bc]; unsigned long m_outputChannels; };
extern CAudioMaster* g_pAudioMaster;                                        // 0x0143BC2C
struct SGainRamp {
    unsigned short m_gain;
    short          m_step;
};
class CVtable_012bedb8 : public CVtable_012beda4 {
public:
    SGainRamp  m_first;
    SGainRamp  m_second;
    SGainRamp* m_pCurrent;
    SGainRamp* m_pPrevious;
};
class CVtable_012b8ac8 : public CVtable_012bedb8 {
public:
    virtual void Process(const short* input, long* const* outputs);
};
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
void MixGainRampUpMMX(const short* source, const SGainRamp* ramp, long* accumulator)
{
    if (ramp->m_step != 0)
        CUnprovenMixKernel_00c6fc80(source, 16, accumulator, 0, ramp->m_step);
}
void MixGainRampDownMMX(const short* source, const SGainRamp* ramp, long* accumulator)
{
    if (ramp->m_gain != 0)
        CUnprovenMixKernel_00c6fc80(source, 16, accumulator, ramp->m_gain, -ramp->m_step);
}
void MixGainMMX(const short* source, unsigned long count, const SGainRamp* ramp, long* accumulator)
{
    if (ramp->m_gain != 0)
        ScaleAccumulate16MMX_00c6fbb0(source, count, accumulator, ramp->m_gain);
}
void CVtable_012b8ac8::Process(const short* input, long* const* outputs)
{
    CRITICAL_SECTION* section = &Lock.Section;
    EnterCriticalSection(section);
    if (m_crossfade) {
        m_crossfade = false;
        MixGainRampDownMMX(input, m_pCurrent, outputs[0]);
        MixGainRampDownMMX(input, m_pCurrent, outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainRampDownMMX(input, m_pCurrent, outputs[2]);
            MixGainRampDownMMX(input, m_pCurrent, outputs[3]);
        }
        SGainRamp* current = m_pCurrent;
        m_pCurrent = m_pPrevious;
        m_pPrevious = current;
        MixGainRampUpMMX(input, m_pCurrent, outputs[0]);
        MixGainRampUpMMX(input, m_pCurrent, outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainRampUpMMX(input, m_pCurrent, outputs[2]);
            MixGainRampUpMMX(input, m_pCurrent, outputs[3]);
        }
    } else {
        MixGainMMX(input, 16, m_pCurrent, outputs[0]);
        MixGainMMX(input, 16, m_pCurrent, outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainMMX(input, 16, m_pCurrent, outputs[2]);
            MixGainMMX(input, 16, m_pCurrent, outputs[3]);
        }
    }
    LeaveCriticalSection(section);
}