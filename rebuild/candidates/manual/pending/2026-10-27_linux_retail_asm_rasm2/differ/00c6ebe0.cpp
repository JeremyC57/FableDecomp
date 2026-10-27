// CVtable_012bedb8::SetAlternate(bool alternate) -- libpk mixer channel: under the channel lock, record the alternate flag and load the selected gain ramp (previous when alternate, current otherwise) with half the Q15 volume and its 1/128 step
// Leaf: ?SetAlternate@CVtable_012bedb8
// Retail-asm: 0x00C6EC0A / 0x00C6EC17 `fld dword ptr [esp+10h]` ; 0x00C6EC0E / 0x00C6EC1B `fistp dword ptr [esp+8]` -- libpk's inline-assembly float-to-long helper (rasm1 family (a)) expanded in both arms: the Q15 product is stored once to the dead `alternate` argument slot (the helper's by-value parameter) and each arm reloads it for a bare dword fistp into a frame word; VC7.1 C++ `(long)x` calls __ftol2, /QIfist emits `fistp qword` and /arch:SSE2 cvttss2si (rasm1 probe family (a)). The lock calls, the flag store, the clamp and the ramp stores are C++.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6EBE0, 106 bytes (+6 int3), __thiscall, `ret 4`. Imports: EnterCriticalSection
//   (0x0143FE0C), LeaveCriticalSection (0x0143FE10). Constant 32767.0f @ 0x0129C820. The object (vtable 0x012BEDB8, derived from
//   the lockable base with vtable 0x012BEDA4 whose ctor/dtor are landed) keeps a linear volume at +0x20, the flag at +0x24, two
//   gain ramps at +0x28/+0x2C and the current/previous ramp pointers at +0x30/+0x34. The catalog label "Audio_SetVolume_Channel"
//   is descriptive.
// libpk's inline-assembly float-to-long helper (VC7.1 expands the __forceinline body and its __asm in place)
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
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(SCriticalSection* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(SCriticalSection* section);
struct SGainRamp {
    unsigned short m_gain;                         // +0x00 Q15
    short          m_step;                         // +0x02 Q15 per sample
};
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
    SGainRamp*    ramp;
    unsigned long value;
    if (alternate) {
        ramp = m_pPrevious;
        value = FloatToLong(level);
    } else {
        ramp = m_pCurrent;
        value = FloatToLong(level);
    }
    if (value > 0x7fff)
        value = 0x7fff;
    unsigned short gain = (unsigned short)((unsigned short)value >> 1);
    ramp->m_gain = gain;
    ramp->m_step = (short)(gain >> 7);
    LeaveCriticalSection(&m_lock);
}
