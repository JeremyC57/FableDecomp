// Behaviour test for CFader::Update (0x00C25F40): ramp update, silent-time accounting, fade-in/out stepping with the 3/2 finishing states.
#include <stdio.h>
struct CFader {
    float m_timeStep, m_level, m_value, m_silentTime, m_fadeInStep, m_fadeOutStep; long m_state; int m_updates;
    float GetOutput() const { return m_value * m_level; }
    void Update()
    {
        ++m_updates;
        if (GetOutput() == 0.0f) m_silentTime += m_timeStep;
        switch (m_state) {
        case 0: if (m_level > m_fadeOutStep) m_level -= m_fadeOutStep; else { m_level = 0.0f; m_state = 2; } break;
        case 1: if (1.0f - m_level > m_fadeInStep) { float level = m_level; level += m_fadeInStep; m_level = level; } else { m_level = 1.0f; m_state = 3; } break;
        }
        if (GetOutput() != 0.0f) m_silentTime = 0.0f;
    }
};
int main()
{
    CFader f = { 0.5f, 0.0f, 2.0f, 0.0f, 0.25f, 0.5f, 1, 0 };
    f.Update();
    if (f.m_level != 0.25f || f.m_state != 1 || f.m_silentTime != 0.0f) { printf("00c25f40_TEST FAIL\n"); return 1; }
    f.Update(); f.Update();
    if (f.m_level != 0.75f || f.m_state != 1) { printf("00c25f40_TEST FAIL\n"); return 1; }
    f.Update();
    if (f.m_level != 1.0f || f.m_state != 3) { printf("00c25f40_TEST FAIL\n"); return 1; }
    f.m_state = 0; f.Update();
    if (f.m_level != 0.5f || f.m_state != 0) { printf("00c25f40_TEST FAIL\n"); return 1; }
    f.Update();
    if (f.m_level != 0.0f || f.m_state != 2 || f.m_silentTime != 0.0f) { printf("00c25f40_TEST FAIL\n"); return 1; }
    f.Update();
    if (f.m_silentTime != 0.5f || f.m_state != 2) { printf("00c25f40_TEST FAIL\n"); return 1; }
    printf("00c25f40_TEST PASS\n"); return 0;
}