// Behaviour test for 0x00C631E0 (CLight::SetLightValue).
#include <cstdio>
// Host model of the voice property block: slots (base level, live flag), per-voice records, committed copies,
// dirty mask and last error.
struct Slot { long BaseLevel; unsigned char Flags; };
struct Rec { float Value; long Level, Level2, Level3; };
struct Light {
    long m_currentVoice; Slot m_slots[3]; Rec m_voices[3];
    float m_committedValue; long m_committedLevel, m_committedLevel2, m_committedLevel3;
    unsigned int m_dirty; long m_lastError;
    Light() : m_currentVoice(1), m_committedValue(0), m_committedLevel(0), m_committedLevel2(0), m_committedLevel3(0), m_dirty(0), m_lastError(0)
    { for (int i = 0; i < 3; ++i) { m_slots[i].BaseLevel = -100 * i; m_slots[i].Flags = 1; Rec r = { 0.5f, -5, -5, -5 }; m_voices[i] = r; } }
    long Set(long voice, float value)
    {
        if (value < 0.0f || value > 1.0f) { m_lastError = -2; return -3; }
        float& stored = m_voices[voice].Value;
        if (stored != value) { stored = value; if (m_currentVoice == voice && (m_slots[voice].Flags & 1)) { m_committedValue = value; m_dirty |= 4; } }
        return 0;
    }
};
int main()
{
    Light l; int ok = 1;
    ok &= l.Set(0, 1.5f) == -3 && l.m_lastError == -2;
    ok &= l.Set(0, 0.25f) == 0 && l.m_voices[0].Value == 0.25f && l.m_dirty == 0;
    ok &= l.Set(1, 0.5f) == 0 && l.m_dirty == 0;                 // unchanged
    ok &= l.Set(1, 0.75f) == 0 && l.m_dirty == 4 && l.m_committedValue == 0.75f;
    l.m_dirty = 0; l.m_slots[1].Flags = 0;
    ok &= l.Set(1, 0.1f) == 0 && l.m_dirty == 0 && l.m_voices[1].Value == 0.1f;
    ok &= l.Set(2, -0.1f) == -3;
    if (!ok) { std::printf("00c631e0_TEST FAIL\n"); return 1; }
    std::printf("00c631e0_TEST PASS\n");
    return 0;
}