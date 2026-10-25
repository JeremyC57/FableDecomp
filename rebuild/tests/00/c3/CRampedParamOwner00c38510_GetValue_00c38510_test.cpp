// Behaviour test for 00c38510: CRampedParamOwner::GetValue -- the ramped parameter's current value, wrapped into [-pi, pi) for angle parameters
#include <cstdio>

static const float kPi = 3.14159265f;
static const float kTwoPi = 6.28318531f;
static const float kMinusPi = -3.14159265f;

struct CRampedParam {
    float m_value;
    float m_target;
    float m_rates[10];
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
};

struct CRampedParamOwner {
    long m_pad[3];
    CRampedParam m_param;
    float GetValue() const { return m_param.GetValue(); }
};

static bool near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }
static int fail() { std::printf("00c38510_TEST FAIL\n"); return 1; }

int main()
{
    CRampedParamOwner o;
    o.m_param.m_bAngle = false; o.m_param.m_value = 4.0f;
    if (!near(o.GetValue(), 4.0f)) return fail();
    o.m_param.m_bAngle = true;
    if (!near(o.GetValue(), 4.0f - kTwoPi)) return fail();
    o.m_param.m_value = -4.0f;
    if (!near(o.GetValue(), -4.0f + kTwoPi)) return fail();
    o.m_param.m_value = 1.0f;
    if (!near(o.GetValue(), 1.0f)) return fail();
    std::printf("00c38510_TEST PASS\n");
    return 0;
}