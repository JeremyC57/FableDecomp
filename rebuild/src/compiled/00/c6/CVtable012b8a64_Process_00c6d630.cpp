// CVtable_012b8a64::Process(const short* input, long* const* outputs) -- vtable slot 2 of the SSE2 multi-tap delay effect: copy the 128-sample block into the aligned history, then under the effect lock either cross-fade (ramp the old taps down, swap the tap sets, ramp the new ones up) or mix each tap at its constant gain, through the SSE2 kernels; finally slide the last 32 samples below the buffer for the next block with a hand SSE2 copy
// Leaf: ?Process@CVtable_012b8a64@@UAEXPBFPBQAJ@Z
// Flags: /GS
// Retail-asm: 0x00C6D923..0x00C6D94C `mov esi,[esp+14h] ; mov edi,[esp+18h] ; movdqa xmm0,[esi] ; movdqa xmm1,[esi+10h] ; movdqa xmm2,[esi+20h] ; movdqa xmm3,[esi+30h] ; movdqa [edi],xmm0 ; movdqa [edi+10h],xmm1 ; movdqa [edi+20h],xmm2 ; movdqa [edi+30h],xmm3` -- a hand SSE2 copy of the last 64 history bytes on fixed esi/edi loaded from two C locals that VC7.1 homed in the dead argument slots (`mov [esp+14h],edx ; mov [esp+18h],ebx`); VC7.1 RTM has no intrinsic that names esi/edi or reloads them from frame slots, and the C path of the same slot (0x00C6CC60) is `rep movsd`; same family as the landed ClearHistory 0x00C6D560 (slot 1 of this vtable) and the SIMD kernels 0x00C6D370 / 0x00C6D410. The rest of the body is C++.
// Retail: Fable.exe (TLC, VC7.1, /GS TU) @ 0x00C6D630, 808 bytes, thiscall `ret 8`; slot 2 of vtable 0x012B8A64 (the only
//   reference: .rdata 0x012B8A6C), the SSE2 derivative of libpk's multi-tap delay effect (ctor 0x00C55070); the base slot 2 is
//   the C Process 0x00C6CC60 (same body over MixDelayTapRampDown / MixDelayTapRampUp / MixDelayTap 0x00C6CB80 / 0x00C6CAF0 /
//   0x00C6CC10), the MMX slot 2 is 0x00C6F3E0.
// Name: descriptive (libpk: no PDB/RTTI); `Process` is the landed model's name for slot 2 of CVtable_012beda4 (Update
//   0x00C6D150 / ClearHistory 0x00C6D560 sources); the catalog label "CAudioMixer_ProcessChannels" is a propagated guess.
// Callees: EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10) on the effect lock (+0x08), memcpy
//   intrinsic (128 samples into the aligned history +0x178), the SSE2 kernels MixSamplesRampSSE2 0x00C6D410 (a static of this
//   TU: VC7.1 passes its first argument in ecx, so its landed body is repeated here) and ScaleAccumulate16 0x00C6D370
//   (cdecl), g_pAudioMaster 0x0143BC2C (four or more output channels mix taps 2 and 3).
// Shape: the three tap mixers retail keeps out of line with no callers (0x00C6D590 ramp up, 0x00C6D5C0 ramp down,
//   0x00C6D600 constant gain) are written as plain functions above Process and VC7.1 expands all twelve uses, as retail;
//   the tap-set swap loads the current set into a named local first (the other two spellings store +0x1dc first).
//   The TU is /GS (the kernels carry cookies), which also decides the static's register convention (without /GS VC7.1
//   passes `step` in eax as well).
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
class CVtable_012b8a64 : public CVtable_012bed7c {                         // vtable 0x012B8A64 (ctor 0x00C55070)
public:
    virtual void Process(const short* input, long* const* outputs);         // slot 2
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
__asm {
 mov ecx,groupCount
 mov eax,source
 mov edx,accumulator
 movdqa xmm7,gains
 movdqa xmm6,increments
 align 16
 next_block:
 movdqu xmm0,[eax]
 pmulhw xmm0,xmm7
 paddsw xmm7,xmm6
 pxor xmm1,xmm1
 pxor xmm2,xmm2
 punpcklwd xmm1,xmm0
 punpckhwd xmm2,xmm0
 psrad xmm1,15
 psrad xmm2,15
 paddd xmm1,[edx]
 paddd xmm2,[edx+10h]
 movdqa [edx],xmm1
 movdqa [edx+10h],xmm2
 add eax,10h
 add edx,20h
 dec ecx
 jne next_block
}
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

void CVtable_012b8a64::Process(const short* input, long* const* outputs)
{
    memcpy(m_pAlignedBuffer, input, 128 * sizeof(short));
    EnterCriticalSection(&Lock.Section);
    if (m_crossfade) {
        m_crossfade = false;
        MixDelayTapRampDownSSE2((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[0], outputs[0]);
        MixDelayTapRampDownSSE2((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[1], outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixDelayTapRampDownSSE2((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[2], outputs[2]);
            MixDelayTapRampDownSSE2((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[3], outputs[3]);
        }
        SRecord_00c6ca20* current = m_pCurrent;
        m_pCurrent = m_pPrevious;
        m_pPrevious = current;
        MixDelayTapRampUpSSE2((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[0], outputs[0]);
        MixDelayTapRampUpSSE2((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[1], outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixDelayTapRampUpSSE2((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[2], outputs[2]);
            MixDelayTapRampUpSSE2((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[3], outputs[3]);
        }
    } else {
        MixDelayTapSSE2((const short*)m_pAlignedBuffer, 16, &m_pCurrent->m_taps[0], outputs[0]);
        MixDelayTapSSE2((const short*)m_pAlignedBuffer, 16, &m_pCurrent->m_taps[1], outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixDelayTapSSE2((const short*)m_pAlignedBuffer, 16, &m_pCurrent->m_taps[2], outputs[2]);
            MixDelayTapSSE2((const short*)m_pAlignedBuffer, 16, &m_pCurrent->m_taps[3], outputs[3]);
        }
    }
    LeaveCriticalSection(&Lock.Section);
    unsigned char* source = m_pAlignedBuffer + 0xc0;
    unsigned char* history = m_pAlignedBuffer - 0x40;
    __asm {
        mov     esi, source
        mov     edi, history
        movdqa  xmm0, [esi]
        movdqa  xmm1, [esi + 10h]
        movdqa  xmm2, [esi + 20h]
        movdqa  xmm3, [esi + 30h]
        movdqa  [edi], xmm0
        movdqa  [edi + 10h], xmm1
        movdqa  [edi + 20h], xmm2
        movdqa  [edi + 30h], xmm3
    }
}