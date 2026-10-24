// Behaviour test for 00c1bda0: GetAttenuation -- 0 at or beyond the far distance, 1 at or inside the near distance, else ((far - d) / (far - near)) ^ exponent
#include <cstdio>
#include <cmath>

float g_falloffExponent = 2.0f;

struct CDistanceFalloff {
    float m_near;
    float m_far;
    float GetAttenuation(float distance)
    {
        float span = m_far - m_near;
        float remaining = m_far - distance;
        if (remaining <= 0.0f)
            return 0.0f;
        if (remaining >= span)
            return 1.0f;
        return (float)std::pow((double)(remaining / span), (double)g_falloffExponent);
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    CDistanceFalloff f = { 10.0f, 50.0f };
    if (!Near(f.GetAttenuation(50.0f), 0.0f) || !Near(f.GetAttenuation(80.0f), 0.0f)) { std::printf("00c1bda0_TEST FAIL\n"); return 1; }
    if (!Near(f.GetAttenuation(10.0f), 1.0f) || !Near(f.GetAttenuation(2.0f), 1.0f)) { std::printf("00c1bda0_TEST FAIL\n"); return 1; }
    if (!Near(f.GetAttenuation(30.0f), 0.25f)) { std::printf("00c1bda0_TEST FAIL\n"); return 1; }      // half-way, squared
    g_falloffExponent = 1.0f;
    if (!Near(f.GetAttenuation(40.0f), 0.25f)) { std::printf("00c1bda0_TEST FAIL\n"); return 1; }      // linear
    std::printf("00c1bda0_TEST PASS\n");
    return 0;
}