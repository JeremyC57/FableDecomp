// CVtable_012bed7c::ComputeTapGain(SDelayTap* tap, float scale, long delay) -- fill a delay-tap record: the delay, the Q15 gain FloatToLong(scale * volume * 32767) capped (unsigned) at 0x7fff, and its 128-sample ramp step gain >> 7
// Grade: DIFFER(82v66) -- retail_asm candidate: bare `fistp dword` rounding helper (0x00C6CF06, value homed in the dead `scale` slot); ours rounds in C++ + __ftol2; the clamp and the three stores line up byte for byte | 1 grade | next: land as retail_asm
// Leaf: ?ComputeTapGain@CVtable_012bed7c
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6CEF0, 66 bytes (+ int3 padding), `ret 0Ch`.
// Module: libpk's software mixer, the multi-tap delay effect of vtable 0x012BED7C (ctor 0x00C6CA20 landed; tap record as
//   in the matched MixDelayTap 0x00C6CC10). Callers 0x00C6D018 / 0x00C6D037 / 0x00C6D0E7 / 0x00C6D106 fill the taps of the
//   +0x1e0 set from the effect's per-tap scales and delays.
// Retail-asm: 0x00C6CEFE `fstp [esp+0Ch] ; fld [esp+0Ch] ; fistp dword [esp]` -- libpk's inline-assembly rounding helper
//   (the product homed in the dead `scale` slot, the result in the `push ecx` slot).
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
