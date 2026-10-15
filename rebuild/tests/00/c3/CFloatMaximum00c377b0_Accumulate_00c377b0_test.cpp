#include <cstdio>
struct CFloatMaximum_00c377b0 {
    float m_max;
    void Accumulate(float value) { if (value > m_max) m_max = value; }
};
int main()
{
    CFloatMaximum_00c377b0 m; m.m_max = 1.5f;
    m.Accumulate(1.0f); m.Accumulate(1.5f);
    if (m.m_max != 1.5f) { std::printf("00c377b0_TEST FAIL\n"); return 1; }
    m.Accumulate(4.25f); m.Accumulate(-2.0f);
    if (m.m_max != 4.25f) { std::printf("00c377b0_TEST FAIL\n"); return 1; }
    std::printf("00c377b0_TEST PASS\n");
    return 0;
}