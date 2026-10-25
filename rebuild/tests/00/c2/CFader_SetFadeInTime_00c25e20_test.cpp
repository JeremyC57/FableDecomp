// behaviour test for 0x00C25E20 CFader::SetFadeInTime: step = period / time, or the whole range when time < period
#include <cstdio>

struct CFader {
    float m_timeStep, m_level;
    char  m_param[0x34];
    float m_silentTime, m_maxRate, m_fadeInStep, m_fadeOutStep;
    int   m_state;
    void SetFadeInTime(float time);
};

void CFader::SetFadeInTime(float time)
{
    if (time < m_timeStep)
        m_fadeInStep = 1.0f;
    else
        m_fadeInStep = m_timeStep / time;
}

static bool Near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }

int main()
{
    CFader f;
    f.m_timeStep = 10.0f; f.m_fadeOutStep = -7.0f;
    f.SetFadeInTime(250.0f); bool ok = Near(f.m_fadeInStep, 0.04f);
    f.SetFadeInTime(10.0f);  ok = ok && Near(f.m_fadeInStep, 1.0f);
    f.SetFadeInTime(5.0f);   ok = ok && f.m_fadeInStep == 1.0f;
    f.SetFadeInTime(40.0f);  ok = ok && Near(f.m_fadeInStep, 0.25f) && f.m_fadeOutStep == -7.0f;
    if (!ok || sizeof(CFader) != 0x50) { std::printf("00c25e20_TEST FAIL\n"); return 1; }
    std::printf("00c25e20_TEST PASS\n");
    return 0;
}