#include <cstdio>
struct IMediaSample;
static unsigned long g_now = 4242; static int g_late = -1, g_frame = -1;
static unsigned long FakeTimeGetTime() { return g_now; }
struct CBaseVideoRenderer {
    virtual void RecordFrameLateness(int trLate, int trFrame) { g_late = trLate; g_frame = trFrame; }
    unsigned char _pad_0x04[0xf8]; long m_tRenderStart; unsigned char _pad_0x100[0x50]; int m_trLate; int m_trFrame;
    void OnRenderStart(IMediaSample* pMediaSample) { RecordFrameLateness(m_trLate, m_trFrame); m_tRenderStart = FakeTimeGetTime(); }
};
int main()
{
    CBaseVideoRenderer r; r.m_trLate = 17; r.m_trFrame = 33; r.m_tRenderStart = 0;
    r.OnRenderStart(0);
    if (g_late != 17 || g_frame != 33 || r.m_tRenderStart != 4242) { std::printf("00ca55e0_TEST FAIL\n"); return 1; }
    std::printf("00ca55e0_TEST PASS\n"); return 0;
}