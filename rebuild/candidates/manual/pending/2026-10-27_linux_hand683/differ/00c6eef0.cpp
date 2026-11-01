// CVtable_012b8a8c::Process(const short* src, long* const* outputs) -- vtable slot 2 of the SSE2 gain-ramp mixer effect: under the effect's lock, mix one 128-sample block into the front pair (and the rear pair when the audio master has four or more output channels), either at the current gain (ScaleAccumulate16) or, when SetVolume flagged a change, ramping the old record down and the new record up (ScaleAccumulate16Ramp) and swapping the current / previous gain records
// Grade: DIFFER(511v507) -- record swap form: retail loads m_pCurrent then m_pPrevious, stores m_pPrevious, copies the new current into eax (`mov eax,ecx`) and then stores m_pCurrent (5 instructions); every C++ swap form tried (two locals either order, one local either order, std::swap-style helper, a `next` local forwarded into the first m_step test) loads m_pPrevious first and stores in 4 instructions (or, with the `old = m_pCurrent; m_pCurrent = m_pPrevious; m_pPrevious = old;` form, forwards the pointer into edx and reads m_step zero-extended, 517 bytes). Everything else is byte-identical, including the private `initial`-in-ecx convention of the same-TU ScaleAccumulate16Ramp kernel (needs /GS and kernel locals WITHOUT __declspec(align(16))). 24 probes.
// Leaf: ?Process@CVtable_012b8a8c@@UAEXPBFPAPAJ@Z
// Flags: /GS
// NOTE for the coordinator: the two same-TU kernels below are the landed retail_asm bodies of 0x00C6ED70 / 0x00C6ECD0 (ScaleAccumulate16Ramp / ScaleAccumulate16) copied as stand-ins because the ramp kernel's private register convention only arises when it is a static function of the same TU; this file therefore holds `__asm` and must live under retail_asm/ (or the kernels be replaced) if it ever lands.
struct SCriticalSection { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(SCriticalSection* section);   // 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(SCriticalSection* section);   // 0x0143FE10
struct CAudioMaster { unsigned char _pad[0x1bc]; unsigned long m_outputChannels; };
extern CAudioMaster* g_pAudioMaster;                     // 0x0143BC2C
class CAutoLock {                                        // scoped critical-section holder (the lock pointer lives in the frame)
public:
    SCriticalSection* m_pSection;
    CAutoLock(SCriticalSection* section) : m_pSection(section) { EnterCriticalSection(m_pSection); }
    ~CAutoLock() { LeaveCriticalSection(m_pSection); }
};
struct SGainRamp { unsigned short m_gain; short m_step; };
class CVtable_012beda4 { public:
    virtual void v00();
    void*            _lockVtable;                        // +0x04
    SCriticalSection m_lock;                             // +0x08
    float            m_volume;                           // +0x20
    bool             m_alternate;                        // +0x24
};
class CVtable_012bedb8 : public CVtable_012beda4 { public:
    virtual void v01();
    virtual void Process(const short* src, long* const* outputs);           // slot 2 (0x00C6E8F0)
    virtual void SetVolume(float, unsigned long, bool);                     // slot 3 (0x00C6EC50)
    virtual void SetParameters(long, long, long, float, long, bool, bool);  // slot 4 (0x00C6EC80)
    SGainRamp  m_first;                                  // +0x28
    SGainRamp  m_second;                                 // +0x2c
    SGainRamp* m_pCurrent;                               // +0x30
    SGainRamp* m_pPrevious;                              // +0x34
};
class CVtable_012b8a8c : public CVtable_012bedb8 { public:
    virtual void Process(const short* src, long* const* outputs);           // slot 2 (0x00C6EEF0)
};
// Retail-asm: 0x00C6EE14 `pmulhw xmm0,xmm7` / 0x00C6ECD0 `movdqa xmm0,[eax] ; pmulhw xmm0,xmm7 ; punpcklwd xmm1,xmm0 ; psrad xmm1,15 ; paddd xmm1,[edx]` -- the two TU-local SSE2 mixing kernels retail's
//   mixer file defines (landed retail_asm rows 0x00C6ED70 ScaleAccumulate16Ramp and 0x00C6ECD0 ScaleAccumulate16): this row's
//   own body is plain C++, but the static ramp kernel must sit in the same TU with its real body so VC7.1 gives it the same
//   private convention (initial gain in ecx, the other four arguments on the stack, caller cleans): a C++ stand-in body makes VC
//   pass `step` in eax as well (probe 1: `add esp,0Ch`). Intrinsics allocate other xmm registers (the landed kernels' evidence).
static __declspec(noinline) void __cdecl ScaleAccumulate16Ramp(const short* src, unsigned long count, long* dst, short initial, short step)
{
    short gains[8];
    short increments[8];
    gains[0] = (short)(initial + 0 * step);
    gains[1] = (short)(initial + 1 * step);
    gains[2] = (short)(initial + 2 * step);
    gains[3] = (short)(initial + 3 * step);
    gains[4] = (short)(initial + 4 * step);
    gains[5] = (short)(initial + 5 * step);
    gains[6] = (short)(initial + 6 * step);
    gains[7] = (short)(initial + 7 * step);
    increments[0] = (short)(step * 8);
    increments[1] = (short)(step * 8);
    increments[2] = (short)(step * 8);
    increments[3] = (short)(step * 8);
    increments[4] = (short)(step * 8);
    increments[5] = (short)(step * 8);
    increments[6] = (short)(step * 8);
    increments[7] = (short)(step * 8);
    __asm {
        mov ecx, count
        mov eax, src
        mov edx, dst
        movdqa xmm7, gains
        movdqa xmm6, increments
        align 16
    next_block:
        movdqa xmm0, [eax]
        pmulhw xmm0, xmm7
        paddsw xmm7, xmm6
        pxor xmm1, xmm1
        pxor xmm2, xmm2
        punpcklwd xmm1, xmm0
        punpckhwd xmm2, xmm0
        psrad xmm1, 15
        psrad xmm2, 15
        paddd xmm1, [edx]
        paddd xmm2, [edx + 10h]
        movdqa [edx], xmm1
        movdqa [edx + 10h], xmm2
        add eax, 10h
        add edx, 20h
        dec ecx
        jne next_block
    }
}
void __cdecl ScaleAccumulate16(const short* src, unsigned long count, long* dst, short gain)   // 0x00C6ECD0 (the same TU; external linkage)
{
    short gains[8];
    gains[0] = gain;
    gains[1] = gain;
    gains[2] = gain;
    gains[3] = gain;
    gains[4] = gain;
    gains[5] = gain;
    gains[6] = gain;
    gains[7] = gain;
    __asm {
        mov ecx, count
        mov eax, src
        mov edx, dst
        movdqa xmm7, gains
        align 16
    next_block:
        movdqa xmm0, [eax]
        pmulhw xmm0, xmm7
        pxor xmm1, xmm1
        pxor xmm2, xmm2
        punpcklwd xmm1, xmm0
        punpckhwd xmm2, xmm0
        psrad xmm1, 15
        psrad xmm2, 15
        paddd xmm1, [edx]
        paddd xmm2, [edx + 10h]
        movdqa [edx], xmm1
        movdqa [edx + 10h], xmm2
        add eax, 10h
        add edx, 20h
        dec ecx
        jne next_block
    }
}

void CVtable_012b8a8c::Process(const short* src, long* const* outputs)
{
    CAutoLock lock(&m_lock);
    if (m_alternate) {
        SGainRamp* ramp = m_pCurrent;
        m_alternate = false;
        if (ramp->m_gain)
            ScaleAccumulate16Ramp(src, 16, outputs[0], ramp->m_gain, (short)-ramp->m_step);
        if (m_pCurrent->m_gain)
            ScaleAccumulate16Ramp(src, 16, outputs[1], m_pCurrent->m_gain, (short)-m_pCurrent->m_step);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            if (m_pCurrent->m_gain)
                ScaleAccumulate16Ramp(src, 16, outputs[2], m_pCurrent->m_gain, (short)-m_pCurrent->m_step);
            if (m_pCurrent->m_gain)
                ScaleAccumulate16Ramp(src, 16, outputs[3], m_pCurrent->m_gain, (short)-m_pCurrent->m_step);
        }
        SGainRamp* current = m_pCurrent;
        SGainRamp* previous = m_pPrevious;
        m_pPrevious = current;
        m_pCurrent = previous;
        if (m_pCurrent->m_step)
            ScaleAccumulate16Ramp(src, 16, outputs[0], 0, m_pCurrent->m_step);
        if (m_pCurrent->m_step)
            ScaleAccumulate16Ramp(src, 16, outputs[1], 0, m_pCurrent->m_step);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            if (m_pCurrent->m_step)
                ScaleAccumulate16Ramp(src, 16, outputs[2], 0, m_pCurrent->m_step);
            if (m_pCurrent->m_step)
                ScaleAccumulate16Ramp(src, 16, outputs[3], 0, m_pCurrent->m_step);
        }
    } else {
        if (m_pCurrent->m_gain)
            ScaleAccumulate16(src, 16, outputs[0], m_pCurrent->m_gain);
        if (m_pCurrent->m_gain)
            ScaleAccumulate16(src, 16, outputs[1], m_pCurrent->m_gain);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            if (m_pCurrent->m_gain)
                ScaleAccumulate16(src, 16, outputs[2], m_pCurrent->m_gain);
            if (m_pCurrent->m_gain)
                ScaleAccumulate16(src, 16, outputs[3], m_pCurrent->m_gain);
        }
    }
}
