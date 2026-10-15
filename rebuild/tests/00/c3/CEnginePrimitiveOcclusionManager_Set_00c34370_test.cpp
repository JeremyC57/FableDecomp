// Behaviour test for 0x00C34370 CEnginePrimitiveOcclusionManager::Set: both arms of the mode test.
#include <cstdio>
struct SOcclusionHead { long a, b, c; bool flag; };
struct SRange { float first, second; void SetFirst(float v) { first = v; } void SetSecond(float v) { second = v; } };
struct SOcclusionFade { float f0, f1, f2, f3; short s0, s1; };
struct SOcclusionSetup { long mode; SOcclusionHead head; float rangeFirst, rangeSecond; long fallback; float secondRangeSecond; long extra; };
struct Mgr {
    SOcclusionHead m_head; long m_mode; SRange m_range; SRange m_secondRange; long m_extra; long m_fallback; SOcclusionFade m_fade;
    void Set(const SOcclusionSetup* setup, const SOcclusionFade* fade)
    {
        if ((m_mode = setup->mode) != 0) {
            m_head = setup->head;
            m_range.SetFirst(setup->rangeFirst); m_range.SetSecond(setup->rangeSecond);
            m_secondRange.SetFirst(0); m_secondRange.SetSecond(setup->secondRangeSecond);
            m_extra = setup->extra;
        } else {
            m_fallback = setup->fallback;
        }
        m_fade = *fade;
    }
};
int main()
{
    Mgr m; m.m_extra = -5; m.m_fallback = -6; m.m_range.first = 9;
    SOcclusionSetup s; s.mode = 2; s.head.a = 11; s.head.b = 12; s.head.c = 13; s.head.flag = true;
    s.rangeFirst = 1.5f; s.rangeSecond = 2.5f; s.fallback = 77; s.secondRangeSecond = 3.5f; s.extra = 88;
    SOcclusionFade f; f.f0 = 0.1f; f.f1 = 0.2f; f.f2 = 0.3f; f.f3 = 0.4f; f.s0 = 5; f.s1 = 6;
    m.Set(&s, &f);
    bool ok = m.m_mode == 2 && m.m_head.b == 12 && m.m_head.flag && m.m_range.first == 1.5f && m.m_range.second == 2.5f
        && m.m_secondRange.first == 0 && m.m_secondRange.second == 3.5f && m.m_extra == 88 && m.m_fallback == -6 && m.m_fade.s1 == 6;
    s.mode = 0; s.extra = 1; m.m_extra = -5; m.Set(&s, &f);
    ok = ok && m.m_mode == 0 && m.m_fallback == 77 && m.m_extra == -5;
    if (!ok) { std::printf("00c34370_TEST FAIL\n"); return 1; }
    std::printf("00c34370_TEST PASS\n");
    return 0;
}