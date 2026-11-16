// Test: CSoundEventManager::GetFaderOutput -- forwards to the +0x7D8 fader's GetOutput (value times level).
#include <cstdio>
#include <cstddef>
struct CFader { float m_timeStep, m_level, m_value; unsigned char rest[0x50 - 12];
                float GetOutput() const { return m_value * m_level; } };
struct CSoundEventManager { unsigned char prefix[0x7d8]; CFader m_fader;
                            float GetFaderOutput() const { return m_fader.GetOutput(); } };
int main()
{
    CSoundEventManager m; m.m_fader.m_level = 0.5f; m.m_fader.m_value = 3.0f;
    bool ok = offsetof(CSoundEventManager, m_fader) == 0x7d8 && sizeof(CFader) == 0x50 && m.GetFaderOutput() == 1.5f;
    m.m_fader.m_level = 0.0f; ok = ok && m.GetFaderOutput() == 0.0f;
    std::printf(ok ? "00c11700_TEST PASS\n" : "00c11700_TEST FAIL\n");
    return ok ? 0 : 1;
}