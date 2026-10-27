#include <cstdio>

// Behaviour model of CSoundEvent::GetMode (0x00C20EE0): settle the state, return the mode word.
struct Level { float m_gain; int m_muted; };
struct Event {
    Level m_level; int m_mode; unsigned short m_state; float m_pan; int m_checks;
    Level m_playerLevel; bool m_hasPlayer;
    void CheckState(int) { ++m_checks; }
    int GetMode() { CheckState(-1); return m_mode; }
    void Set2DPosition(float pan) { m_pan = pan; }
    void GetLevel(void*, Level* level) { CheckState(-1); *level = m_hasPlayer ? m_playerLevel : m_level; }
};

int main()
{

    Event e; e.m_checks = 0; e.m_mode = 3;
    if (e.GetMode() != 3 || e.m_checks != 1) { std::printf("00c20ee0_TEST FAIL\n"); return 1; }
    e.m_mode = 0;
    if (e.GetMode() != 0 || e.m_checks != 2) { std::printf("00c20ee0_TEST FAIL\n"); return 1; }
    std::printf("00c20ee0_TEST PASS\n");
    return 0;
}