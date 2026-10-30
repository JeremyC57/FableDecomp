// Behaviour test for 0x00C74BB0 CDspFilter::SetHighPass: pass-through set for 0 attenuation; otherwise the clamped corner,
// the first-order pole/gain and the {1, t, 0, g, -g, 0} layout.
#include <cstdio>
#include <cmath>

struct CDspFilter {
    float m_gain, m_a0, m_a1, m_a2, m_b0, m_b1, m_b2;
    void SetHighPass(int attenuation, float reference, unsigned int sampleRate)
    {
        if (attenuation == 0) {
            m_a0 = 1.0f; m_a1 = 0.0f; m_a2 = 0.0f; m_b0 = 1.0f; m_b1 = 0.0f; m_b2 = 0.0f;
            return;
        }
        float frequency = (float)std::pow(10.0, (double)((attenuation + 300) * -0.000501716684f)) * reference;
        float rate = (float)sampleRate;
        float limit = rate * 0.495f;
        if (frequency > limit)
            frequency = limit;
        float c = (float)std::cos((double)(frequency * 6.28318548f / rate));
        float a = (1.0f - 0.5f * c) / (c - 0.5f);
        float s = (float)std::sqrt((double)(a * a - 1.0f));
        float t = a - s;
        if (t > 1.0f || t < -1.0f)
            t = a + s;
        float g = 2.0f / (t + 1.0f);
        if (g > 10.0f)
            g = 10.0f;
        m_a0 = 1.0f; m_a1 = t; m_a2 = 0.0f; m_b0 = g; m_b1 = -g; m_b2 = 0.0f;
    }
};
static bool near(double a, double b, double tol) { return std::fabs(a - b) < tol; }

int main()
{
    int bad = 0;
    CDspFilter f;
    f.m_gain = 1.0f; f.m_a1 = 5.0f; f.m_b1 = 5.0f;
    f.SetHighPass(0, 100.0f, 44100);
    if (f.m_a0 != 1.0f || f.m_a1 != 0.0f || f.m_a2 != 0.0f || f.m_b0 != 1.0f || f.m_b1 != 0.0f || f.m_b2 != 0.0f) bad = 1;
    // -300 mB: corner = reference exactly (2^0)
    f.SetHighPass(-300, 250.0f, 44100);
    double c = std::cos(2 * 3.14159265358979 * 250.0 / 44100.0), a = (1 - 0.5 * c) / (c - 0.5), t = a - std::sqrt(a * a - 1);
    if (t > 1 || t < -1) t = a + std::sqrt(a * a - 1);
    double g = 2 / (t + 1); if (g > 10) g = 10;
    if (!near(f.m_a1, t, 1e-4) || !near(f.m_b0, g, 1e-4) || f.m_b1 != -f.m_b0 || f.m_a0 != 1.0f || f.m_a2 != 0.0f || f.m_b2 != 0.0f) bad = 1;
    if (!(f.m_a1 > -1.0f && f.m_a1 <= 1.0f)) bad = 1;               // the pole is always the stable root
    // a huge corner is clamped to 0.495 * rate: both clamps give the same set
    CDspFilter h, k;
    h.SetHighPass(-5000, 1.0e6f, 22050);
    k.SetHighPass(-300, 0.495f * 22050.0f, 22050);
    if (!near(h.m_a1, k.m_a1, 1e-5) || !near(h.m_b0, k.m_b0, 1e-5)) bad = 1;
    if (h.m_b0 > 10.0f) bad = 1;
    if (bad) { std::printf("00c74bb0_TEST FAIL\n"); return 1; }
    std::printf("00c74bb0_TEST PASS\n");
    return 0;
}
