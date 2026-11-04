// CEffectFilter::SetShelves(float gain1, float frequency1, float gain2, float frequency2, unsigned long sampleRate) -- set the six coefficients for two cascaded first-order shelving sections
// Grade: DIFFER(588v593) -- the (float)sampleRate CSE spills to a frame slot across the if/else join; retail keeps it on the x87 stack
// Leaf: ?SetShelves@CEffectFilter@@QAEXMMMMK@Z
// Flags: none
extern "C" double __cdecl exp(double x);
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(exp, sqrt)

struct CEffectFilter {
public:
    float Gain;                                 // +0x00
    float m_a0;                                 // +0x04
    float m_a1;                                 // +0x08
    float m_a2;                                 // +0x0c
    float m_b0;                                 // +0x10
    float m_b1;                                 // +0x14
    float m_b2;                                 // +0x18
    void SetShelves(float gain1, float frequency1, float gain2, float frequency2, unsigned long sampleRate);
};

void CEffectFilter::SetShelves(float gain1, float frequency1, float gain2, float frequency2, unsigned long sampleRate)
{
    if (gain1 > 50.0f)
        gain1 = 50.0f;
    if (gain2 > 50.0f)
        gain2 = 50.0f;
    if (gain1 < 0.00001)
        gain1 = 0.00001f;
    if (gain2 < 0.00001)
        gain2 = 0.00001f;
    float scale = 1.0f;
    float centre = (float)sqrt(frequency1 * frequency2);
    if (gain1 == 1.0f) {
        gain1 = frequency1 = (float)exp((float)sqrt(centre * frequency1) * -3.14159274f / (double)sampleRate);
    } else {
        float root = (float)sqrt(gain1);
        frequency1 = (float)sqrt(centre * frequency1);
        gain1 = (float)exp(frequency1 / root * -3.14159274f / (double)sampleRate);
        frequency1 = (float)exp(frequency1 * root * -3.14159274f / (double)sampleRate);
    }
    if (gain2 == 1.0f) {
        gain2 = frequency2 = (float)exp((float)sqrt(centre * frequency2) * -3.14159274f / (double)sampleRate);
    } else {
        float root = (float)sqrt(gain2);
        float w = (float)sqrt(centre * frequency2);
        gain2 = (float)exp(w * root * -3.14159274f / (double)sampleRate);
        frequency2 = (float)exp(w / root * -3.14159274f / (double)sampleRate);
        scale = -((1.0f - gain2) / (frequency2 - 1.0f));
    }
    m_a0 = 1.0f;
    m_b0 = scale;
    m_a1 = gain2 + gain1;
    m_a2 = -(gain2 * gain1);
    m_b1 = -((frequency1 + frequency2) * scale);
    m_b2 = frequency2 * frequency1 * scale;
}
