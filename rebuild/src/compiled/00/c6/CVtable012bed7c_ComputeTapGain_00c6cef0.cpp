// CVtable_012bed7c::ComputeTapGain(SDelayTap* tap, float scale, long delay) -- fill a delay-tap record: the delay, the Q15 gain FloatToLong(scale * volume * 32767) capped (unsigned) at 0x7fff, and its 128-sample ramp step gain >> 7
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Leaf: ?ComputeTapGain@CVtable_012bed7c
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6CEF0, 66 bytes (+ int3 padding), `ret 0Ch`.
// Module: libpk's software mixer, the multi-tap delay effect of vtable 0x012BED7C (ctor 0x00C6CA20 landed; tap record as
//   in the matched MixDelayTap 0x00C6CC10). Callers 0x00C6D018 / 0x00C6D037 / 0x00C6D0E7 / 0x00C6D106 fill the taps of the
//   +0x1e0 set from the effect's per-tap scales and delays.
// Retail-asm: 0x00C6CEFE `fstp [esp+0Ch] ; fld [esp+0Ch] ; fistp dword [esp]` -- libpk's inline-assembly rounding helper (the product homed in the dead `scale` slot, the result in the `push ecx` slot). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// libpk's inline-assembly float-to-long helper. Retail homes the value in a dead stack slot and rounds it with a bare
// `fld [slot] ; fistp dword [tmp]` (round to nearest, the FPU default); VC7.1 expands the __forceinline helper in place.
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
struct SCriticalSection { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
struct CVtable_012beda4 {                          // vtable 0x012BEDA4, 0x28 bytes (ctor 0x00C6E6A0, dtor 0x00C6E630 landed)
    void*            _vtable;                      // +0x00
    void*            _lockVtable;                  // +0x04 critical-section owner (vtable 0x012B8FC8)
    SCriticalSection m_lock;                       // +0x08
    float            m_volume;                     // +0x20 linear (the landed dtor model calls it `long m_count`)
    bool             m_alternate;                  // +0x24
};
struct SDelayTap {
    long           m_delay;                        // +0x00
    unsigned short m_gain;                         // +0x04 Q15
    short          m_step;                         // +0x06 Q15 per sample
};
struct CVtable_012bed7c : CVtable_012beda4 {
    void ComputeTapGain(SDelayTap* tap, float scale, long delay);
};
void CVtable_012bed7c::ComputeTapGain(SDelayTap* tap, float scale, long delay)
{
    unsigned long gain = FloatToLong(scale * m_volume * 32767.0f);
    if (gain > 0x7fff)
        gain = 0x7fff;
    tap->m_gain = (unsigned short)gain;
    tap->m_delay = delay;
    tap->m_step = (short)((unsigned short)gain >> 7);
}