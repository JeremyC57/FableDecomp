// Behaviour test for 0x00c632e0 (CLight level setter: Level2).
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
    long Set(long voice, long level)
    {
        if (level < -10000 || level > 0) { m_lastError = -2; return -3; }
        long& stored = m_voices[voice].Level2;
        if (stored != level) { stored = level; if (m_currentVoice == voice && (m_slots[voice].Flags & 1)) { m_committedLevel2 = (0 ? m_slots[voice].BaseLevel : 0) + level; m_dirty |= 16; } }
        return 0;
    }
};
int main()
{
    Light l; int ok = 1;
    ok &= l.Set(0, 1) == -3 && l.m_lastError == -2;
    ok &= l.Set(0, -10001) == -3;
    ok &= l.Set(0, -300) == 0 && l.m_voices[0].Level2 == -300 && l.m_dirty == 0;
    ok &= l.Set(1, -5) == 0 && l.m_dirty == 0;
    ok &= l.Set(1, -50) == 0 && l.m_dirty == 16 && l.m_committedLevel2 == (0 ? -150 : -50);
    if (!ok) { std::printf("00c632e0_TEST FAIL\n"); return 1; }
    std::printf("00c632e0_TEST PASS\n");
    return 0;
}
