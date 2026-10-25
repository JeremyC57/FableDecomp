// Behaviour test for 0x00C6B9A0 CStateVariableFilter::CStateVariableFilter().
#include <cstdio>
#include <cstring>

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
struct SFilterCoefficients { float m_f; float m_damping; float m_gain; };
struct CStateVariableFilter {
    SFilterCoefficients m_current;
    SFilterCoefficients m_target;
    float               m_band;
    float               m_low;
    unsigned long       m_rampPosition;
    CStateVariableFilter()
    {
        m_target.m_f = 0.0f; m_target.m_gain = 0.0f; m_target.m_damping = 0.0f;
        m_current = m_target;
        m_band = 0.0f; m_low = 0.0f; m_rampPosition = 0;
    }
};

int main()
{
    CStateVariableFilter f;
    if (!(Near(f.m_current.m_f, 0.0f) && Near(f.m_current.m_damping, 0.0f) && Near(f.m_current.m_gain, 0.0f) && Near(f.m_target.m_f, 0.0f) && Near(f.m_target.m_damping, 0.0f) && Near(f.m_target.m_gain, 0.0f) && Near(f.m_band, 0.0f) && Near(f.m_low, 0.0f) && f.m_rampPosition == 0)) {
        std::printf("00c6b9a0_TEST FAIL\n");
        return 1;
    }
    std::printf("00c6b9a0_TEST PASS\n");
    return 0;
}