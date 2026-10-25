// CVtable_012bedb8::SetAlternate(bool alternate) -- under the effect lock: store the flag and write the half-volume Q15 gain (min(FloatToLong(volume * 32767), 0x7fff) >> 1) and its ramp step into the previous (+0x34, flag set) or current (+0x30) gain record
// Grade: DIFFER(123v106) -- retail_asm candidate: bare `fistp dword` rounding helper expanded in both arms (0x00C6EC0E / 0x00C6EC1B, the product homed in the dead `alternate` slot); ours rounds in C++ + __ftol2; the lock calls, flag store and record stores line up | 1 grade | next: land as retail_asm
// Leaf: ?SetAlternate@CVtable_012bedb8
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6EBE0, 106 bytes (+ int3 padding), `ret 4`.
// Module: libpk's software mixer, the gain-ramp effect of vtable 0x012BEDB8 (ctor 0x00C6E760 matched in this chunk).
//   Callers: slots 3 / 4 of the vtable (0x00C6EC50 / 0x00C6EC80, call sites 0x00C6EC6C / 0x00C6ECA4).
// Callees: KERNEL32 EnterCriticalSection / LeaveCriticalSection through the IAT slots 0x0143FE0C / 0x0143FE10 (on +0x08).
// Retail-asm: 0x00C6EC0A / 0x00C6EC17 `fld [esp+10h] ; fistp dword [esp+8]` (one per arm; the product was homed in the dead
//   `alternate` slot before the test) -- libpk's inline-assembly rounding helper expanded in both arms of the record choice.
// libpk's inline-assembly float-to-long helper. Retail homes the value in a dead stack slot and rounds it with a bare
// `fld [slot] ; fistp dword [tmp]` (round to nearest, the FPU default). VC7.1 C++ cannot emit that (`(long)x` calls
// __ftol2 and /QIfist emits `fistp qword`), so the body below is a C++ stand-in; at landing it becomes
// the minimal inline-assembly `fld value ; fistp result` body (grade retail_asm, like the landed ROUND of 0x00BE6790).
__forceinline long FloatToLong(float value)
{
    return (long)(value < 0.0f ? value - 0.5f : value + 0.5f);     // stand-in: round to nearest (fistp: ties to even)
}
struct SCriticalSection { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
struct CVtable_012beda4 {                          // vtable 0x012BEDA4, 0x28 bytes (ctor 0x00C6E6A0, dtor 0x00C6E630 landed)
    void*            _vtable;                      // +0x00
    void*            _lockVtable;                  // +0x04 critical-section owner (vtable 0x012B8FC8)
    SCriticalSection m_lock;                       // +0x08
    float            m_volume;                     // +0x20 linear (the landed dtor model calls it `long m_count`)
    bool             m_alternate;                  // +0x24
};
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(SCriticalSection* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(SCriticalSection* section);
struct SGainRamp {
    unsigned short m_gain;                         // +0x00 Q15
    short          m_step;                         // +0x02 Q15 per sample
};
__forceinline void StoreHalfGain(SGainRamp* ramp, float level)
{
    unsigned long value = FloatToLong(level);
    if (value > 0x7fff)
        value = 0x7fff;
    unsigned short gain = (unsigned short)((unsigned short)value >> 1);
    ramp->m_gain = gain;
    ramp->m_step = (short)(gain >> 7);
}
struct CVtable_012bedb8 : CVtable_012beda4 {
    SGainRamp  m_first;                            // +0x28
    SGainRamp  m_second;                           // +0x2c
    SGainRamp* m_pCurrent;                         // +0x30
    SGainRamp* m_pPrevious;                        // +0x34
    void SetAlternate(bool alternate);
};
void CVtable_012bedb8::SetAlternate(bool alternate)
{
    EnterCriticalSection(&m_lock);
    m_alternate = alternate;
    float level = m_volume * 32767.0f;
    if (alternate)
        StoreHalfGain(m_pPrevious, level);
    else
        StoreHalfGain(m_pCurrent, level);
    LeaveCriticalSection(&m_lock);
}
