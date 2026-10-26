#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SRecord30_00885010 { unsigned char _body[0x30]; }; struct SRecord20_00885010 { unsigned char _body[0x20]; }; struct SColourF_00885010 { float m_r, m_g, m_b, m_a; };
class CEnvironmentSpellEffectInterface { public: unsigned char _pad[4]; SRecord30_00885010 m_filter; SRecord20_00885010 m_blur; SColourF_00885010 m_monochromeColour; SRecord20_00885010 m_displacement; SRecord30_00885010 m_previousFilter; SRecord20_00885010 m_previousBlur; SColourF_00885010 m_previousMonochromeColour; SRecord20_00885010 m_previousDisplacement;
    void FrameUpdate() { m_previousFilter = m_filter; m_previousBlur = m_blur; m_previousMonochromeColour = m_monochromeColour; m_previousDisplacement = m_displacement; } };
int main() {
    CEnvironmentSpellEffectInterface e; std::memset(&e, 0, sizeof e); e.m_filter._body[5] = 1; e.m_blur._body[2] = 2; e.m_monochromeColour.m_b = 3.0f; e.m_displacement._body[0x1f] = 4; e.FrameUpdate();
    if (e.m_previousFilter._body[5] != 1 || e.m_previousBlur._body[2] != 2 || e.m_previousMonochromeColour.m_b != 3.0f || e.m_previousDisplacement._body[0x1f] != 4 || e.m_previousFilter._body[0] != 0) { std::printf("00885010_TEST FAIL\n"); return 1; }
    std::printf("00885010_TEST PASS\n"); return 0;
}