#include <cstdio>
#include <cmath>
static int g_next;
static int FakeRand() { return g_next; }
struct CRandomDelay_00c2d100 {
    long m_base; float m_period;
    long PickDelay(float rate)
    {
        rate *= 10.0f;
        rate /= m_period;
        return m_base + (long)(std::log10(1.0 - FakeRand() * 3.0518509e-05f) / std::log10(1.0 - rate)) + 1;
    }
};
int main()
{
    CRandomDelay_00c2d100 d; d.m_base = 100; d.m_period = 10.0f;
    g_next = 0;                       // log10(1) = 0 -> base + 1
    if (d.PickDelay(0.5f) != 101) { std::printf("00c2d100_TEST FAIL\n"); return 1; }
    g_next = 24575;                   // 1 - 0.75 = 0.25 = 0.5^2 with rate 0.5 -> 2
    long v = d.PickDelay(0.5f);
    if (v < 102 || v > 103) { std::printf("00c2d100_TEST FAIL\n"); return 1; }
    std::printf("00c2d100_TEST PASS\n");
    return 0;
}
