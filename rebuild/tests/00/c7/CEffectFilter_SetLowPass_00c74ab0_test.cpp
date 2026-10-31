// Behaviour test for CEffectFilter::SetLowPass 0x00C74AB0 (copy of the model + logic).
#include <cstdio>
#include <cmath>
struct CEffectFilter {
    float Gain, m_a0, m_a1, m_a2, m_b0, m_b1, m_b2;
    void SetLowPass(long cutoff, float reference, unsigned long sampleRate);
};
void CEffectFilter::SetLowPass(long cutoff, float reference, unsigned long sampleRate)
{
    if (cutoff == 0) {
        m_a0 = 1.0f; m_a1 = 0.0f; m_a2 = 0.0f; m_b0 = 1.0f; m_b1 = 0.0f; m_b2 = 0.0f;
        return;
    }
    float frequency = (float)std::pow(10.0, (double)((cutoff + 300) * 0.00025085834f));
    frequency *= reference;
    float rate = (float)sampleRate;
    float limit = rate * 0.495f;
    if (frequency > limit)
        frequency = limit;
    float angle = frequency * 3.14159265f;
    m_a0 = 1.0f;
    float k = (float)std::tan(angle / rate);
    float kk = k * k;
    float d = k * 1.41421356f + kk + 1.0f;
    float a = (1.0f - kk) / d;
    float x = (kk + kk) / (d * d);
    float y = ((1.0f - a) * (1.0f - a) + x) * 0.25f;
    m_a1 = a + a;
    m_a2 = -(a * a + x);
    m_b0 = y;
    m_b1 = y + y;
    m_b2 = y;
}
static bool near(double a, double b) { return std::fabs(a - b) < 1e-4 * (1.0 + std::fabs(b)); }
int main()
{
    CEffectFilter f;
    f.Gain = 7.0f; f.m_a0 = f.m_a1 = f.m_a2 = f.m_b0 = f.m_b1 = f.m_b2 = 9.0f;
    f.SetLowPass(0, 1000.0f, 44100);
    if (f.Gain != 7.0f || f.m_a0 != 1.0f || f.m_a1 != 0.0f || f.m_a2 != 0.0f || f.m_b0 != 1.0f || f.m_b1 != 0.0f || f.m_b2 != 0.0f) {
        std::printf("00c74ab0_TEST FAIL\n"); return 1;
    }
    // cutoff -300 cents: 2^0 = 1, so the corner is the reference itself.
    f.SetLowPass(-300, 1000.0f, 44100);
    double k = std::tan(3.14159265 * 1000.0 / 44100.0), kk = k * k, d = 1.0 + std::sqrt(2.0) * k + kk;
    if (!near(f.m_a0, 1.0) || !near(f.m_a1, 2.0 * (1.0 - kk) / d) || !near(f.m_a2, -(1.0 + kk * kk) / (d * d))
        || !near(f.m_b0, kk / d) || !near(f.m_b1, 2.0 * kk / d) || !near(f.m_b2, kk / d)) {
        std::printf("00c74ab0_TEST FAIL\n"); return 1;
    }
    // unity DC gain: (b0 + b1 + b2) / (1 - a1 - a2) == 1
    if (!near((f.m_b0 + f.m_b1 + f.m_b2) / (1.0 - f.m_a1 - f.m_a2), 1.0)) { std::printf("00c74ab0_TEST FAIL\n"); return 1; }
    // a huge cutoff clamps the corner to 0.495 * rate
    CEffectFilter g;
    g.SetLowPass(100000, 1000.0f, 8000);
    double k2 = std::tan(3.14159265 * 0.495), kk2 = k2 * k2, d2 = 1.0 + std::sqrt(2.0) * k2 + kk2;
    if (!near(g.m_b0, kk2 / d2) || !near(g.m_a1, 2.0 * (1.0 - kk2) / d2)) { std::printf("00c74ab0_TEST FAIL\n"); return 1; }
    std::printf("00c74ab0_TEST PASS\n");
    return 0;
}