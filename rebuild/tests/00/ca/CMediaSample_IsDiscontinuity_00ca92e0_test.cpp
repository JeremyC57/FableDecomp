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
    int IsDiscontinuity() { if (m_dwFlags & 0x04) return 0; else return 1; }
};
int main()
{
    Sample s;
    s.m_dwFlags = 0x1FF & ~0x04; if (s.IsDiscontinuity() != 1) { std::printf("00ca92e0_TEST FAIL\n"); return 1; }
    s.m_dwFlags = 0x04; if (s.IsDiscontinuity() != 0) { std::printf("00ca92e0_TEST FAIL\n"); return 1; }
    std::printf("00ca92e0_TEST PASS\n");
    return 0;
}