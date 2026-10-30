// CReverbProcessor::Allocate(const CReverbConfig* config, float* memory, unsigned int* pBytes) -- two-pass memory layout of the libpk software reverb: restart the line index, size (and with memory, carve) the processor's own work block, reset both gains, then lay out the four comb filters (a fifth of a second plus 128 samples, multiple of 4), the tap bank, the diffuser, the feedback delay, the pre-delay and the stereo delay pair from one cursor, and return the total bytes
// Leaf: ?Allocate@CReverbProcessor@@QAEXPBUCReverbConfig@@PAMPAI@Z
// Retail-asm: 0x00C6A4CE `fld dword ptr [esp+1Ch]` + 0x00C6A4D2 `fistp dword ptr [esp+14h]` -- libpk's inline-assembly float-to-long helper inside its round-half-down (as 0x00C3FF70 and the landed CReverbProcessor::UpdateChorusLength 0x00C77FC0: the `rate * 0.2f - 0.5f` stored to a dead frame slot and reloaded, a bare dword fistp, then the C++ `+1 if negative`). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword` into an 8-byte slot: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6A460, 364 bytes, thiscall `ret 0Ch`, 8-byte frame (the running byte total and
//   the comb length / loop counter slots). Constants 2^32 (0x0122DCB4, unsigned rate fix-up), 0.2f (0x012305A0),
//   0.5f (0x0122F59C).
// Identity (class proven, method name descriptive; libpk, no PDB / RTTI): `this` is the CReverbProcessor the landed
//   ctor 0x00C6A650 / dtor 0x00C6A780 build (CDspGain +0x6C / +0x80, CReverbTapBank +0x94, CReverbDiffuser +0x144,
//   CReverbPreDelay +0x1E0, four 0x14-byte CCombFilter +0x1F8, CReverbDelay +0x248, CStereoDelayLine +0x25C). Its only
//   caller is the processor's configure step 0x00C6A8B0, which calls it twice (memory 0 to size, then the 16-aligned
//   block) before setting the ready byte +0x294 -- the same two-pass protocol as every callee `Allocate`. The catalog
//   label CQ_GuildTrainingSkillScript_Init is propagated; the landed caller's `BufferState::Build(const Configuration*,
//   void*, unsigned long*)` spelling is a placeholder (NAME_FIXES.tsv).
// Callees: CReverbProcessor::AllocateMemory 0x00C69A70 (parked hand342 as CReverbProcessor_00c6a460::AllocateMemory),
//   CDspGain::Reset 0x00C744E0 (landed), CCombFilter::Allocate 0x00C75F80 (parked hand444 as CReverbTap::Allocate),
//   CReverbTapBank::Allocate 0x00C75C30 (landed), CReverbDiffuser::Allocate 0x00C755B0, CReverbDelay::Allocate
//   0x00C76AB0, CReverbPreDelay::Allocate 0x00C76DD0, CStereoDelayLine::Allocate 0x00C75050 (all parked hand444, same
//   signatures as declared here).
// Config: +4 channel count, +8 sample rate, +0x1C the tap lengths (the fields this body and its callees read).
struct CReverbConfig {
    unsigned int  m_flags;                              // +0x00
    unsigned int  m_channels;                           // +0x04
    unsigned int  m_sampleRate;                         // +0x08
    unsigned char _pad0c[0x1c - 0x0c];
    unsigned int  m_tapLengths[4];                      // +0x1c
};

// libpk's inline-assembly float-to-long helper (round to nearest, the FPU default)
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
// libpk's round-half-down helper (out-of-line copy at 0x00C3FF70): round(value - 0.5), negatives step back up by one
__forceinline long RoundHalfDownToLong(float value)
{
    long result = FloatToLong(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
}

class CDspGain {                                        // vtable 0x012B9578, 0x14 bytes
public:
    void Reset();                                       // 0x00C744E0
    unsigned char _body[0x14];
};
class CCombFilter {                                     // vtable 0x012B9588, 0x14 bytes
public:
    void Allocate(unsigned int length, float** pMemory, unsigned int* pBytes, unsigned int index);   // 0x00C75F80
    unsigned char _body[0x14];
};
class CReverbTapBank {                                  // vtable 0x012B9590, 0xb0 bytes
public:
    void Allocate(unsigned int channels, const unsigned int* lengths, float** pMemory, unsigned int* pBytes,
                  unsigned int* pIndex);                // 0x00C75C30
    unsigned char _body[0xb0];
};
class CReverbDiffuser {                                 // vtable 0x012BF080, 0x9c bytes
public:
    void Allocate(const CReverbConfig* config, float** pMemory, unsigned int* pBytes, unsigned int* pIndex);   // 0x00C755B0
    unsigned char _body[0x9c];
};
class CReverbPreDelay {                                 // vtable 0x012B9580, 0x18 bytes
public:
    void Allocate(unsigned int rate, float** pMemory, unsigned int* pBytes, unsigned int index);   // 0x00C76DD0
    unsigned char _body[0x18];
};
class CReverbDelay {                                    // vtable 0x012B957C, 0x14 bytes
public:
    void Allocate(unsigned int rate, float** pMemory, unsigned int* pBytes, unsigned int index);   // 0x00C76AB0
    unsigned char _body[0x14];
};
class CStereoDelayLine {                                // vtable 0x012BF078, 0x24 bytes
public:
    void Allocate(unsigned int rate, float** pMemory, unsigned int* pBytes);   // 0x00C75050
    unsigned char _body[0x24];
};

// Layout: the landed ctor 0x00C6A650's member offsets; the settings-block base (+0..+0x28) and the format member are
// opaque here (no shared header: libpk has no PDB), only the line-index counter +0x18 is named.
class CReverbProcessor {
public:
    unsigned char    _base[0x18];                       // vtable 0x012B9594 + CVtable_012bf0c8 settings base
    unsigned int     m_lineIndex;                       // +0x018 next delay-line index
    unsigned char    _pad1c[0x6c - 0x1c];
    CDspGain         m_inputGain;                       // +0x06c
    CDspGain         m_outputGain;                      // +0x080
    CReverbTapBank   m_taps;                            // +0x094
    CReverbDiffuser  m_diffuser;                        // +0x144
    CReverbPreDelay  m_preDelay;                        // +0x1e0
    CCombFilter      m_combs[4];                        // +0x1f8
    CReverbDelay     m_delay;                           // +0x248
    CStereoDelayLine m_stereoDelay;                     // +0x25c
    unsigned char    _pad280[0x290 - 0x280];
    unsigned int     m_firstLineIndex;                  // +0x290
    void AllocateMemory(const CReverbConfig* config, float** pMemory, unsigned int* pBytes);   // 0x00C69A70
    void Allocate(const CReverbConfig* config, float* memory, unsigned int* pBytes);
};

void CReverbProcessor::Allocate(const CReverbConfig* config, float* memory, unsigned int* pBytes)
{
    m_lineIndex = 0;
    unsigned int bytes = 0;
    AllocateMemory(config, &memory, &bytes);
    m_inputGain.Reset();
    m_outputGain.Reset();
    m_firstLineIndex = m_lineIndex++;
    unsigned int length = (RoundHalfDownToLong((float)config->m_sampleRate * 0.2f) + 128 + 3) & ~3;
    for (int i = 0; i < 4; ++i) {
        m_combs[i].Allocate(length, &memory, &bytes, m_lineIndex);
        m_lineIndex++;
    }
    m_taps.Allocate(config->m_channels, config->m_tapLengths, &memory, &bytes, &m_lineIndex);
    m_diffuser.Allocate(config, &memory, &bytes, &m_lineIndex);
    m_delay.Allocate(config->m_sampleRate, &memory, &bytes, m_lineIndex);
    m_preDelay.Allocate(config->m_sampleRate, &memory, &bytes, ++m_lineIndex);
    m_lineIndex++;
    m_stereoDelay.Allocate(config->m_sampleRate, &memory, &bytes);
    *pBytes = bytes;
}