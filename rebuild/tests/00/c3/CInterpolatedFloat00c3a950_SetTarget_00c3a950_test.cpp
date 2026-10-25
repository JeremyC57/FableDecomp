// Behaviour test for 00c3a950: CInterpolatedFloat::SetTarget(target, time) -- forwards to the three-argument SetTarget with a zero arrival rate
#include <cstdio>

struct CInterpolatedFloat {
    float m_current, m_target, m_rate, m_time;
    void SetTarget(float target, float rate, float time) { m_target = target; m_rate = rate; m_time = time; }
    void SetTarget(float target, float time) { SetTarget(target, 0.0f, time); }
};

int main()
{
    CInterpolatedFloat f = { 1.0f, 0.0f, 9.0f, 0.0f };
    f.SetTarget(3.0f, 0.25f);
    if (f.m_target != 3.0f || f.m_rate != 0.0f || f.m_time != 0.25f) { std::printf("00c3a950_TEST FAIL\n"); return 1; }
    std::printf("00c3a950_TEST PASS\n");
    return 0;
}