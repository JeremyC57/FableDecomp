// behaviour test for 0x00C0B510 CRampedParam::SetImmediate: jump to a value (wrapped when the param is an angle), stop the ramp
#include <cstdio>

struct CRampedParam {
    float m_value;
    float m_target;
    float m_targetRate;
    float m_rate;
    float m_10;
    float m_time;
    float m_duration;
    float m_coef[5];
    bool  m_bAngle;
    void SetImmediate(float value);
};

static const float kPi = 3.14159265f;
static const float kTwoPi = 6.28318531f;
static const float kMinusPi = -3.14159265f;

void CRampedParam::SetImmediate(float value)
{
    if (m_bAngle) {
        if (value >= kPi)
            value -= kTwoPi;
        else if (value < kMinusPi)
            value += kTwoPi;
    }
    m_target = value;
    m_value = value;
    m_coef[0] = value;
    m_duration = 0.0f;
    m_time = 0.0f;
    m_coef[4] = 0.0f;
    m_coef[3] = 0.0f;
    m_10 = 0.0f;
    m_coef[2] = 0.0f;
    m_rate = 0.0f;
    m_coef[1] = 0.0f;
    m_targetRate = 0.0f;
}

static bool Near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }

static void Fill(CRampedParam& p, bool angle)
{
    p.m_value = p.m_target = p.m_targetRate = p.m_rate = p.m_10 = p.m_time = p.m_duration = 7.0f;
    for (int i = 0; i < 5; ++i) p.m_coef[i] = 7.0f;
    p.m_bAngle = angle;
}

static bool Stopped(const CRampedParam& p, float v)
{
    if (!Near(p.m_value, v) || !Near(p.m_target, v) || !Near(p.m_coef[0], v)) return false;
    if (p.m_targetRate != 0.0f || p.m_rate != 0.0f || p.m_10 != 0.0f || p.m_time != 0.0f || p.m_duration != 0.0f) return false;
    for (int i = 1; i < 5; ++i) if (p.m_coef[i] != 0.0f) return false;
    return true;
}

int main()
{
    CRampedParam p;
    Fill(p, false);
    p.SetImmediate(5.0f);
    if (!Stopped(p, 5.0f)) { std::printf("00c0b510_TEST FAIL\n"); return 1; }
    Fill(p, true);
    p.SetImmediate(5.0f);
    if (!Stopped(p, 5.0f - kTwoPi)) { std::printf("00c0b510_TEST FAIL\n"); return 1; }
    Fill(p, true);
    p.SetImmediate(-3.5f);
    if (!Stopped(p, -3.5f + kTwoPi)) { std::printf("00c0b510_TEST FAIL\n"); return 1; }
    Fill(p, true);
    p.SetImmediate(1.0f);
    if (!Stopped(p, 1.0f)) { std::printf("00c0b510_TEST FAIL\n"); return 1; }
    std::printf("00c0b510_TEST PASS\n");
    return 0;
}