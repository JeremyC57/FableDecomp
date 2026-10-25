// CVtable_012bedb8::ComputeGainRamp(SGainRamp* ramp) -- fill a gain-ramp record from the effect volume alone: gain = min(FloatToLong(volume * 32767), 0x7fff) >> 1 (unsigned) and its 128-sample ramp step gain >> 7
// Grade: DIFFER(74v62) -- retail_asm candidate: bare `fistp dword` rounding helper (0x00C6EBB2); ours rounds in C++ + __ftol2 | 1 grade | next: land as retail_asm
// Leaf: ?ComputeGainRamp@CVtable_012bedb8
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6EBA0, 62 bytes (+ int3 padding), `ret 4`.
// Module: libpk's software mixer, the gain-ramp effect of vtable 0x012BEDB8 (ctor 0x00C6E760 matched in this chunk). No
//   direct callers (address-taken).
// Retail-asm: 0x00C6EBAC `fstp [esp] ; fld [esp] ; fistp dword [esp+4]` -- libpk's inline-assembly rounding helper.
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
struct CVtable_012bedb8 : CVtable_012beda4 {
    void ComputeGainRamp(SGainRamp* ramp);
};
void CVtable_012bedb8::ComputeGainRamp(SGainRamp* ramp)
{
    unsigned long level = FloatToLong(m_volume * 32767.0f);
    if (level > 0x7fff)
        level = 0x7fff;
    unsigned short gain = (unsigned short)((unsigned short)level >> 1);
    ramp->m_gain = gain;
    ramp->m_step = (short)(gain >> 7);
}
