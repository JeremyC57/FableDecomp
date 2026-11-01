// CVtable_012bed7c::Process(const short* input, long* const* outputs) -- vtable slot 2 of libpk's multi-tap delay effect, the plain C path: copy the 128-sample block into the aligned history, then under the effect lock either cross-fade (ramp the old taps down, swap the tap sets, ramp the new ones up) or mix each tap at its constant gain; finally slide the last 32 samples below the buffer for the next block
// Leaf: ?Process@CVtable_012bed7c@@UAEXPBFPBQAJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6CC60, 648 bytes, thiscall `ret 8`; slot 2 of vtable 0x012BED7C (the only reference:
//   .rdata 0x012BED84). Same slot in the derived tables: SSE2 0x012B8A64 -> 0x00C6D630 (landed retail_asm, same body over the
//   SSE2 kernels), MMX 0x012B8AA0 -> 0x00C6F3E0.
// Name: descriptive (libpk: no PDB / RTTI); class and method names as the landed family (ctor 0x00C6CA20, ClearHistory
//   0x00C6CAD0, Update 0x00C6D150, the SSE2 Process 0x00C6D630, which names this row as "the C path of the same slot").
// Callees: EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10) on the effect lock (+0x08),
//   MixDelayTapRampDown 0x00C6CB80, MixDelayTapRampUp 0x00C6CAF0, MixDelayTap 0x00C6CC10 (all landed, same TU, cdecl);
//   g_pAudioMaster 0x0143BC2C (four or more output channels mix taps 2 and 3).
// Shape: the three landed tap mixers are written above Process with their landed bodies; VC7.1 keeps the ramps out of line and
//   expands MixDelayTap at the first two sites only (its inline budget), exactly as retail; the block copies are the memcpy
//   intrinsic (`rep movsd`, 0x40 and 0x10 dwords).
#include <string.h>
#pragma intrinsic(memcpy)
struct CRITICAL_SECTION { void* DebugInfo; long LockCount, RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE10
class CUnprovenLibraryObject_012B5868 { public: virtual ~CUnprovenLibraryObject_012B5868() {} };
class CUnprovenLock_012B8FC8 { public: virtual ~CUnprovenLock_012B8FC8() {} CRITICAL_SECTION Section; };
class CVtable_012beda4 : public CUnprovenLibraryObject_012B5868 {
public:
    virtual ~CVtable_012beda4() {}
    virtual void Clear() {}
    virtual void Process(const short*, long* const*) {}
    virtual void SetVolume(float, unsigned long, bool) = 0;
    virtual void Update(float, float, float, float, const float*, bool, bool) {}
    CUnprovenLock_012B8FC8 Lock;
    float m_volume;
    bool  m_crossfade;
};
struct CAudioMaster { unsigned char _pad[0x1bc]; unsigned long m_outputChannels; };
extern CAudioMaster* g_pAudioMaster;                                        // 0x0143BC2C
struct SDelayTap {
    long           m_delay;
    unsigned short m_gain;
    short          m_step;
};
struct SRecord_00c6ca20 { SDelayTap m_taps[6]; };
class CVtable_012bed7c : public CVtable_012beda4 {
public:
    virtual void Process(const short* input, long* const* outputs);
    unsigned char     m_state[0x40];
    unsigned char     m_buffer[0x110];
    unsigned char*    m_pAlignedBuffer;
    SRecord_00c6ca20  m_first;
    SRecord_00c6ca20  m_second;
    SRecord_00c6ca20* m_pCurrent;
    SRecord_00c6ca20* m_pPrevious;
    float             m_gains[6];
    long              m_delays[6];
};
void MixDelayTapRampUp(const short* source, const SDelayTap* tap, long* accumulator)
{
    short step = tap->m_step;
    if (step == 0)
        return;
    const short* samples = source - tap->m_delay;
    long gain = 0;
    for (int i = 0; i < 128; ++i) {
        accumulator[i] += (*samples++ * gain) >> 15;
        gain += step;
    }
}
void MixDelayTapRampDown(const short* source, const SDelayTap* tap, long* accumulator)
{
    unsigned short start = tap->m_gain;
    if (start == 0)
        return;
    short step = -tap->m_step;
    const short* samples = source - tap->m_delay;
    long gain = start;
    for (int i = 0; i < 128; ++i) {
        accumulator[i] += (*samples++ * gain) >> 15;
        gain += step;
    }
}
void MixDelayTap(const short* source, unsigned long count, const SDelayTap* tap, long* accumulator)
{
    unsigned short gain = tap->m_gain;
    if (gain == 0)
        return;
    long* out = accumulator;
    const short* samples = source - tap->m_delay;
    for (unsigned long i = 0; i < count; ++i)
        *out++ += (*samples++ * gain) >> 15;
}

void CVtable_012bed7c::Process(const short* input, long* const* outputs)
{
    memcpy(m_pAlignedBuffer, input, 128 * sizeof(short));
    EnterCriticalSection(&Lock.Section);
    if (m_crossfade) {
        m_crossfade = false;
        MixDelayTapRampDown((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[0], outputs[0]);
        MixDelayTapRampDown((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[1], outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixDelayTapRampDown((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[2], outputs[2]);
            MixDelayTapRampDown((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[3], outputs[3]);
        }
        SRecord_00c6ca20* current = m_pCurrent;
        m_pCurrent = m_pPrevious;
        m_pPrevious = current;
        MixDelayTapRampUp((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[0], outputs[0]);
        MixDelayTapRampUp((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[1], outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixDelayTapRampUp((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[2], outputs[2]);
            MixDelayTapRampUp((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[3], outputs[3]);
        }
    } else {
        MixDelayTap((const short*)m_pAlignedBuffer, 128, &m_pCurrent->m_taps[0], outputs[0]);
        MixDelayTap((const short*)m_pAlignedBuffer, 128, &m_pCurrent->m_taps[1], outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixDelayTap((const short*)m_pAlignedBuffer, 128, &m_pCurrent->m_taps[2], outputs[2]);
            MixDelayTap((const short*)m_pAlignedBuffer, 128, &m_pCurrent->m_taps[3], outputs[3]);
        }
    }
    LeaveCriticalSection(&Lock.Section);
    memcpy(m_pAlignedBuffer - 0x40, m_pAlignedBuffer + 0xc0, 0x40);
}