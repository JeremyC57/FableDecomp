// behaviour test for 00c0b880 (standalone copy of the model)
#include <cstdio>

static float g_t, g_r, g_time;
struct CRampedParam { float m_value; void SetTarget(float target, float time) { SetTarget(target, 0.0f, time); }
    void SetTarget(float target, float rate, float time) { g_t = target; g_r = rate; g_time = time; } };

int main()
{
    CRampedParam p; p.m_value = 0.0f; p.SetTarget(2.5f, 0.75f);
    if (!(g_t == 2.5f && g_r == 0.0f && g_time == 0.75f)) { std::printf("00c0b880_TEST FAIL\n"); return 1; }
    std::printf("00c0b880_TEST PASS\n");
    return 0;
}