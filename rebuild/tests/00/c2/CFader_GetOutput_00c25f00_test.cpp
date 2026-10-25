// behaviour test for 0x00C25F00 CFader::GetOutput: the parameter value (wrapped into [-pi, pi) for an angle) times the level
#include <cstdio>

struct CRampedParam {
    float m_value, m_target, m_targetRate, m_rate, m_10, m_time, m_duration, m_coef[5];
    bool  m_bAngle;
    float GetValue() const;
};

static const float kPi = 3.14159265f;
static const float kTwoPi = 6.28318531f;
static const float kMinusPi = -3.14159265f;

inline float CRampedParam::GetValue() const
{
    float value = m_value;
    if (m_bAngle) {
        if (value >= kPi)
            value -= kTwoPi;
        else if (value < kMinusPi)
            value += kTwoPi;
    }
    return value;
}

struct CFader {
    float m_timeStep, m_level;
    CRampedParam m_param;
    float m_silentTime, m_maxRate, m_fadeInStep, m_fadeOutStep;
    int   m_state;
    float GetOutput() const;
};

float CFader::GetOutput() const
{
    return m_param.GetValue() * m_level;
}

static bool Near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }

int main()
{
    CFader f;
    f.m_level = 0.5f; f.m_param.m_bAngle = false; f.m_param.m_value = 4.0f;
    bool ok = Near(f.GetOutput(), 2.0f);
    f.m_param.m_bAngle = true;                       // 4 wraps to 4 - 2pi
    ok = ok && Near(f.GetOutput(), (4.0f - kTwoPi) * 0.5f);
    f.m_param.m_value = -4.0f;                       // -4 wraps to -4 + 2pi
    ok = ok && Near(f.GetOutput(), (-4.0f + kTwoPi) * 0.5f);
    f.m_param.m_value = 1.0f; f.m_level = 0.0f;
    ok = ok && f.GetOutput() == 0.0f;
    if (!ok || sizeof(CFader) != 0x50) { std::printf("00c25f00_TEST FAIL\n"); return 1; }
    std::printf("00c25f00_TEST PASS\n");
    return 0;
}