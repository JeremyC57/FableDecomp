// Behaviour test for 00c45940: CAmbientCube::GetLevel -- value at +0xE0, wrapped into [-pi, pi) once when the angle flag is set
#include <cstdio>
#include <cmath>

struct CRampedParam {
    float m_value, m_target, m_rates[10];
    bool  m_bAngle;
    float GetValue() const
    {
        float value = m_value;
        if (m_bAngle) {
            if (value >= 3.14159265f) return value - 6.28318531f;
            if (value < -3.14159265f) return value + 6.28318531f;
        }
        return value;
    }
};
struct CAmbientCube {
    unsigned char m_pad00[0xe0];
    CRampedParam  m_param;
    float GetLevel() const { return m_param.GetValue(); }
};

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    static CAmbientCube o;
    o.m_param.m_value = 4.0f; o.m_param.m_bAngle = false;
    if (!Near(o.GetLevel(), 4.0f)) { std::printf("00c45940_TEST FAIL\n"); return 1; }
    o.m_param.m_bAngle = true;
    if (!Near(o.GetLevel(), 4.0f - 6.28318531f)) { std::printf("00c45940_TEST FAIL\n"); return 1; }
    o.m_param.m_value = -4.0f;
    if (!Near(o.GetLevel(), -4.0f + 6.28318531f)) { std::printf("00c45940_TEST FAIL\n"); return 1; }
    o.m_param.m_value = 1.0f;
    if (!Near(o.GetLevel(), 1.0f)) { std::printf("00c45940_TEST FAIL\n"); return 1; }
    std::printf("00c45940_TEST PASS\n");
    return 0;
}