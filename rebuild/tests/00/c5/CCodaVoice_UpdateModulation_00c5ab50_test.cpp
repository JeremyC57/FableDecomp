// Behaviour test for 0x00C5AB50 CCodaVoice::UpdateModulation: the delay phase, half-period entry, half flips, the three
// depth-gated multipliers and the phase write-back into flag bits 15..16 (other flag bits preserved).
#include <cstdio>
#include <cmath>

struct SModulationParams { float delay, rate, pitchDepth, thirdDepth; int levelDepth; };
struct CCodaVoice {
    unsigned int m_flags;
    float m_modulationScale, m_modulationUp, m_modulationDown, m_levelScale, m_levelUp, m_levelDown;
    float m_thirdScale, m_thirdUp, m_thirdDown;
    int m_modulationCounter, m_modulationPeriod;
    void UpdateModulation(const SModulationParams* mod)
    {
        unsigned int state = m_flags & 0x18000;
        switch (state) {
        case 0:
            if (m_modulationCounter-- > 0)
                break;
            state = 0x8000;
            m_modulationCounter = m_modulationPeriod / 2;
        case 0x8000:
        case 0x10000:
            if (m_modulationCounter-- <= 0) {
                state ^= 0x18000;
                m_modulationCounter = m_modulationPeriod;
            }
            if (mod->pitchDepth != 0.0f)
                m_modulationScale *= state == 0x8000 ? m_modulationUp : m_modulationDown;
            if (mod->levelDepth != 0)
                m_levelScale *= state == 0x8000 ? m_levelUp : m_levelDown;
            if (mod->thirdDepth != 0.0f)
                m_thirdScale *= state == 0x8000 ? m_thirdUp : m_thirdDown;
            break;
        }
        m_flags = (m_flags & ~0x18000) | state;
    }
};
static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }

int main()
{
    int bad = 0;
    SModulationParams mod = { 0.0f, 1.0f, 0.5f, 0.0f, 3 };
    CCodaVoice v;
    v.m_flags = 0x40000003; v.m_modulationScale = 1.0f; v.m_modulationUp = 2.0f; v.m_modulationDown = 0.5f;
    v.m_levelScale = 1.0f; v.m_levelUp = 3.0f; v.m_levelDown = 0.25f; v.m_thirdScale = 1.0f; v.m_thirdUp = 5.0f; v.m_thirdDown = 7.0f;
    v.m_modulationCounter = 1; v.m_modulationPeriod = 4;
    v.UpdateModulation(&mod);                            // delay: counter 1 -> 0, still phase 0
    if (v.m_modulationCounter != 0 || (v.m_flags & 0x18000) != 0 || !near(v.m_modulationScale, 1.0f)) bad = 1;
    v.UpdateModulation(&mod);                            // delay over: rising half, counter 2 -> 1, multiply up
    if ((v.m_flags & 0x18000) != 0x8000 || v.m_modulationCounter != 1 || !near(v.m_modulationScale, 2.0f)
        || !near(v.m_levelScale, 3.0f) || !near(v.m_thirdScale, 1.0f) || (v.m_flags & ~0x18000u) != 0x40000003) bad = 1;
    v.UpdateModulation(&mod);                            // counter 1 -> 0, still rising
    v.UpdateModulation(&mod);                            // counter 0 -> flip to falling, counter = period
    if ((v.m_flags & 0x18000) != 0x10000 || v.m_modulationCounter != 4 || !near(v.m_modulationScale, 2.0f)
        || !near(v.m_levelScale, 9.0f * 0.25f)) bad = 1;
    mod.pitchDepth = 0.0f; mod.levelDepth = 0; mod.thirdDepth = 1.0f;
    v.UpdateModulation(&mod);                            // falling: only the third scale moves
    if (!near(v.m_modulationScale, 2.0f) || !near(v.m_levelScale, 2.25f) || !near(v.m_thirdScale, 7.0f) || v.m_modulationCounter != 3) bad = 1;
    v.m_flags |= 0x18000;                                // both bits: written straight back
    v.UpdateModulation(&mod);
    if ((v.m_flags & 0x18000) != 0x18000 || v.m_modulationCounter != 3 || !near(v.m_thirdScale, 7.0f)) bad = 1;
    if (bad) { std::printf("00c5ab50_TEST FAIL\n"); return 1; }
    std::printf("00c5ab50_TEST PASS\n");
    return 0;
}