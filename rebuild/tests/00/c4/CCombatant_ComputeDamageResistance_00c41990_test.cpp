// Behaviour test for 00c41990 (CCombatant pow-of-clamped-ratio, see the source): the capped flag returns
// 0, otherwise pow(base / clamp(value, min, max), exponent) with each clamp arm exercised.
#include <cstdio>
#include <math.h>

static float g_scale = 2.0f;
struct CCombatant {
    unsigned long m_flags; float m_value, m_max, m_min, m_base, m_exponent, m_spellScale;
    float Compute() const
    {
        if (((unsigned char)(m_flags >> 19) & 1) && m_value >= m_max)
            return 0.0f;
        float clamped;
        if (m_value < m_min) clamped = m_min;
        else if (m_value > m_max) clamped = m_max;
        else clamped = m_value;
        return (float)pow(m_base / clamped, g_scale * m_exponent);
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }

int main()
{
    CCombatant c; c.m_flags = 1ul << 19; c.m_value = 10.0f; c.m_max = 10.0f; c.m_min = 2.0f;
    c.m_base = 8.0f; c.m_exponent = 0.5f; c.m_spellScale = 2.0f / 3.0f;
    float e = 1.0f;                                   // both forms use an exponent of 1.0
    bool ok = c.Compute() == 0.0f;                          // capped
    c.m_flags = 0;
    ok = ok && Near(c.Compute(), (float)pow(8.0f / 10.0f, e));  // value == max: clamped to value
    c.m_value = 1.0f; ok = ok && Near(c.Compute(), (float)pow(8.0f / 2.0f, e));    // below min
    c.m_value = 20.0f; ok = ok && Near(c.Compute(), (float)pow(8.0f / 10.0f, e));  // above max
    c.m_value = 4.0f; c.m_flags = 1ul << 19; ok = ok && Near(c.Compute(), 2.0f);   // flag but below max
    if (!ok) { std::printf("00c41990_TEST FAIL\n"); return 1; }
    std::printf("00c41990_TEST PASS\n");
    return 0;
}