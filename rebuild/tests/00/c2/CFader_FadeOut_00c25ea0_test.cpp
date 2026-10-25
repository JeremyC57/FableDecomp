// behaviour test for 0x00C25EA0 CFader::FadeOut: unless silent, go silent at once when the parameter is 0, else start fading out
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
    void FadeOut();
};

void CFader::FadeOut()
{
    if (m_state != 2) {
        if (m_param.GetValue() == 0.0f) {
            m_level = 0.0f;
            m_state = 2;
        } else
            m_state = 0;
    }
}

int main()
{
    CFader f;
    f.m_param.m_bAngle = false;
    f.m_level = 0.7f; f.m_state = 3; f.m_param.m_value = 0.0f;
    f.FadeOut(); bool ok = f.m_state == 2 && f.m_level == 0.0f;
    f.m_level = 0.7f; f.m_state = 1; f.m_param.m_value = 0.3f;
    f.FadeOut(); ok = ok && f.m_state == 0 && f.m_level == 0.7f;
    f.m_level = 0.4f; f.m_state = 2; f.m_param.m_value = 0.0f;   // already silent: untouched
    f.FadeOut(); ok = ok && f.m_state == 2 && f.m_level == 0.4f;
    if (!ok) { std::printf("00c25ea0_TEST FAIL\n"); return 1; }
    std::printf("00c25ea0_TEST PASS\n");
    return 0;
}