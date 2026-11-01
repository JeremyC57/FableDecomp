// CVtable_012bed90::Process(const short* input, long* const* outputs) -- vtable slot 2 of libpk's 4-channel gain mixer, the plain C path: under the effect lock either cross-fade (ramp the current gains down, swap the current/previous gain-ramp arrays, ramp the new gains up) or mix the block at constant gain into outputs 0-1 (and 2-3 with four or more output channels)
// Grade: DIFFER(689v689) -- register allocation only: LeaveCriticalSection's argument register (edx v retail eax) and the gain-record pointer in the constant arm (ecx v retail eax/esi; retail keeps a dead `add eax,4` / `add eax,8` / `add esi,0xc`)
// Leaf: ?Process@CVtable_012bed90@@UAEXPBFPBQAJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6DBA0, 689 bytes, thiscall `ret 8`; slot 2 of vtable 0x012BED90 (only reference .rdata
//   0x012BED98).
// Name: descriptive (libpk: no PDB / RTTI), the landed family convention CVtable_<vtable>::Process.
// Callees: Enter/LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10) on the lock +0x08; same-TU gain mixers MixRampUp 0x00C6DA40,
//   MixRampDown 0x00C6DAD0 (called), Mix 0x00C6DB60 (cdecl (source, count, ramp, accumulator); out-of-line copy with no callers,
//   inlined at all four constant-arm sites); g_pAudioMaster 0x0143BC2C (m_outputChannels >= 4 mixes channels 2 and 3).
// Next: as 0x00C6E8F0 -- make the inlined Mix keep its ramp pointer live (gain read through the pointer inside the loop, or
//   an SGainRamp member Mix); 14 shape probes (lock guard, section local, indexing, ramps local, /G7) left 18-19 lines.
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
struct SGainRamp {
    unsigned short m_gain;
    short          m_step;
};
class CVtable_012bed90 : public CVtable_012beda4 {
public:
    virtual void Process(const short* input, long* const* outputs);
    SGainRamp  m_first[6];
    SGainRamp  m_second[6];
    SGainRamp* m_pCurrent;
    SGainRamp* m_pPrevious;
    float      m_scales[6];
    static void MixRampUp(const short* source, const SGainRamp* ramp, long* accumulator);
    static void MixRampDown(const short* source, const SGainRamp* ramp, long* accumulator);
    static void Mix(const short* source, unsigned long count, const SGainRamp* ramp, long* accumulator);
};
void CVtable_012bed90::MixRampUp(const short* source, const SGainRamp* ramp, long* accumulator)
{
    short step = ramp->m_step;
    if (step == 0)
        return;
    const short* samples = source;
    long gain = 0;
    for (int i = 0; i < 128; ++i) {
        accumulator[i] += (*samples++ * gain) >> 15;
        gain += step;
    }
}
void CVtable_012bed90::MixRampDown(const short* source, const SGainRamp* ramp, long* accumulator)
{
    unsigned short start = ramp->m_gain;
    if (start == 0)
        return;
    short step = -ramp->m_step;
    const short* samples = source;
    long gain = start;
    for (int i = 0; i < 128; ++i) {
        accumulator[i] += (*samples++ * gain) >> 15;
        gain += step;
    }
}
void CVtable_012bed90::Mix(const short* source, unsigned long count, const SGainRamp* ramp, long* accumulator)
{
    unsigned short gain = ramp->m_gain;
    if (gain == 0)
        return;
    long* out = accumulator;
    const short* samples = source;
    for (unsigned long i = 0; i < count; ++i)
        *out++ += (*samples++ * gain) >> 15;
}
void CVtable_012bed90::Process(const short* input, long* const* outputs)
{
    CRITICAL_SECTION* section = &Lock.Section;
    EnterCriticalSection(section);
    if (m_crossfade) {
        m_crossfade = false;
        MixRampDown(input, &m_pCurrent[0], outputs[0]);
        MixRampDown(input, &m_pCurrent[1], outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixRampDown(input, &m_pCurrent[2], outputs[2]);
            MixRampDown(input, &m_pCurrent[3], outputs[3]);
        }
        SGainRamp* current = m_pCurrent;
        m_pCurrent = m_pPrevious;
        m_pPrevious = current;
        MixRampUp(input, &m_pCurrent[0], outputs[0]);
        MixRampUp(input, &m_pCurrent[1], outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixRampUp(input, &m_pCurrent[2], outputs[2]);
            MixRampUp(input, &m_pCurrent[3], outputs[3]);
        }
        LeaveCriticalSection(section);
        return;
    }
    {
        Mix(input, 128, &m_pCurrent[0], outputs[0]);
        Mix(input, 128, &m_pCurrent[1], outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            Mix(input, 128, &m_pCurrent[2], outputs[2]);
            Mix(input, 128, &m_pCurrent[3], outputs[3]);
        }
    }
    LeaveCriticalSection(section);
}
