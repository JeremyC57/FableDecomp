#include <cstdio>
typedef long long REFERENCE_TIME;
struct IReferenceClock { long hr; REFERENCE_TIME now; long GetTime(REFERENCE_TIME* p) { if (hr >= 0) *p = now; return hr; } };
struct CRefTime { REFERENCE_TIME m_time; CRefTime& operator-=(const CRefTime& rt) { m_time -= rt.m_time; return *this; } };
struct CBaseFilter {
    IReferenceClock* m_pClock; CRefTime m_tStart;
    long StreamTime(CRefTime& rtStream)
    {
        if (m_pClock == 0) return (long)0x80040213L;
        long hr = m_pClock->GetTime((REFERENCE_TIME*)&rtStream);
        if (hr < 0) return hr;
        rtStream -= m_tStart;
        return 0;
    }
};
int main()
{
    CBaseFilter f; f.m_pClock = 0; f.m_tStart.m_time = 0x100000010LL;
    CRefTime t; t.m_time = 0;
    bool ok = f.StreamTime(t) == (long)0x80040213L;
    IReferenceClock c; c.hr = 0; c.now = 0x200000005LL; f.m_pClock = &c;
    ok = ok && f.StreamTime(t) == 0 && t.m_time == 0xFFFFFFF5LL;
    c.hr = -5; t.m_time = 7; ok = ok && f.StreamTime(t) == -5 && t.m_time == 7;
    if (!ok) { std::printf("00ca77a0_TEST FAIL\n"); return 1; }
    std::printf("00ca77a0_TEST PASS\n");
    return 0;
}
