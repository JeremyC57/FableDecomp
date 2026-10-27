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
    int GetMediaTime(long long* ps, long long* pe)
    {
        if (!(m_dwFlags & Sample_MediaTimeValid)) return (int)0x80040251;
        *ps = m_MediaStart; *pe = m_MediaStart + m_MediaEnd; return 0;
    }
};
int main()
{
    Sample s;
    long long a = 0, b = 0; s.m_MediaStart = 0xFFFFFFF0LL; s.m_MediaEnd = 0x20;
    if (s.GetMediaTime(&a, &b) != (int)0x80040251) { std::printf("00ca9210_TEST FAIL\n"); return 1; }
    s.m_dwFlags = 0x20; if (s.GetMediaTime(&a, &b) != 0 || a != 0xFFFFFFF0LL || b != 0x100000010LL) { std::printf("00ca9210_TEST FAIL\n"); return 1; }
    s.m_MediaEnd = -0x10; if (s.GetMediaTime(&a, &b) != 0 || b != 0xFFFFFFE0LL) { std::printf("00ca9210_TEST FAIL\n"); return 1; }
    std::printf("00ca9210_TEST PASS\n");
    return 0;
}