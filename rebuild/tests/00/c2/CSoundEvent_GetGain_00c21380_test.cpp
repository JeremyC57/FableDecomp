#include <cstdio>

// Behaviour model of CSoundEvent::GetGain (0x00C21380): the level record's gain, its muted word stored out.
struct Level { float m_gain; int m_muted; };
struct Event {
    Level m_level; int m_mode; unsigned short m_state; float m_pan; int m_checks;
    Level m_playerLevel; bool m_hasPlayer;
    void CheckState(int) { ++m_checks; }
    int GetMode() { CheckState(-1); return m_mode; }
    void Set2DPosition(float pan) { m_pan = pan; }
    void GetLevel(void*, Level* level) { CheckState(-1); *level = m_hasPlayer ? m_playerLevel : m_level; }
};
static float GetGain(Event& e, void* manager, int* muted) { Level level; e.GetLevel(manager, &level); *muted = level.m_muted; return level.m_gain; }

int main()
{

    Event e; e.m_checks = 0; e.m_level.m_gain = 0.75f; e.m_level.m_muted = 1; e.m_hasPlayer = false;
    e.m_playerLevel.m_gain = 0.25f; e.m_playerLevel.m_muted = 0;
    int muted = -1;
    float g = GetGain(e, 0, &muted);
    if (g < 0.74f || g > 0.76f || muted != 1) { std::printf("00c21380_TEST FAIL\n"); return 1; }
    e.m_hasPlayer = true;
    g = GetGain(e, 0, &muted);
    if (g < 0.24f || g > 0.26f || muted != 0) { std::printf("00c21380_TEST FAIL\n"); return 1; }
    std::printf("00c21380_TEST PASS\n");
    return 0;
}