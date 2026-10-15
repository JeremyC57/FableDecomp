#include <cstdio>
#include <new>
// Behaviour test for CInterpolatedFloat::CInterpolatedFloat (0x00C3A970): the callees modelled after
// the retail bodies (Reset writes current/target; SetTarget records the target and time).
struct CInterpolatedFloat {
    float m_current;
    float m_target;
    float m_time;
    unsigned char _pad_0x0c[0x24];
    bool m_isAngle;
    int m_calls;
    CInterpolatedFloat(float target, float time);
    void Reset(float value) { m_calls = m_isAngle ? 100 : 1; m_current = value; m_target = value; }
    void SetTarget(float target, float time) { m_calls = m_calls * 10 + 2; m_target = target; m_time = time; }
};
CInterpolatedFloat::CInterpolatedFloat(float target, float time)
    : m_isAngle(false)
{
    Reset(0.0f);
    SetTarget(target, time);
}
int main()
{
    static unsigned char raw[sizeof(CInterpolatedFloat)];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xFF;
    CInterpolatedFloat* f = new (raw) CInterpolatedFloat(2.5f, 0.75f);
    if (f->m_isAngle || f->m_calls != 12 || f->m_current != 0.0f || f->m_target != 2.5f || f->m_time != 0.75f)
    { std::printf("00c3a970_TEST FAIL\n"); return 1; }
    std::printf("00c3a970_TEST PASS\n");
    return 0;
}