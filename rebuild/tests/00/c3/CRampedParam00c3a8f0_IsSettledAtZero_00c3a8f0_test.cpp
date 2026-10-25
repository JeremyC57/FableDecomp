// Behaviour test for 00c3a8f0: CRampedParam::IsSettledAtZero -- 1 when the ramp has run its full duration and the (angle-wrapped) value is zero
#include <cstdio>

static const float kPi = 3.14159265f;
static const float kTwoPi = 6.28318531f;
static const float kMinusPi = -3.14159265f;

struct CRampedParam {
    float m_value, m_target, m_targetRate, m_rate, m_10, m_time, m_duration;
    float m_coef[5];
    bool m_bAngle;
    float GetValue() const
    {
        float value = m_value;
        if (m_bAngle) {
            if (value >= kPi)
                return value - kTwoPi;
            if (value < kMinusPi)
                return value + kTwoPi;
        }
        return value;
    }
    long IsSettledAtZero() const
    {
        if (m_time != m_duration)
            return 0;
        if (GetValue() == 0.0f)
            return 1;
        return 0;
    }
};

static int fail() { std::printf("00c3a8f0_TEST FAIL\n"); return 1; }

int main()
{
    CRampedParam p;
    p.m_bAngle = false; p.m_value = 0.0f; p.m_time = 1.0f; p.m_duration = 2.0f;
    if (p.IsSettledAtZero() != 0) return fail();            // still ramping
    p.m_time = 2.0f;
    if (p.IsSettledAtZero() != 1) return fail();
    p.m_value = 0.5f;
    if (p.IsSettledAtZero() != 0) return fail();
    std::printf("00c3a8f0_TEST PASS\n");
    return 0;
}