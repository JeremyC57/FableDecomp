#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_calc = 0;
struct CFrameRateSmoother { unsigned char _pad[8]; float* m_pSamples; long m_c; long m_10; long m_capacity; long m_count; long m_1c; long m_start; long m_next; double m_smoothedTime; void CalculateSmoothedTime() { ++g_calc; }
    void AddFrameTime(double t) { if (m_count == m_capacity) { m_start = (m_start + 1) % m_capacity; --m_count; } m_pSamples[m_next] = (float)t; m_next = (m_next + 1) % m_capacity; ++m_count; CalculateSmoothedTime(); } };
int main() {
    float samples[4] = { 0, 0, 0, 0 }; CFrameRateSmoother s; std::memset(&s, 0, sizeof s); s.m_pSamples = samples; s.m_capacity = 4;
    s.AddFrameTime(1.5); if (samples[0] != 1.5f || s.m_next != 1 || s.m_count != 1 || s.m_start != 0 || g_calc != 1) { std::printf("0049b9e0_TEST FAIL\n"); return 1; }
    s.AddFrameTime(2.0); s.AddFrameTime(3.0); s.AddFrameTime(4.0); if (s.m_count != 4 || s.m_next != 0) { std::printf("0049b9e0_TEST FAIL\n"); return 1; }
    s.AddFrameTime(5.0); if (samples[0] != 5.0f || s.m_start != 1 || s.m_count != 4 || s.m_next != 1 || g_calc != 5) { std::printf("0049b9e0_TEST FAIL\n"); return 1; }
    std::printf("0049b9e0_TEST PASS\n"); return 0;
}