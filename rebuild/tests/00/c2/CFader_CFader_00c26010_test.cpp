// behaviour test for 0x00C26010 CFader::CFader: period stored, silent start, parameter reset to 0, 250 defaults
#include <cstdio>

static float g_target = -1.0f, g_rate = -1.0f, g_time = -1.0f;

struct CRampedParam {
    float m_value, m_target, m_targetRate, m_rate, m_10, m_time, m_duration, m_coef[5];
    bool  m_bAngle;
    CRampedParam() : m_bAngle(false) {}
    void SetTarget(float target, float rate, float time) { g_target = target; g_rate = rate; g_time = time; m_value = target; }
};

struct CFader {
    float        m_timeStep, m_level;
    CRampedParam m_param;
    float        m_silentTime, m_maxRate, m_fadeInStep, m_fadeOutStep;
    int          m_state;
    explicit CFader(float timeStep);
    void SetFadeInTime(float time);
    void SetFadeOutTime(float time);
    void SetMaxRate(float rate);
    void SetValue(float value, float time) { m_param.SetTarget(value, 0.0f, time); }
};

void CFader::SetFadeInTime(float time)
{
    if (time < m_timeStep)
        m_fadeInStep = 1.0f;
    else
        m_fadeInStep = m_timeStep / time;
}

void CFader::SetFadeOutTime(float time)
{
    if (time < m_timeStep)
        m_fadeOutStep = 1.0f;
    else
        m_fadeOutStep = m_timeStep / time;
}

void CFader::SetMaxRate(float rate)
{
    m_maxRate = rate;
}

CFader::CFader(float timeStep)
    : m_timeStep(timeStep), m_level(0.0f), m_silentTime(0.0f), m_state(2)
{
    SetValue(0.0f, 0.0f);
    SetMaxRate(250.0f);
    SetFadeOutTime(250.0f);
    SetFadeInTime(250.0f);
}

static bool Near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }

int main()
{
    CFader f(10.0f);
    bool ok = f.m_timeStep == 10.0f && f.m_level == 0.0f && !f.m_param.m_bAngle && f.m_silentTime == 0.0f
           && f.m_state == 2 && f.m_maxRate == 250.0f && Near(f.m_fadeInStep, 0.04f) && Near(f.m_fadeOutStep, 0.04f)
           && g_target == 0.0f && g_rate == 0.0f && g_time == 0.0f;
    CFader g(500.0f);                                   // period longer than the default fade: one-step fades
    ok = ok && g.m_fadeInStep == 1.0f && g.m_fadeOutStep == 1.0f;
    if (!ok || sizeof(CFader) != 0x50) { std::printf("00c26010_TEST FAIL\n"); return 1; }
    std::printf("00c26010_TEST PASS\n");
    return 0;
}