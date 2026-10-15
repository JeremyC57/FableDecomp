#include <cstdio>
typedef long long REFERENCE_TIME;
struct IMediaSample {
    long hr; REFERENCE_TIME s, e;
    long GetTime(REFERENCE_TIME* start, REFERENCE_TIME* end) { if (hr >= 0) { *start = s; *end = e; } return hr; }
};
static int g_should = 0;
struct CBaseRenderer {
    void* m_pClock;
    virtual long ShouldDrawSampleNow(IMediaSample*, REFERENCE_TIME*, REFERENCE_TIME*) { ++g_should; return 1; }
    virtual ~CBaseRenderer() { }
    long GetSampleTimes(IMediaSample* pMediaSample, REFERENCE_TIME* pStartTime, REFERENCE_TIME* pEndTime)
    {
        if (pMediaSample->GetTime(pStartTime, pEndTime) >= 0) {
            if (*pEndTime < *pStartTime)
                return (long)0x80040228L;
        } else {
            return 0;
        }
        if (m_pClock == 0)
            return 0;
        return ShouldDrawSampleNow(pMediaSample, pStartTime, pEndTime);
    }
};
int main()
{
    CBaseRenderer r; r.m_pClock = 0;
    IMediaSample m; REFERENCE_TIME a, b;
    m.hr = -1; bool ok = r.GetSampleTimes(&m, &a, &b) == 0;
    m.hr = 0; m.s = 0x100000005LL; m.e = 0x100000004LL; ok = ok && r.GetSampleTimes(&m, &a, &b) == (long)0x80040228L;
    m.e = 0x100000005LL; ok = ok && r.GetSampleTimes(&m, &a, &b) == 0 && g_should == 0;
    r.m_pClock = &r; ok = ok && r.GetSampleTimes(&m, &a, &b) == 1 && g_should == 1;
    if (!ok) { std::printf("00ca49f0_TEST FAIL\n"); return 1; }
    std::printf("00ca49f0_TEST PASS\n");
    return 0;
}