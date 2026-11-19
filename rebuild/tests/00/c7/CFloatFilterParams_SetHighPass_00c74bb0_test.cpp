// Behaviour test for CEffectFilter::SetHighPass (0x00C74BB0): pass-through set, a mid-band corner, the Nyquist clamp and the gain cap.
#include <cstdio>
#include <cmath>
struct CEffectFilter {
    float Gain, m_a0, m_a1, m_a2, m_b0, m_b1, m_b2;
    void SetHighPass(long attenuation, float reference, unsigned long sampleRate);
};
void CEffectFilter::SetHighPass(long attenuation, float reference, unsigned long sampleRate)
{
    if (attenuation == 0) {
        m_a0 = 1.0f; m_a1 = 0.0f; m_a2 = 0.0f; m_b0 = 1.0f; m_b1 = 0.0f; m_b2 = 0.0f;
        return;
    }
    float frequency = (float)pow(10.0, (double)((attenuation + 300) * -0.000501716684f));
    frequency *= reference;
    float rate = (float)sampleRate;
    float limit = rate * 0.495f;
    if (frequency > limit)
        frequency = limit;
    float omega = frequency * 6.28318548f;
    float c = (float)cos(omega / rate);
    float a = (1.0f - 0.5f * c) / (c - 0.5f);
    float s = (float)sqrt(a * a - 1.0f);
    float t = a - s;
    if (t > 1.0f || t < -1.0f)
        t = a + s;
    float g = 2.0f / (t + 1.0f);
    if (g > 10.0f)
        g = 10.0f;
    m_a0 = 1.0f; m_a1 = t; m_a2 = 0.0f; m_b0 = g; m_b1 = -g; m_b2 = 0.0f;
}
static bool near(double x, double y) { return std::fabs(x - y) < 1e-3 * (1.0 + std::fabs(y)); }
int main()
{
    CEffectFilter f;
    f.Gain = 5.0f; f.m_a1 = 7.0f; f.m_b1 = 7.0f;
    f.SetHighPass(0, 1000.0f, 44100);
    if (f.m_a0 != 1.0f || f.m_a1 != 0.0f || f.m_a2 != 0.0f || f.m_b0 != 1.0f || f.m_b1 != 0.0f || f.m_b2 != 0.0f || f.Gain != 5.0f) {
        std::printf("00c74bb0_TEST FAIL\n"); return 1;
    }
    // -300 mB: exponent 0 -> corner = reference
    f.SetHighPass(-300, 5000.0f, 44100);
    double c = std::cos(2.0 * 3.14159265358979 * 5000.0 / 44100.0);
    double a = (1.0 - 0.5 * c) / (c - 0.5);
    double t = a - std::sqrt(a * a - 1.0);
    if (t > 1.0 || t < -1.0) t = a + std::sqrt(a * a - 1.0);
    double g = 2.0 / (t + 1.0); if (g > 10.0) g = 10.0;
    if (!near(f.m_a1, t) || !near(f.m_b0, g) || f.m_b1 != -f.m_b0 || f.m_a0 != 1.0f || f.m_a2 != 0.0f || f.m_b2 != 0.0f) {
        std::printf("00c74bb0_TEST FAIL\n"); return 1;
    }
    // +300 mB doubles the exponent term: corner = reference * 2^-1
    CEffectFilter h;
    h.SetHighPass(300, 10000.0f, 44100);
    CEffectFilter r;
    r.SetHighPass(-300, 5000.0f, 44100);
    if (!near(h.m_a1, r.m_a1) || !near(h.m_b0, r.m_b0)) { std::printf("00c74bb0_TEST FAIL\n"); return 1; }
    // a corner far above Nyquist clamps to 0.495 * rate
    CEffectFilter k, l;
    k.SetHighPass(-300, 100000.0f, 22050);
    l.SetHighPass(-300, 22050.0f * 0.495f, 22050);
    if (!near(k.m_a1, l.m_a1) || !near(k.m_b0, l.m_b0)) { std::printf("00c74bb0_TEST FAIL\n"); return 1; }
    std::printf("00c74bb0_TEST PASS\n");
    return 0;
}