// Behaviour test for CCodaVoice::UpdateVibrato (0x00C59890): the start delay, the half-period entry, the alternating
// rising/falling halves, the zero-depth hold and the untouched 0x180 phase.
#include <cstdio>
struct SVibratoParams { float delay, rate, depth; };
struct CCodaVoice {
    unsigned int m_flags; float m_pitchScale, m_vibratoUp, m_vibratoDown; int m_vibratoCounter, m_vibratoPeriod;
    void UpdateVibrato(const SVibratoParams* vib)
    {
        unsigned int state = m_flags & 0x180;
        switch (state) {
        case 0:
            if (m_vibratoCounter-- > 0) break;
            state = 0x80;
            m_vibratoCounter = m_vibratoPeriod / 2;
        case 0x80:
        case 0x100:
            if (vib->depth != 0.0f) {
                if (m_vibratoCounter-- <= 0) { state ^= 0x180; m_vibratoCounter = m_vibratoPeriod; }
                m_pitchScale *= state == 0x80 ? m_vibratoUp : m_vibratoDown;
            }
            break;
        }
        m_flags = (m_flags & ~0x180u) | state;
    }
};
int main()
{
    SVibratoParams p = { 0, 1, 0.5f };
    CCodaVoice v; v.m_flags = 0x1; v.m_pitchScale = 1.0f; v.m_vibratoUp = 2.0f; v.m_vibratoDown = 0.5f;
    v.m_vibratoCounter = 1; v.m_vibratoPeriod = 4;
    v.UpdateVibrato(&p);                                 // delay: 1 -> 0, nothing else
    int ok = v.m_flags == 0x1 && v.m_vibratoCounter == 0 && v.m_pitchScale == 1.0f;
    v.UpdateVibrato(&p);                                 // delay over: rising half, counter 2 -> 1, scale x2
    ok = ok && (v.m_flags & 0x180) == 0x80 && (v.m_flags & 1) && v.m_vibratoCounter == 1 && v.m_pitchScale == 2.0f;
    v.UpdateVibrato(&p);                                 // 1 -> 0, x2
    v.UpdateVibrato(&p);                                 // 0: flip to falling, counter = 4, x0.5
    ok = ok && (v.m_flags & 0x180) == 0x100 && v.m_vibratoCounter == 4 && v.m_pitchScale == 2.0f;
    p.depth = 0.0f;
    v.UpdateVibrato(&p);                                 // zero depth: nothing moves
    ok = ok && (v.m_flags & 0x180) == 0x100 && v.m_vibratoCounter == 4 && v.m_pitchScale == 2.0f;
    v.m_flags = 0x181; v.m_vibratoCounter = 9;
    v.UpdateVibrato(&p);                                 // phase 0x180: written back unchanged
    ok = ok && v.m_flags == 0x181 && v.m_vibratoCounter == 9;
    if (!ok) { std::printf("00c59890_TEST FAIL\n"); return 1; }
    std::printf("00c59890_TEST PASS\n");
    return 0;
}