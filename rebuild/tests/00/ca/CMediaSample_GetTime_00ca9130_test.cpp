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
    int GetTime(long long* ps, long long* pe)
    {
        if (!(m_dwFlags & Sample_StopValid)) {
            if (!(m_dwFlags & Sample_TimeValid)) return (int)0x80040249;
            *ps = m_Start; *pe = m_Start + 1; return 0x00040270;
        }
        *ps = m_Start; *pe = m_End; return 0;
    }
};
int main()
{
    Sample s;
    long long a = -1, b = -1; s.m_Start = 0xFFFFFFFFLL; s.m_End = 0x200000000LL;
    if (s.GetTime(&a, &b) != (int)0x80040249 || a != -1) { std::printf("00ca9130_TEST FAIL\n"); return 1; }
    s.m_dwFlags = 0x10; if (s.GetTime(&a, &b) != 0x00040270 || a != 0xFFFFFFFFLL || b != 0x100000000LL) { std::printf("00ca9130_TEST FAIL\n"); return 1; }
    s.m_dwFlags = 0x110; if (s.GetTime(&a, &b) != 0 || a != 0xFFFFFFFFLL || b != 0x200000000LL) { std::printf("00ca9130_TEST FAIL\n"); return 1; }
    std::printf("00ca9130_TEST PASS\n");
    return 0;
}