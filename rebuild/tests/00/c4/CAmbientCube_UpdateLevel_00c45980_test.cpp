// Behaviour test for CAmbientCube::UpdateLevel 0x00C45980: the time step goes to the cube's own ramped level parameter (the one after
// the eight apexes), which ramps toward its target and lands on it once the ramp time is used up; the apexes are untouched.
#include <cstdio>
#include <cmath>
struct CRampedParam {
    float m_value, m_start, m_target, m_time, m_duration;
    void Update(float dt)
    {
        m_time += dt;
        if (m_time >= m_duration) { m_time = m_duration; m_value = m_target; return; }
        m_value = m_start + (m_target - m_start) * (m_time / m_duration);
    }
};
struct SAmbientApex { float m_offset[3], m_position[3], m_minDistance; };
struct CAmbientCube {
    SAmbientApex m_apexes[8];
    CRampedParam m_level;
    void UpdateLevel(float dt) { m_level.Update(dt); }
};
int main()
{
    CAmbientCube cube;
    for (int i = 0; i < 8; ++i) cube.m_apexes[i].m_minDistance = 100.0f + i;
    cube.m_level.m_value = 0.0f; cube.m_level.m_start = 0.0f; cube.m_level.m_target = 1.0f; cube.m_level.m_time = 0.0f; cube.m_level.m_duration = 2.0f;
    cube.UpdateLevel(1.0f);
    if (std::fabs(cube.m_level.m_value - 0.5f) > 1e-5f) { std::printf("00c45980_TEST FAIL\n"); return 1; }
    cube.UpdateLevel(5.0f);
    if (std::fabs(cube.m_level.m_value - 1.0f) > 1e-5f || cube.m_apexes[3].m_minDistance != 103.0f) { std::printf("00c45980_TEST FAIL\n"); return 1; }
    std::printf("00c45980_TEST PASS\n");
    return 0;
}