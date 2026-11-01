// CVtable_012bed7c::Update(float x, float y, float blend, float volume, const float* gains, bool crossfade, bool force) -- vtable slot 4 of libpk's multi-tap delay effect: look up the direction's pair of filter records (0x00C400D0), take their delays into the four delay slots when they changed, compute the stereo (or quad, when the audio master has four or more output channels) panning gains from the azimuth, take the volume, and rebuild the gain records (0x00C6CF40) when anything changed or `force` is set
// Leaf: ?Update@CVtable_012bed7c@@UAEXMMMMPBM_N1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6D150, 511 bytes, __fastcall, 7 stack args (`ret 1Ch`); `blend` and `gains` are not read here.
// Identity (descriptive; libpk has no PDB/RTTI): slot 4 of vtable 0x012BED7C, shared by the derived SSE2 / MMX tables 0x012B8A64 /
//   0x012B8AA0 (ctors 0x00C55070 / 0x00C550B0), the same slot the landed siblings CVtable_012bede0::Update 0x00C71270 and
//   CVtable_012bedcc::Update 0x00C708A0 fill with the same (x, y, blend, volume, gains, crossfade, force) signature and the same
//   0x00C400D0 lookup + angular-gain + conditional-rebuild shape. The object is the one the landed ctor 0x00C6CA20 builds
//   (+0x1e4 six floats, +0x1fc six longs, both cleared there; here the per-channel gains and the filter delays).
// Callees: 0x00C400D0 (cdecl, 5 args: a table selector 4 here where the siblings pass g_pAudioMaster->TapTable, the azimuth,
//   the elevation, two out pointers to 0x34-byte direction-filter records whose word +0x32 is the delay; the landed name
//   Animation_ComputeFrameOffset is a propagated wrong label), CVtable_012bed7c::RebuildGain 0x00C6CF40 (thiscall, bool).
// Shape: a function-scope float gain[6] (the frame is 0x20: two record pointers + six floats; the stereo arm reuses gain[0..1]),
//   sin()/cos() intrinsics on the inline `x * 0.017453292f` (a float `angle` local is rounded through memory and spills), the
//   compiler homes cos*0.4, 0.6-sin*0.4 and cos*0.4+0.6 in the dead x / y / force parameter slots.
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(sin, cos)
#endif
class CUnprovenLibraryObject_012B5868 { public: virtual ~CUnprovenLibraryObject_012B5868() {} };
struct CRITICAL_SECTION { void* DebugInfo; long LockCount, RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
class CUnprovenLock_012B8FC8 { public: virtual ~CUnprovenLock_012B8FC8() {} CRITICAL_SECTION Section; };
class CVtable_012beda4 : public CUnprovenLibraryObject_012B5868 {          // 0x28 bytes (ctor 0x00C6E6A0 landed)
public:
    virtual ~CVtable_012beda4() {}
    virtual void Clear() {}
    virtual void Process(const short*, long* const*) {}
    virtual void SetVolume(float, unsigned long, bool) = 0;
    virtual void Update(float, float, float, float, const float*, bool, bool) {}
    CUnprovenLock_012B8FC8 Lock;                                            // +0x04
    float m_volume;                                                         // +0x20
    bool  m_crossfade;                                                      // +0x24
};
struct CAudioMaster { unsigned char _pad[0x1bc]; unsigned long m_outputChannels; };
extern CAudioMaster* g_pAudioMaster;                                        // 0x0143BC2C
struct SDirectionFilter { short m_coefficients[25]; unsigned short m_delay; };   // 0x34-byte record of the direction table (delay word at +0x32)
void LookupDirectionFilterPair(long table, float azimuth, float elevation, const SDirectionFilter** first, const SDirectionFilter** second);   // 0x00C400D0 (landed callers call it Animation_ComputeFrameOffset: a propagated wrong label)
struct SRecord_00c6ca20 { long m_words[12]; };
class CVtable_012bed7c : public CVtable_012beda4 {                         // vtable 0x012BED7C (ctor 0x00C6CA20 landed)
public:
    unsigned char     m_state[0x40];                                        // +0x28
    unsigned char     m_buffer[0x110];                                      // +0x68
    unsigned char*    m_pAlignedBuffer;                                     // +0x178
    SRecord_00c6ca20  m_first;                                              // +0x17c
    SRecord_00c6ca20  m_second;                                             // +0x1ac
    SRecord_00c6ca20* m_pCurrent;                                           // +0x1dc
    SRecord_00c6ca20* m_pPrevious;                                          // +0x1e0
    float             m_gains[6];                                           // +0x1e4 (the ctor model's m_valuesA)
    long              m_delays[6];                                          // +0x1fc (the ctor model's m_valuesB)
    void RebuildGain(bool crossfade);                                       // 0x00C6CF40
    virtual void Update(float x, float y, float blend, float volume, const float* gains, bool crossfade, bool force);
};
void CVtable_012bed7c::Update(float x, float y, float blend, float volume, const float* gains, bool crossfade, bool force)
{
    const SDirectionFilter* first;
    const SDirectionFilter* second;
    bool changed = false;
    float gain[6];
    LookupDirectionFilterPair(4, x, y, &first, &second);
    if (force || m_delays[0] != first->m_delay || m_delays[1] != second->m_delay) {
        m_delays[0] = first->m_delay;
        m_delays[2] = first->m_delay;
        m_delays[1] = second->m_delay;
        m_delays[3] = second->m_delay;
        changed = true;
    }
    if (g_pAudioMaster->m_outputChannels >= 4) {
        float s = (float)sin(x * 0.017453292f) * 0.4f;
        float c = (float)cos(x * 0.017453292f) * 0.4f;
        gain[0] = (c + 0.6f) * (0.6f - s);
        gain[1] = (c + 0.6f) * (s + 0.6f);
        gain[2] = (0.6f - s) * (0.6f - c);
        gain[3] = (s + 0.6f) * (0.6f - c);
        if (force || m_gains[0] != gain[0] || m_gains[1] != gain[1] || m_gains[2] != gain[2] || m_gains[3] != gain[3]) {
            m_gains[0] = gain[0];
            m_gains[1] = gain[1];
            m_gains[2] = gain[2];
            m_gains[3] = gain[3];
            changed = true;
        }
    } else {
        float s = (float)sin(x * 0.017453292f) * 0.4f;
        gain[0] = 0.6f - s;
        gain[1] = s + 0.6f;
        if (force || m_gains[0] != gain[0] || m_gains[1] != gain[1]) {
            m_gains[0] = gain[0];
            m_gains[1] = gain[1];
            changed = true;
        }
    }
    if (force || m_volume != volume) {
        m_volume = volume;
        changed = true;
    }
    if (changed)
        RebuildGain(crossfade);
}