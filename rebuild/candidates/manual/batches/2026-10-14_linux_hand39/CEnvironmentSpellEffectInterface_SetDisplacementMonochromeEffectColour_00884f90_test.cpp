#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SColourF_00884f90 { float m_r, m_g, m_b, m_a; };
struct CEnvironmentSpellEffectInterface { unsigned char _pad[0x54]; SColourF_00884f90 m_monochromeColour; unsigned char _pad64[0x70]; SColourF_00884f90 m_previousMonochromeColour;
    void SetDisplacementMonochromeEffectColour(const SColourF_00884f90& colour, bool keepPrevious) { m_monochromeColour = colour; if (!keepPrevious) m_previousMonochromeColour = colour; } };
int main() {
    CEnvironmentSpellEffectInterface e; std::memset(&e, 0, sizeof e); SColourF_00884f90 c = { 1.0f, 0.5f, 0.25f, 1.0f }; e.SetDisplacementMonochromeEffectColour(c, false);
    if (e.m_monochromeColour.m_g != 0.5f || e.m_previousMonochromeColour.m_b != 0.25f) { std::printf("00884f90_TEST FAIL\n"); return 1; }
    SColourF_00884f90 d = { 0.0f, 0.0f, 0.0f, 0.5f }; e.SetDisplacementMonochromeEffectColour(d, true); if (e.m_monochromeColour.m_a != 0.5f || e.m_monochromeColour.m_r != 0.0f || e.m_previousMonochromeColour.m_r != 1.0f) { std::printf("00884f90_TEST FAIL\n"); return 1; }
    std::printf("00884f90_TEST PASS\n"); return 0;
}
