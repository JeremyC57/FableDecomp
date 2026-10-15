// CLight::SetIntegerProperty(long voice, long level) -- validate a level in millibels (-10000..0, else last error -2, result -3), store it in the voice's record (Level) when it changed and, when that voice is the current, live one, commit m_slots[voice].BaseLevel + level (m_committedLevel) and mark dirty bit 8; returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C63270, length 112 bytes.
// Callees: none.
// Shape: the range test (`jl`/`jg`) jumps to the failure block at the end; the stored level is reached through a
// reference local; `stored != level` keeps the member on the left of the compare.
// Local model (no PDB class carries these offsets): an EAX/DirectSound-style property block like the landed
// CEntity::SetVectorProperty_XY (0x00C647B0) -- per-voice 0x1c-byte slots at +0x94 (base level +0x00, flags +0x08
// with bit 0 = slot live), per-voice 0x70-byte property records from +0xfc (Value, Level, Level2, Level3), the
// committed copies of the current voice's properties at +0x348.., the dirty mask at +0x3bc and the last error at
// +0x3d4 (-2 = invalid parameter; the setters then return -3). Array lengths are bounded by the next member.
struct SVoiceSlot {                                // 0x1c bytes
    long          BaseLevel;                       // +0x00
    unsigned char _pad04[4];
    unsigned char Flags;                           // +0x08 bit 0: live
    unsigned char _pad09[0x13];
};
struct SVoiceProperties {                          // 0x70 bytes
    float         Value;                           // +0x00
    long          Level;                           // +0x04
    long          Level2;                          // +0x08
    long          Level3;                          // +0x0c
    unsigned char _pad10[0x60];
};
struct CLight {
    unsigned char    _pad00[4];
    long             m_currentVoice;               // +0x04
    unsigned char    _pad08[0x8c];
    SVoiceSlot       m_slots[3];                   // +0x94
    unsigned char    _pad0e8[0x14];
    SVoiceProperties m_voices[5];                  // +0xfc
    unsigned char    _pad32c[0x1c];
    float            m_committedValue;             // +0x348
    long             m_committedLevel;             // +0x34c
    long             m_committedLevel2;            // +0x350
    long             m_committedLevel3;            // +0x354
    unsigned char    _pad358[0x64];
    unsigned long    m_dirty;                      // +0x3bc
    unsigned char    _pad3c0[0x14];
    long             m_lastError;                  // +0x3d4
    long SetIntegerProperty(long voice, long level);
};

long CLight::SetIntegerProperty(long voice, long level)
{
    if (level < -10000 || level > 0)
    {
        m_lastError = -2;
        return -3;
    }
    long& stored = m_voices[voice].Level;
    if (stored != level)
    {
        stored = level;
        if (m_currentVoice == voice && (m_slots[voice].Flags & 1))
        {
            m_committedLevel = m_slots[voice].BaseLevel + level;
            m_dirty |= 8;
        }
    }
    return 0;
}