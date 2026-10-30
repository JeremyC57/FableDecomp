// CVtable_012bed90::RebuildGain(bool alternate) -- under the effect's lock record the selected gain set and rebuild its Q15 gain ramps from the channel scales and the volume: the front pair always, the rear pair only when the audio master runs four or more output channels
// Leaf: ?RebuildGain@CVtable_012bed90@@QAEX_N@Z
// Retail-asm: 0x00C6DED1 `fld dword ptr [esp+10h]` + 0x00C6DED5 `fistp dword ptr [esp+8]` (and 0x00C6DF0A / 0x00C6DF0E, 0x00C6DF74 / 0x00C6DF78, 0x00C6DFAD / 0x00C6DFB1) -- libpk's inline-assembly float-to-long helper (the landed ComputeGainRamp 0x00C6DE60 runs the same `fld ; fistp dword` block): the product homed in the dead argument slot and reloaded, a bare dword fistp. VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword` into an 8-byte slot: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6DEA0, 369 bytes, thiscall `ret 4`, one dword frame word.
// Identity (descriptive; libpk software mixer, no PDB / RTTI): the landed binding `?RebuildGain@CVtable_012bed90@@QAEX_N@Z`
//   (caller SetVolume 0x00C6E020, slot 3); the other callers are slot 4 SetParameters 0x00C6E050 (this chunk). Layout as
//   the landed ctor 0x00C6D9D0: two 6-word gain records +0x28 / +0x40 reached through +0x58 (alternate false) / +0x5C
//   (true), the four channel scales +0x60..+0x6C; the base's lock +8, volume +0x20, flag +0x24 as the landed
//   ComputeGainRamp model. The catalog label Audio_SetOutputVolume is descriptive.
// Callees: EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10); ComputeGainRamp 0x00C6DE60 (the
//   rear pair); g_pAudioMaster 0x0143BC2C (`class CAudioMaster`, the spelling of the landed mixer rows) +0x1BC output
//   channels.
// Shape: the front pair is the inline gain step (the landed ComputeGainRamp body as an inline helper); the rear pair calls
//   the out-of-line ComputeGainRamp, as retail does.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);   // IAT 0x0143FE10

// libpk's inline-assembly float-to-long helper (round to nearest, the FPU default)
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

class CUnprovenAudioMasterBase_012B5B80 { public: virtual void v00(); unsigned char UnprovenBase04[0x50]; };
class CAudioMaster : public CUnprovenAudioMasterBase_012B5B80 {
public:
    unsigned char UnprovenFields54[356];
    void* TapTable;                                // +0x1b8
    unsigned long m_outputChannels;                // +0x1bc
    unsigned char UnprovenTail1C0[24];
};
extern CAudioMaster* g_pAudioMaster;               // 0x0143BC2C

struct SCriticalSection { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
struct CVtable_012beda4 {                          // vtable 0x012BEDA4, 0x28 bytes (ctor 0x00C6E6A0, dtor 0x00C6E630 landed)
    void*            _vtable;                      // +0x00
    void*            _lockVtable;                  // +0x04 critical-section owner (vtable 0x012B8FC8)
    SCriticalSection m_lock;                       // +0x08
    float            m_volume;                     // +0x20 linear
    bool             m_alternate;                  // +0x24
};
struct SGainRamp {
    unsigned short m_gain;                         // +0x00 Q15
    short          m_step;                         // +0x02 Q15 per sample
};
struct CVtable_012bed90 : CVtable_012beda4 {       // vtable 0x012BED90 (ctor 0x00C6D9D0)
    SGainRamp  m_first[6];                         // +0x28
    SGainRamp  m_second[6];                        // +0x40
    SGainRamp* m_pCurrent;                         // +0x58
    SGainRamp* m_pPrevious;                        // +0x5c
    float      m_scales[6];                        // +0x60 front left/right, rear left/right
    void ComputeGainRamp(SGainRamp* ramp, float scale);   // 0x00C6DE60
    void SetGainRamp(SGainRamp* ramp, float scale)
    {
        unsigned long gain = FloatToLong(scale * m_volume * 32767.0f);
        if (gain > 0x7fff)
            gain = 0x7fff;
        ramp->m_gain = (unsigned short)gain;
        ramp->m_step = (short)((unsigned short)gain >> 7);
    }
    void RebuildGain(bool alternate);
};

void CVtable_012bed90::RebuildGain(bool alternate)
{
    EnterCriticalSection(&m_lock);
    m_alternate = alternate;
    if (alternate) {
        SetGainRamp(&m_pPrevious[0], m_scales[0]);
        SetGainRamp(&m_pPrevious[1], m_scales[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            ComputeGainRamp(&m_pPrevious[2], m_scales[2]);
            ComputeGainRamp(&m_pPrevious[3], m_scales[3]);
        }
    } else {
        SetGainRamp(&m_pCurrent[0], m_scales[0]);
        SetGainRamp(&m_pCurrent[1], m_scales[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            ComputeGainRamp(&m_pCurrent[2], m_scales[2]);
            ComputeGainRamp(&m_pCurrent[3], m_scales[3]);
        }
    }
    LeaveCriticalSection(&m_lock);
}