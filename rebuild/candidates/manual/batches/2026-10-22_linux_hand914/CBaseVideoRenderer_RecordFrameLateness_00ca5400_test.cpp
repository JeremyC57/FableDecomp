// Behaviour test for 00ca5400 CBaseVideoRenderer::RecordFrameLateness (strmbase renbase.cpp quality statistics)
#include <cstdio>
#define FRAMES_TO_IGNORE 1
#ifdef _MSC_VER
typedef __int64 I64;
#else
typedef long long I64;
#endif
class CBaseVideoRenderer {
public:
    int m_cFramesDrawn;
    I64 m_iTotAcc, m_iSumSqAcc, m_iSumSqFrameTime, m_iSumFrameTime;
    CBaseVideoRenderer() : m_cFramesDrawn(0), m_iTotAcc(0), m_iSumSqAcc(0), m_iSumSqFrameTime(0), m_iSumFrameTime(0) {}
    void RecordFrameLateness(int trLate, int trFrame);
};
void CBaseVideoRenderer::RecordFrameLateness(int trLate, int trFrame)
{
    int tLate = trLate / 10000;
    if (tLate > 1000 || tLate < -1000) {
        if (m_cFramesDrawn <= FRAMES_TO_IGNORE) tLate = 0;
        else if (tLate > 0) tLate = 1000;
        else tLate = -1000;
    }
    if (m_cFramesDrawn > FRAMES_TO_IGNORE) {
        m_iTotAcc += tLate;
        m_iSumSqAcc += (tLate * tLate);
    }
    if (m_cFramesDrawn > FRAMES_TO_IGNORE + 1) {
        int tInterFrame = trFrame / 10000;
        if (tInterFrame > 1000 || tInterFrame < 0) tInterFrame = 1000;
        m_iSumSqFrameTime += tInterFrame * tInterFrame;
        m_iSumFrameTime += tInterFrame;
    }
    ++m_cFramesDrawn;
}
int main()
{
    CBaseVideoRenderer r;
    r.RecordFrameLateness(50000, 400000);      // frame 0: ignored
    r.RecordFrameLateness(-30000000, 400000);  // frame 1: ignored
    if (r.m_iTotAcc != 0 || r.m_iSumFrameTime != 0 || r.m_cFramesDrawn != 2) { std::printf("00ca5400_TEST FAIL\n"); return 1; }
    r.RecordFrameLateness(50000, 400000);      // late 5 ms counted; inter-frame needs a third drawn frame
    if (r.m_iTotAcc != 5 || r.m_iSumSqAcc != 25 || r.m_iSumFrameTime != 0) { std::printf("00ca5400_TEST FAIL\n"); return 1; }
    r.RecordFrameLateness(-20000000, -10000);  // late clamped to -1000, inter-frame negative -> 1000
    if (r.m_iTotAcc != -995 || r.m_iSumSqAcc != 1000025 || r.m_iSumFrameTime != 1000 || r.m_iSumSqFrameTime != 1000000) { std::printf("00ca5400_TEST FAIL\n"); return 1; }
    r.RecordFrameLateness(20000000, 0);        // clamped to +1000
    if (r.m_iTotAcc != 5 || r.m_cFramesDrawn != 5) { std::printf("00ca5400_TEST FAIL\n"); return 1; }
    std::printf("00ca5400_TEST PASS\n");
    return 0;
}
