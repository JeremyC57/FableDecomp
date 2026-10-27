#include <cstdio>

// Behaviour model of CSoundEvent::ApplyPan (0x00C21360): store the pan only while the mode is zero.
struct Level { float m_gain; int m_muted; };
struct Event {
    Level m_level; int m_mode; unsigned short m_state; float m_pan; int m_checks;
    Level m_playerLevel; bool m_hasPlayer;
    void CheckState(int) { ++m_checks; }
    int GetMode() { CheckState(-1); return m_mode; }
    void Set2DPosition(float pan) { m_pan = pan; }
    void GetLevel(void*, Level* level) { CheckState(-1); *level = m_hasPlayer ? m_playerLevel : m_level; }
};
static void ApplyPan(Event& e, void* manager, float pan) { if (!e.GetMode()) e.Set2DPosition(pan); }

int main()
{

    Event e; e.m_checks = 0; e.m_mode = 1; e.m_pan = 0.0f;
    ApplyPan(e, 0, 0.5f); if (e.m_pan != 0.0f) { std::printf("00c21360_TEST FAIL\n"); return 1; }
    e.m_mode = 0;
    ApplyPan(e, 0, -0.25f); if (e.m_pan != -0.25f || e.m_checks != 2) { std::printf("00c21360_TEST FAIL\n"); return 1; }
    std::printf("00c21360_TEST PASS\n");
    return 0;
}