// Behaviour test for 0x00C0B180 MultiplyMaskedParams: multiplies value by each masked parameter's GetValue() (an angle parameter wrapped into
// [-pi, pi) with 3.14159265f / 6.28318531f / -3.14159265f, retail 0x0128F604 / 0x0128F608 / 0x012B1EB8), stopping once value == 0.0f (0x0122DEDC).
// NF418 (2026-11-25): linked behaviour test: the row function is declared here, not defined, so the test links the row object (and
// defines its callees under the row's spellings); it asserts the value of the float literal the row now spells (a mutated literal fails).
#include <cstdio>
struct CRampedParam {
    float m_value; float m_target; float m_targetRate; float m_rate; float m_10; float m_time; float m_duration; float m_coef[5]; bool m_bAngle;
    float GetValue() const;
};
float __fastcall MultiplyMaskedParams(const CRampedParam* params, unsigned long mask, float value, unsigned long count);
static bool near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }
int main()
{
    static CRampedParam p[4];
    for (int i = 0; i < 4; ++i) { p[i].m_value = 1.0f; p[i].m_bAngle = false; }
    p[0].m_value = 2.0f; p[1].m_value = 3.0f; p[2].m_value = 5.0f;
    float a = MultiplyMaskedParams(p, 0x5, 1.0f, 4);                 // p0 * p2 = 10
    p[1].m_value = 4.0f; p[1].m_bAngle = true;                      // 4 >= pi -> 4 - 2pi
    float b = MultiplyMaskedParams(p, 0x2, 1.0f, 4);
    p[1].m_value = -4.0f;                                           // -4 < -pi -> -4 + 2pi
    float c = MultiplyMaskedParams(p, 0x2, 1.0f, 4);
    p[1].m_value = 3.0f;                                            // inside [-pi, pi): unchanged
    float d = MultiplyMaskedParams(p, 0x2, 1.0f, 4);
    p[0].m_value = 0.5f; p[2].m_value = 4.0f;                        // 0.5 is not zero: the loop goes on to p2 (0.5 * 4 = 2)
    float f = MultiplyMaskedParams(p, 0x5, 1.0f, 4);
    float inf = 1e38f; inf *= 10.0f;
    p[0].m_value = 0.0f; p[2].m_value = inf;                        // value becomes 0 after p0: the loop stops (0 * inf would be NaN)
    float e = MultiplyMaskedParams(p, 0x5, 1.0f, 4);
    if (!near(a, 10.0f) || !near(b, 4.0f - 6.28318531f) || !near(c, -4.0f + 6.28318531f) || !near(d, 3.0f) || e != 0.0f || !near(f, 2.0f)) {
        std::printf("BAD %f %f %f %f %f %f\n", a, b, c, d, e, f); return 1;
    }
    std::printf("00c0b180_TEST PASS\n");
    return 0;
}
