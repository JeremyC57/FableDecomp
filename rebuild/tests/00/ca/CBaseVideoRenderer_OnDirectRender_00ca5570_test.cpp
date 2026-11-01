// Behaviour test: CBaseVideoRenderer::OnDirectRender (0x00CA5570) -- reset the render averages, record the frame lateness
// through the virtual RecordFrameLateness(m_trLate, m_trFrame), then throttle (Sleep(m_trThrottle/10000) or Sleep(0)).
#include <cstdio>
static unsigned long g_slept = 12345;
static int g_sleepCalls = 0;
static void Sleep(unsigned long ms) { g_slept = ms; ++g_sleepCalls; }
class CBaseVideoRenderer {
public:
    int m_trThrottle, m_trRenderAvg, m_trRenderLast, m_trLate, m_trFrame;
    int recLate, recFrame, recCalls;
    CBaseVideoRenderer() : m_trThrottle(0), m_trRenderAvg(7), m_trRenderLast(9), m_trLate(0), m_trFrame(0),
                           recLate(0), recFrame(0), recCalls(0) { }
    virtual ~CBaseVideoRenderer() { }
    virtual void RecordFrameLateness(int trLate, int trFrame) { recLate = trLate; recFrame = trFrame; ++recCalls; }
    void ThrottleWait()
    {
        if (m_trThrottle > 0) {
            int iThrottle = m_trThrottle / 10000;
            Sleep(iThrottle);
        } else {
            Sleep(0);
        }
    }
    virtual void OnDirectRender(void* pMediaSample)
    {
        m_trRenderAvg = 0;
        m_trRenderLast = 5000000;
        RecordFrameLateness(m_trLate, m_trFrame);
        ThrottleWait();
    }
};
int main()
{
    CBaseVideoRenderer r;
    r.m_trThrottle = 250000; r.m_trLate = -40000; r.m_trFrame = 333333;
    r.OnDirectRender(0);
    if (r.m_trRenderAvg != 0 || r.m_trRenderLast != 5000000 || r.recCalls != 1 || r.recLate != -40000 ||
        r.recFrame != 333333 || g_slept != 25 || g_sleepCalls != 1) { std::printf("00ca5570_TEST FAIL\n"); return 1; }
    r.m_trThrottle = -5;
    r.OnDirectRender(0);
    if (g_slept != 0 || g_sleepCalls != 2 || r.recCalls != 2) { std::printf("00ca5570_TEST FAIL\n"); return 1; }
    r.m_trThrottle = 9999;                       // under one millisecond: Sleep(0) through the division
    r.OnDirectRender(0);
    if (g_slept != 0 || g_sleepCalls != 3) { std::printf("00ca5570_TEST FAIL\n"); return 1; }
    std::printf("00ca5570_TEST PASS\n");
    return 0;
}