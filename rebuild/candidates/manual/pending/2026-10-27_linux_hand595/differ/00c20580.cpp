// CSoundPlayParameterOverride::ApplyTo(CSoundPlayParameters& params) -- copy into params each play parameter whose override bit is set (bits 0..9: volume, pitch, loop start, loop end, priority, delay, category, looped, pan, frequency scale) from the embedded parameter block, then merge the fade-in and group words bit by bit under their two masks
// Grade: DIFFER(332v330) -- register allocation only: retail loads params into ebx first (`mov ebx,[esp+8]`), keeps the mask in di, &m_params in ebp and spills params.GetFadeIn() to the arg slot [esp+0x14]; ours keeps params in edi and spills &m_params
// Leaf: ?ApplyTo@CSoundPlayParameterOverride@@QBEXAAVCSoundPlayParameters@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C20580, 330 bytes, __thiscall, `ret 4`.
// Identity: libpk audio code (no PDB / RTTI): the only caller 0x00C129C0 applies it to a CSoundPlayParameters block on its
//   stack and then reads that block's frequency scale (0x00C1BB00). `this` = { override bits +0, fade-in mask +4, group mask +6,
//   CSoundPlayParameters +8 }. Class and method names are descriptive; the CSoundPlayParameters setters keep the landed
//   descriptive spellings of CSoundTemplateRecord::FillPlayParameters 0x00C35770 (SetLoopStart / SetLoopEnd take a float:
//   their body 0x00C1BA20 `fld dword [esp+4]`s the argument, and this row stores a float into it; NAME_FIXES), the getters
//   are their mirrors (each reads the field its setter writes: volume byte +0, pan byte +1, looped bit +2, group byte +3,
//   delay +4, fade-in +6, pitch +8, priority +0xA, category +0xC, frequency scale +0xE, loop start +0x10, loop end +0x12).
//   The manifest label "CTCHero_ProcessInventoryFlagMask" is wrong.
// Callees: the getters 0x00C1B9C0..0x00C1BB00 on the embedded block and the setters 0x00C1BB20..0x00C1BC70 on params.
class CSoundPlayParameters {                       // 0x14 bytes (libpk), packed fields behind accessors
public:
    float GetVolume() const;                       // 0x00C1B9C0
    float GetPitch() const;                        // 0x00C1B9E0
    float GetPan() const;                          // 0x00C1BA00
    float GetLoopStart() const;                    // 0x00C1BA60
    float GetLoopEnd() const;                      // 0x00C1BA70
    long GetPriority() const;                      // 0x00C1BA80
    long GetDelay() const;                         // 0x00C1BA90
    unsigned short GetFadeIn() const;              // 0x00C1BAA0
    unsigned short GetGroup() const;               // 0x00C1BAB0
    long GetCategory() const;                      // 0x00C1BAC0
    bool GetLooped() const;                        // 0x00C1BAD0
    float GetFrequencyScale() const;               // 0x00C1BB00
    void SetVolume(float v);                       // 0x00C1BB20
    void SetPitch(float v);                        // 0x00C1BB40
    void SetPan(float v);                          // 0x00C1BB60
    void SetLoopStart(float v);                    // 0x00C1BB80
    void SetLoopEnd(float v);                      // 0x00C1BBA0
    void SetPriority(long v);                      // 0x00C1BBC0
    void SetDelay(short v);                        // 0x00C1BBD0
    void SetFadeIn(unsigned short v);              // 0x00C1BBE0
    void SetGroup(unsigned short v);               // 0x00C1BBF0
    void SetCategory(long v);                      // 0x00C1BC00
    void SetLooped(bool v);                        // 0x00C1BC10
    void SetFrequencyScale(float v);               // 0x00C1BC70
    unsigned char m_data[0x14];
};

static inline unsigned short MergeMasked(unsigned short dst, unsigned short src, unsigned short mask)
{
    return (unsigned short)((src & mask) | (~mask & dst));
}

class CSoundPlayParameterOverride {
public:
    unsigned long        m_overrides;              // +0x00: one bit per parameter
    unsigned short       m_fadeInMask;             // +0x04
    unsigned short       m_groupMask;              // +0x06
    CSoundPlayParameters m_params;                 // +0x08

    void ApplyTo(CSoundPlayParameters& params) const;
};

void CSoundPlayParameterOverride::ApplyTo(CSoundPlayParameters& params) const
{
    if (m_overrides & 0x001)
        params.SetVolume(m_params.GetVolume());
    if (m_overrides & 0x002)
        params.SetPitch(m_params.GetPitch());
    if (m_overrides & 0x004)
        params.SetLoopStart(m_params.GetLoopStart());
    if (m_overrides & 0x008)
        params.SetLoopEnd(m_params.GetLoopEnd());
    if (m_overrides & 0x010)
        params.SetPriority(m_params.GetPriority());
    if (m_overrides & 0x020)
        params.SetDelay((short)m_params.GetDelay());
    if (m_overrides & 0x040)
        params.SetCategory(m_params.GetCategory());
    if (m_overrides & 0x080)
        params.SetLooped(m_params.GetLooped());
    if (m_overrides & 0x100)
        params.SetPan(m_params.GetPan());
    if (m_overrides & 0x200)
        params.SetFrequencyScale(m_params.GetFrequencyScale());

    params.SetFadeIn(MergeMasked(params.GetFadeIn(), m_params.GetFadeIn(), m_fadeInMask));
    params.SetGroup(MergeMasked(params.GetGroup(), m_params.GetGroup(), m_groupMask));
}
