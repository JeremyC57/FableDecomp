// behaviour test for 0x00C0B180 MultiplyMaskedParams: product of value and the (angle-wrapped) values of the masked params
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
    float GetValue() const;
};

static const float kZero = 0.0f;
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

float MultiplyMaskedParams(const CRampedParam* params, unsigned long mask, float value, unsigned long count)
{
    while (value != kZero && mask != 0 && count != 0) {
        if (mask & 1)
            value *= params->GetValue();
        --count;
        mask >>= 1;
        ++params;
    }
    return value;
}

static bool Near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }

int main()
{
    CRampedParam p[4];
    for (int i = 0; i < 4; ++i) { p[i].m_value = (float)(i + 2); p[i].m_bAngle = false; }   // 2, 3, 4, 5
    if (!Near(MultiplyMaskedParams(p, 0x5, 1.0f, 4), 8.0f)) { std::printf("00c0b180_TEST FAIL\n"); return 1; }   // 2*4
    if (!Near(MultiplyMaskedParams(p, 0xF, 0.5f, 4), 60.0f)) { std::printf("00c0b180_TEST FAIL\n"); return 1; }
    if (!Near(MultiplyMaskedParams(p, 0xF, 1.0f, 2), 6.0f)) { std::printf("00c0b180_TEST FAIL\n"); return 1; }    // count limits
    if (!Near(MultiplyMaskedParams(p, 0, 3.0f, 4), 3.0f)) { std::printf("00c0b180_TEST FAIL\n"); return 1; }
    p[1].m_bAngle = true;                                         // 3 stays 3 (< pi); make it 4 -> 4 - 2pi
    p[1].m_value = 4.0f;
    if (!Near(MultiplyMaskedParams(p, 0x2, 1.0f, 4), 4.0f - kTwoPi)) { std::printf("00c0b180_TEST FAIL\n"); return 1; }
    p[0].m_value = 0.0f;                                          // product hits zero and stops
    if (MultiplyMaskedParams(p, 0xF, 1.0f, 4) != 0.0f) { std::printf("00c0b180_TEST FAIL\n"); return 1; }
    std::printf("00c0b180_TEST PASS\n");
    return 0;
}