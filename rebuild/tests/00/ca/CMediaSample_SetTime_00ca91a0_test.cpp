#include <cstdio>
struct AMT { int x; };
static AMT gCopy; static int gFail;
static AMT* CreateMediaType(const AMT* p) { if (gFail) return 0; gCopy = *p; return &gCopy; }
struct Sample {
    enum { Sample_SyncPoint = 0x01, Sample_Preroll = 0x02, Sample_Discontinuity = 0x04, Sample_TypeChanged = 0x08,
           Sample_TimeValid = 0x10, Sample_MediaTimeValid = 0x20, Sample_StopValid = 0x100 };
    unsigned m_dwFlags; unsigned char* m_pBuffer; int m_lActual; int m_cbBuffer;
    long long m_Start, m_End, m_MediaStart; int m_MediaEnd; AMT* m_pMediaType;
    Sample() : m_dwFlags(0), m_pBuffer(0), m_lActual(0), m_cbBuffer(0), m_Start(0), m_End(0), m_MediaStart(0), m_MediaEnd(0), m_pMediaType(0) {}
    int SetTime(long long* ps, long long* pe)
    {
        if (ps == 0) m_dwFlags &= ~(Sample_TimeValid | Sample_StopValid);
        else if (pe == 0) { m_Start = *ps; m_dwFlags |= Sample_TimeValid; m_dwFlags &= ~Sample_StopValid; }
        else { m_Start = *ps; m_End = *pe; m_dwFlags |= Sample_TimeValid | Sample_StopValid; }
        return 0;
    }
};
int main()
{
    Sample s;
    long long a = 5, b = 9; s.m_dwFlags = 0x1;
    if (s.SetTime(&a, &b) != 0 || s.m_Start != 5 || s.m_End != 9 || s.m_dwFlags != 0x111) { std::printf("00ca91a0_TEST FAIL\n"); return 1; }
    a = 7; if (s.SetTime(&a, 0) != 0 || s.m_Start != 7 || s.m_dwFlags != 0x11) { std::printf("00ca91a0_TEST FAIL\n"); return 1; }
    if (s.SetTime(0, 0) != 0 || s.m_dwFlags != 0x1) { std::printf("00ca91a0_TEST FAIL\n"); return 1; }
    std::printf("00ca91a0_TEST PASS\n");
    return 0;
}