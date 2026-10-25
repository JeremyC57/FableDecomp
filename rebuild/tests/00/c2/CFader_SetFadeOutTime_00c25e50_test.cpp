// behaviour test for 0x00C25E50 CFader::SetFadeOutTime: step = period / time, or the whole range when time < period
#include <cstdio>

struct CFader {
    float m_timeStep, m_level;
    char  m_param[0x34];
    float m_silentTime, m_maxRate, m_fadeInStep, m_fadeOutStep;
    int   m_state;
    void SetFadeOutTime(float time);
};

void CFader::SetFadeOutTime(float time)
{
    if (time < m_timeStep)
        m_fadeOutStep = 1.0f;
    else
        m_fadeOutStep = m_timeStep / time;
}

static bool Near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }

int main()
{
    CFader f;
    f.m_timeStep = 10.0f; f.m_fadeInStep = -7.0f;
    f.SetFadeOutTime(250.0f); bool ok = Near(f.m_fadeOutStep, 0.04f);
    f.SetFadeOutTime(10.0f);  ok = ok && Near(f.m_fadeOutStep, 1.0f);
    f.SetFadeOutTime(5.0f);   ok = ok && f.m_fadeOutStep == 1.0f;
    f.SetFadeOutTime(40.0f);  ok = ok && Near(f.m_fadeOutStep, 0.25f) && f.m_fadeInStep == -7.0f;
    if (!ok || sizeof(CFader) != 0x50) { std::printf("00c25e50_TEST FAIL\n"); return 1; }
    std::printf("00c25e50_TEST PASS\n");
    return 0;
}