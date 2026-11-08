// CVtable_012bed7c::RebuildGain(bool crossfade) -- under the multi-tap delay effect's lock record which tap set is live and rebuild its Q15 tap records from the per-tap gains, the delays and the volume: taps 0 and 1 always, taps 2 and 3 only when the audio master runs four or more output channels
// Leaf: ?RebuildGain@CVtable_012bed7c@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6CF40, 470 bytes, thiscall `ret 4`, one dword frame word (the FtoL result).
// Identity (descriptive; libpk software mixer, no PDB / RTTI): the landed binding `?RebuildGain@CVtable_012bed7c@@QAEX_N@Z`
//   (callers: slot 3 SetVolume 0x00C6D120 and slot 4 Update 0x00C6D150, both landed, which name it RebuildGain); the
//   twin of the landed CVtable_012bed90::RebuildGain 0x00C6DEA0 (same lock / flag / two-pair shape over gain ramps). The
//   catalog label "Audio_SetFilterCoefficients" is a propagated descriptive guess.
// Callees: EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10) on the effect lock (+0x08),
//   ComputeTapGain 0x00C6CEF0 (taps 2 and 3, out of line), g_pAudioMaster 0x0143BC2C (+0x1BC output channels).
// Retail-asm: 0x00C6CF7D `fld dword ptr [esp+10h]` + 0x00C6CF81 `fistp dword ptr [esp+8]` (and 0x00C6CFC5 / 0x00C6CFC9,
//   0x00C6D04F / 0x00C6D053, 0x00C6D097 / 0x00C6D09B) -- libpk's inline-assembly float-to-long helper, the product homed in
//   the dead `crossfade` slot and reloaded, a bare dword fistp (no VC7.1 C++ form: `(long)x` calls __ftol2, /QIfist gives
//   `fistp qword`); reached only through the shared libpk/FtoL.h.
// Layout: the landed Update 0x00C6D150 model: the live tap set +0x1DC (crossfade false) / the cross-fade target +0x1E0
//   (true), the per-tap gains +0x1E4, the tap delays +0x1FC; the base's lock +4 (CRITICAL_SECTION +8), volume +0x20,
//   cross-fade flag +0x24.
// Shape: as the twin 0x00C6DEA0: the front pair is the inline tap step (the landed ComputeTapGain body as an inline
//   helper); the rear pair calls the out-of-line ComputeTapGain, as retail does.
#include "libpk/FtoL.h"
struct CRITICAL_SECTION { void* DebugInfo; long LockCount, RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE10
class CUnprovenAudioMasterBase_012B5B80 { public: virtual void v00(); unsigned char UnprovenBase04[0x50]; };
class CAudioMaster : public CUnprovenAudioMasterBase_012B5B80 {
public:
    unsigned char UnprovenFields54[356];
    void* TapTable;                                                         // +0x1b8
    unsigned long m_outputChannels;                                         // +0x1bc
    unsigned char UnprovenTail1C0[24];
};
extern CAudioMaster* g_pAudioMaster;                                        // 0x0143BC2C
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
struct SDelayTap {                        // 8 bytes (landed MixDelayTap 0x00C6CC10 model)
    long           m_delay;               // +0x00 samples back into the history
    unsigned short m_gain;                // +0x04 Q15 gain
    short          m_step;                // +0x06 Q15 gain step per sample (ramps)
};
class CVtable_012bed7c : public CVtable_012beda4 {                         // vtable 0x012BED7C (ctor 0x00C6CA20 landed)
public:
    unsigned char     m_state[0x40];                                        // +0x28
    unsigned char     m_buffer[0x110];                                      // +0x68
    unsigned char*    m_pAlignedBuffer;                                     // +0x178
    SDelayTap         m_first[6];                                           // +0x17c
    SDelayTap         m_second[6];                                          // +0x1ac
    SDelayTap*        m_pCurrent;                                           // +0x1dc
    SDelayTap*        m_pPrevious;                                          // +0x1e0
    float             m_gains[6];                                           // +0x1e4
    long              m_delays[6];                                          // +0x1fc
    void ComputeTapGain(SDelayTap* tap, float scale, long delay);           // 0x00C6CEF0
    void SetTapGain(SDelayTap* tap, float scale, long delay)
    {
        unsigned long gain = FtoL(scale * m_volume * 32767.0f);
        if (gain > 0x7fff)
            gain = 0x7fff;
        tap->m_gain = (unsigned short)gain;
        tap->m_delay = delay;
        tap->m_step = (short)((unsigned short)gain >> 7);
    }
    void RebuildGain(bool crossfade);
};

void CVtable_012bed7c::RebuildGain(bool crossfade)
{
    EnterCriticalSection(&Lock.Section);
    m_crossfade = crossfade;
    if (crossfade) {
        SetTapGain(&m_pPrevious[0], m_gains[0], m_delays[0]);
        SetTapGain(&m_pPrevious[1], m_gains[1], m_delays[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            ComputeTapGain(&m_pPrevious[2], m_gains[2], m_delays[2]);
            ComputeTapGain(&m_pPrevious[3], m_gains[3], m_delays[3]);
        }
    } else {
        SetTapGain(&m_pCurrent[0], m_gains[0], m_delays[0]);
        SetTapGain(&m_pCurrent[1], m_gains[1], m_delays[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            ComputeTapGain(&m_pCurrent[2], m_gains[2], m_delays[2]);
            ComputeTapGain(&m_pCurrent[3], m_gains[3], m_delays[3]);
        }
    }
    LeaveCriticalSection(&Lock.Section);
}