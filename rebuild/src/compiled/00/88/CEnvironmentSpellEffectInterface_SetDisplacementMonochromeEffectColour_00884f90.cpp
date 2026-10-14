// CEnvironmentSpellEffectInterface::SetDisplacementMonochromeEffectColour(const SColourF& colour, bool keepPrevious) -- copy the 16-byte colour into the current slot at +0x54 and, unless asked to keep it, into the previous-frame slot at +0xd4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00884F90, length 71 bytes.
// Callees: none. Shape: two inline 16-byte struct assignments (the first through `lea edx`, the second through `add ecx,0D4h`); the flag byte is tested through dl
// after the first copy; `ret 8`.
struct SColourF_00884f90 { float m_r, m_g, m_b, m_a; };   // 16 bytes

struct CEnvironmentSpellEffectInterface {
    unsigned char     _pad[0x54];
    SColourF_00884f90 m_monochromeColour;          // +0x54
    unsigned char     _pad64[0x70];
    SColourF_00884f90 m_previousMonochromeColour;  // +0xd4
    void SetDisplacementMonochromeEffectColour(const SColourF_00884f90& colour, bool keepPrevious);
};

void CEnvironmentSpellEffectInterface::SetDisplacementMonochromeEffectColour(const SColourF_00884f90& colour, bool keepPrevious)
{
    m_monochromeColour = colour;
    if (!keepPrevious)
        m_previousMonochromeColour = colour;
}