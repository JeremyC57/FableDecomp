#include <cstdio>
static void* g_self; static float g_target, g_rate, g_time;
struct CRampedParam { float f[12]; bool m_bAngle;
    void SetTarget(float target, float rate, float time) { g_self = this; g_target = target; g_rate = rate; g_time = time; } };
struct CFader { float m_timeStep, m_level; CRampedParam m_param; float m_silentTime, m_maxRate, m_fadeInStep, m_fadeOutStep; long m_state;
    void SetValue(float value, float time) { m_param.SetTarget(value, 0, time); } };
int main()
{
    CFader f; f.SetValue(0.75f, 120.0f);
    bool ok = (char*)g_self == (char*)&f + 8 && g_target == 0.75f && g_rate == 0.0f && g_time == 120.0f;
    if (!ok) { std::printf("00c25ff0_TEST FAIL\n"); return 1; }
    std::printf("00c25ff0_TEST PASS\n");
    return 0;
}
