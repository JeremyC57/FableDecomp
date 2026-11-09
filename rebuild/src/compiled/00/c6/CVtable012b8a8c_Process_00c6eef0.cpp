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
class CVtable_012b8a8c : public CVtable_012bedb8 {
public:
    virtual void Process(const short* input, long* const* outputs);
};
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
void MixGainRampUpSSE2(const short* source, const SGainRamp* ramp, long* accumulator)          // 0x00C6EE60 (no callers)
{
    if (ramp->m_step != 0)
        Row00c6ed70::ScaleAccumulate16Ramp(source, 16, accumulator, 0, ramp->m_step);
}
void MixGainRampDownSSE2(const short* source, const SGainRamp* ramp, long* accumulator)        // 0x00C6EE90 (no callers)
{
    if (ramp->m_gain != 0)
        Row00c6ed70::ScaleAccumulate16Ramp(source, 16, accumulator, ramp->m_gain, -ramp->m_step);
}
void MixGainSSE2(const short* source, unsigned long count, const SGainRamp* ramp, long* accumulator)   // 0x00C6EEC0 (no callers)
{
    if (ramp->m_gain != 0)
        Row00c6ecd0::ScaleAccumulate16(source, count, accumulator, ramp->m_gain);
}
}
using namespace Row00c6eef0;
void CVtable_012b8a8c::Process(const short* input, long* const* outputs)
{
    CRITICAL_SECTION* section = &Lock.Section;
    EnterCriticalSection(section);
    if (m_crossfade) {
        m_crossfade = false;
        MixGainRampDownSSE2(input, m_pCurrent, outputs[0]);
        MixGainRampDownSSE2(input, m_pCurrent, outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainRampDownSSE2(input, m_pCurrent, outputs[2]);
            MixGainRampDownSSE2(input, m_pCurrent, outputs[3]);
        }
        SGainRamp* current = m_pCurrent;
        m_pCurrent = m_pPrevious;
        m_pPrevious = current;
        MixGainRampUpSSE2(input, m_pCurrent, outputs[0]);
        MixGainRampUpSSE2(input, m_pCurrent, outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainRampUpSSE2(input, m_pCurrent, outputs[2]);
            MixGainRampUpSSE2(input, m_pCurrent, outputs[3]);
        }
    } else {
        MixGainSSE2(input, 16, m_pCurrent, outputs[0]);
        MixGainSSE2(input, 16, m_pCurrent, outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainSSE2(input, 16, m_pCurrent, outputs[2]);
            MixGainSSE2(input, 16, m_pCurrent, outputs[3]);
        }
    }
    LeaveCriticalSection(section);
}