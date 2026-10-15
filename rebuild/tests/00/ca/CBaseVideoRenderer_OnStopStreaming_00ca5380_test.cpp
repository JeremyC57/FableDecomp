// Behaviour test for CBaseVideoRenderer::OnStopStreaming @ 0x00CA5380: start time becomes elapsed time, returns NOERROR (0).
#include <cstdio>
static unsigned long g_now;
static unsigned long FakeTimeGetTime() { return g_now; }
struct CBaseVideoRenderer {
    int m_tStreamingStart;
    long OnStopStreaming() { m_tStreamingStart = FakeTimeGetTime() - m_tStreamingStart; return 0; }
};
int main()
{
    CBaseVideoRenderer r; r.m_tStreamingStart = 1000; g_now = 4500;
    if (r.OnStopStreaming() != 0 || r.m_tStreamingStart != 3500) { std::printf("00ca5380_TEST FAIL\n"); return 1; }
    std::printf("00ca5380_TEST PASS\n");
    return 0;
}