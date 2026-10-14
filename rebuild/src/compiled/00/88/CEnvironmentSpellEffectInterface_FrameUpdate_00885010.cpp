// CEnvironmentSpellEffectInterface::FrameUpdate() -- copy the four current-frame records (+4 / +0x34 / +0x54 / +0x64) into their previous-frame slots (+0x84 / +0xb4 / +0xd4 / +0xe4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00885010, length 86 bytes.
// Callees: none. Shape: the 0x30- and 0x20-byte records go through `rep movsd`, the 16-byte one through four register moves (`lea ecx` / `lea edx` for the two
// addresses); `this` is kept in eax.
struct SRecord30_00885010 { unsigned char _body[0x30]; };
struct SRecord20_00885010 { unsigned char _body[0x20]; };
struct SColourF_00885010 { float m_r, m_g, m_b, m_a; };

struct CEnvironmentSpellEffectInterface {
    unsigned char      _pad[4];
    SRecord30_00885010 m_filter;                   // +0x04
    SRecord20_00885010 m_blur;                     // +0x34
    SColourF_00885010  m_monochromeColour;         // +0x54
    SRecord20_00885010 m_displacement;             // +0x64
    SRecord30_00885010 m_previousFilter;           // +0x84
    SRecord20_00885010 m_previousBlur;             // +0xb4
    SColourF_00885010  m_previousMonochromeColour; // +0xd4
    SRecord20_00885010 m_previousDisplacement;     // +0xe4
    void FrameUpdate();
};

void CEnvironmentSpellEffectInterface::FrameUpdate()
{
    m_previousFilter = m_filter;
    m_previousBlur = m_blur;
    m_previousMonochromeColour = m_monochromeColour;
    m_previousDisplacement = m_displacement;
}