// CLight::SetLightValue(long voice, float value) -- validate a 0..1 property value (else last error -2, result -3), store it in the voice's record when it changed and, when that voice is the current, live one, commit it (+0x348) and mark dirty bit 4; returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C631E0, length 144 bytes.
// Callees: none. Float pool: 0x0122DEDC = 0.0f, 0x0122DED8 = 1.0f.
// Shape: the range test reads the argument twice (`< 0` = `test ah,5 ; jnp`, `> 1` = `test ah,41h ; je`) with the
// failure block last; the change test is `stored != value` (the argument loaded first) (fucompp, `test ah,44h ; jnp`) through a reference
// local (`lea esi,[eax+ecx+0FCh]`); the float copies are dword moves.
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
    long SetLightValue(long voice, float value);
};

long CLight::SetLightValue(long voice, float value)
{
    if (value < 0.0f || value > 1.0f)
    {
        m_lastError = -2;
        return -3;
    }
    float& stored = m_voices[voice].Value;
    if (stored != value)
    {
        stored = value;
        if (m_currentVoice == voice && (m_slots[voice].Flags & 1))
        {
            m_committedValue = value;
            m_dirty |= 4;
        }
    }
    return 0;
}