// CVtable_012b8a78::Process(const short* input, long* const* outputs) -- vtable slot 2 of libpk's SSE2 four-record gain mixer: under the effect lock either cross-fade (ramp each current gain record down on its output, swap the current / previous record arrays, ramp the new records up) or mix the input at each record's constant Q15 gain into outputs 0-1 (and 2-3 with four or more output channels), through the SSE2 kernels
// Leaf: ?Process@CVtable_012b8a78@@UAEXPBFPBQAJ@Z
// Flags: /GS
// Retail-asm: 0x00C6E334 `pmulhw xmm0,xmm7` (0x00C6E317..0x00C6E36F `movdqa xmm7,[gains] ; movdqa xmm6,[increments] ; movdqa xmm0,[eax] ; pmulhw xmm0,xmm7 ; paddsw xmm7,xmm6 ; ... ; dec ecx ; jne`) -- the hand SSE2 ramp kernel Row00c6e290::ScaleAccumulate16Ramp (landed retail_asm) is a static of this retail TU: VC7.1 passes its `initial` argument in ecx only to a same-TU caller (`mov cx,[eax]` / `xor ecx,ecx` before each call here), so its landed body is repeated verbatim; Process itself is plain C++ (precedent: the landed SSE2 Process 0x00C6D630 and MMX Process 0x00C6FE10).
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared libpk/MixKernelsSIMD.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family SIMD_LIBPK.
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
class CVtable_012bed90 : public CVtable_012beda4 {
public:
    SGainRamp  m_first[6];                                                  // +0x28
    SGainRamp  m_second[6];                                                 // +0x40
    SGainRamp* m_pCurrent;                                                  // +0x58
    SGainRamp* m_pPrevious;                                                 // +0x5c
    float      m_scales[6];                                                 // +0x60
};
class CVtable_012b8a78 : public CVtable_012bed90 {
public:
    virtual void Process(const short* input, long* const* outputs);
};
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
void MixGainRampUpSSE2(const short* source, const SGainRamp* ramp, long* accumulator)          // 0x00C6E380 (no callers)
{
    if (ramp->m_step != 0)
        Row00c6e290::ScaleAccumulate16Ramp(source, 16, accumulator, 0, ramp->m_step);
}
void MixGainRampDownSSE2(const short* source, const SGainRamp* ramp, long* accumulator)        // 0x00C6E3B0 (no callers)
{
    if (ramp->m_gain != 0)
        Row00c6e290::ScaleAccumulate16Ramp(source, 16, accumulator, ramp->m_gain, -ramp->m_step);
}
void MixGainSSE2(const short* source, unsigned long count, const SGainRamp* ramp, long* accumulator)   // 0x00C6E3E0 (no callers)
{
    if (ramp->m_gain != 0)
        Row00c6e1f0::ScaleAccumulate16(source, count, accumulator, ramp->m_gain);
}
void CVtable_012b8a78::Process(const short* input, long* const* outputs)
{
    CRITICAL_SECTION* section = &Lock.Section;
    EnterCriticalSection(section);
    if (m_crossfade) {
        m_crossfade = false;
        MixGainRampDownSSE2(input, &m_pCurrent[0], outputs[0]);
        MixGainRampDownSSE2(input, &m_pCurrent[1], outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainRampDownSSE2(input, &m_pCurrent[2], outputs[2]);
            MixGainRampDownSSE2(input, &m_pCurrent[3], outputs[3]);
        }
        SGainRamp* current = m_pCurrent;
        m_pCurrent = m_pPrevious;
        m_pPrevious = current;
        MixGainRampUpSSE2(input, &m_pCurrent[0], outputs[0]);
        MixGainRampUpSSE2(input, &m_pCurrent[1], outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainRampUpSSE2(input, &m_pCurrent[2], outputs[2]);
            MixGainRampUpSSE2(input, &m_pCurrent[3], outputs[3]);
        }
    } else {
        MixGainSSE2(input, 16, &m_pCurrent[0], outputs[0]);
        MixGainSSE2(input, 16, &m_pCurrent[1], outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainSSE2(input, 16, &m_pCurrent[2], outputs[2]);
            MixGainSSE2(input, 16, &m_pCurrent[3], outputs[3]);
        }
    }
    LeaveCriticalSection(section);
}