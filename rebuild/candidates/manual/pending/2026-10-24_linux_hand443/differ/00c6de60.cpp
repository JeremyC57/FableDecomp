// CVtable_012bed90::ComputeGainRamp(SGainRamp* ramp, float scale) -- fill a gain-ramp record: the Q15 gain FloatToLong(scale * volume * 32767) capped (unsigned) at 0x7fff and its 128-sample ramp step gain >> 7
// Grade: DIFFER(75v59) -- retail_asm candidate: bare `fistp dword` rounding helper (0x00C6DE76); ours rounds in C++ + __ftol2; the clamp and stores line up | 1 grade | next: land as retail_asm
// Leaf: ?ComputeGainRamp@CVtable_012bed90
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6DE60, 59 bytes (+ int3 padding), `ret 8`.
// Module: libpk's software mixer, the effect of vtable 0x012BED90 (ctor 0x00C6D9D0 landed; its ramps run through the
//   parked MixGainRampUp / MixGainRampDown 0x00C6DA40 / 0x00C6DAD0). Callers 0x00C6DF4D / 0x00C6DF5F / 0x00C6DFED / 0x00C6DFFF
//   fill the gain records at +8 / +0xc of the +0x5c record from the scales at +0x68 / +0x6c.
// Retail-asm: 0x00C6DE6E `fstp [esp+0Ch] ; fld [esp+0Ch] ; fistp dword [esp]` -- libpk's inline-assembly rounding helper.
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
struct SGainRamp {
    unsigned short m_gain;                         // +0x00 Q15
    short          m_step;                         // +0x02 Q15 per sample
};
struct CVtable_012bed90 : CVtable_012beda4 {
    void ComputeGainRamp(SGainRamp* ramp, float scale);
};
void CVtable_012bed90::ComputeGainRamp(SGainRamp* ramp, float scale)
{
    unsigned long gain = FloatToLong(scale * m_volume * 32767.0f);
    if (gain > 0x7fff)
        gain = 0x7fff;
    ramp->m_gain = (unsigned short)gain;
    ramp->m_step = (short)((unsigned short)gain >> 7);
}
