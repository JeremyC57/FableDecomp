// CVtable_012b8aa0::Process(const short* input, long* const* outputs) -- vtable slot 2 of the MMX multi-tap delay effect: copy the 128-sample block into the aligned history, then under the effect lock either cross-fade (ramp the old taps down, swap the tap sets, ramp the new ones up) or mix each tap at its constant gain, then keep the last 64 history bytes in front of the buffer
// Leaf: ?Process@CVtable_012b8aa0@@UAEXPBFPBQAJ@Z
// Flags: /GS
// Retail-asm: 0x00C6F6A3 `movq mm0,[esi]` (0x00C6F69B..0x00C6F6DC `mov esi,[esp+14h] ; mov edi,[esp+18h] ; movq mm0,[esi] ; movq [edi],mm0 ; ... ; movq [edi+38h],mm7 ; emms`) -- the hand MMX copy of the last 64 history bytes
//   (LIBPK_MMX_COPY64 of libpk/MixKernelsSIMD.h) and, in the static kernel CUnprovenMixKernel_00c6f1a0 0x00C6F1A0 (landed, same text), LIBPK_MMX_SCALE_ACCUMULATE16_RAMP; the row has no asm of its own.
// Identity: slot 2 of vtable 0x012B8AA0 (the only reference: .rdata 0x012B8AA8; reloc_bindings ??_7CVtable_012b8aa0@@6B@), the MMX derivative of libpk's multi-tap delay effect; the landed
//   SSE2 sibling 0x00C6D630 (CVtable_012b8a64::Process) names this address as "the MMX slot 2"; same body over the MMX kernels ScaleAccumulate16MMX 0x00C6F0F0 / CUnprovenMixKernel_00c6f1a0.
// Name: descriptive (libpk: no PDB / RTTI): class after the bound vtable symbol, method after the family's landed slot-2 name. The catalog label "CAudioMixer_ProcessChannels_64bit" is a guess.
//   Not vendor-library bytes in the toolchain_library sense: the module is compiled C++ (/GS cookie in its kernels) landed row by row (0x00C6D630, 0x00C6EEF0, 0x00C6F990, 0x00C6FE10).
// Models: local classes as in every landed row of this module (libpk has no shared class headers for the effect family yet).
// Retail: Fable.exe (TLC, VC7.1, /GS TU) @ 0x00C6F3E0, 834 bytes, thiscall `ret 8`.
#include "libpk/MixKernelsSIMD.h"
#include <string.h>
#pragma intrinsic(memcpy)
struct CRITICAL_SECTION { void* DebugInfo; long LockCount, RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE10
class CUnprovenLibraryObject_012B5868 { public: virtual ~CUnprovenLibraryObject_012B5868() {} };
class CUnprovenLock_012B8FC8 { public: virtual ~CUnprovenLock_012B8FC8() {} CRITICAL_SECTION Section; };
class CVtable_012beda4 : public CUnprovenLibraryObject_012B5868 {          // 0x28 bytes (ctor 0x00C6E6A0 landed)
public:
    virtual ~CVtable_012beda4() {}
    virtual void Clear() {}
    virtual void Process(const short*, long* const*) {}
    virtual void SetVolume(float, unsigned long, bool) = 0;
    virtual void Update(float, float, float, float, const float*, bool, bool) {}
    CUnprovenLock_012B8FC8 Lock;                                            // +0x04 (its CRITICAL_SECTION at +0x08)
    float m_volume;                                                         // +0x20
    bool  m_crossfade;                                                      // +0x24
};
struct CAudioMaster { unsigned char _pad[0x1bc]; unsigned long m_outputChannels; };
extern CAudioMaster* g_pAudioMaster;                                        // 0x0143BC2C
struct SDelayTap {                        // 8 bytes (landed MixDelayTapRampDown 0x00C6CB80 model)
    long           m_delay;               // +0x00 samples back into the history
    unsigned short m_gain;                // +0x04 Q15 gain
    short          m_step;                // +0x06 Q15 gain step (ramps)
};
struct SRecord_00c6ca20 { SDelayTap m_taps[6]; };                          // the landed ctor's twelve-word tap set
class CVtable_012bed7c : public CVtable_012beda4 {                         // vtable 0x012BED7C (ctor 0x00C6CA20 landed)
public:
    unsigned char     m_state[0x40];                                        // +0x28
    unsigned char     m_buffer[0x110];                                      // +0x68
    unsigned char*    m_pAlignedBuffer;                                     // +0x178
    SRecord_00c6ca20  m_first;                                              // +0x17c
    SRecord_00c6ca20  m_second;                                             // +0x1ac
    SRecord_00c6ca20* m_pCurrent;                                           // +0x1dc
    SRecord_00c6ca20* m_pPrevious;                                          // +0x1e0
    float             m_gains[6];                                           // +0x1e4
    long              m_delays[6];                                          // +0x1fc
};
class CVtable_012b8aa0 : public CVtable_012bed7c {                         // vtable 0x012B8AA0
public:
    virtual void Process(const short* input, long* const* outputs);         // slot 2
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

void CVtable_012b8aa0::Process(const short* input, long* const* outputs)
{
    memcpy(m_pAlignedBuffer, input, 128 * sizeof(short));
    EnterCriticalSection(&Lock.Section);
    if (m_crossfade) {
        m_crossfade = false;
        MixDelayTapRampDownMMX((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[0], outputs[0]);
        MixDelayTapRampDownMMX((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[1], outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixDelayTapRampDownMMX((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[2], outputs[2]);
            MixDelayTapRampDownMMX((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[3], outputs[3]);
        }
        SRecord_00c6ca20* current = m_pCurrent;
        m_pCurrent = m_pPrevious;
        m_pPrevious = current;
        MixDelayTapRampUpMMX((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[0], outputs[0]);
        MixDelayTapRampUpMMX((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[1], outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixDelayTapRampUpMMX((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[2], outputs[2]);
            MixDelayTapRampUpMMX((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[3], outputs[3]);
        }
    } else {
        MixDelayTapMMX((const short*)m_pAlignedBuffer, 16, &m_pCurrent->m_taps[0], outputs[0]);
        MixDelayTapMMX((const short*)m_pAlignedBuffer, 16, &m_pCurrent->m_taps[1], outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixDelayTapMMX((const short*)m_pAlignedBuffer, 16, &m_pCurrent->m_taps[2], outputs[2]);
            MixDelayTapMMX((const short*)m_pAlignedBuffer, 16, &m_pCurrent->m_taps[3], outputs[3]);
        }
    }
    LeaveCriticalSection(&Lock.Section);
    unsigned char* source = m_pAlignedBuffer + 0xc0;
    unsigned char* history = m_pAlignedBuffer - 0x40;
    LIBPK_MMX_COPY64(source, history);
}