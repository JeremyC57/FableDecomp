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
    int GetMediaType(AMT** pp)
    {
        if (!(m_dwFlags & Sample_TypeChanged)) { *pp = 0; return 1; }
        *pp = CreateMediaType(m_pMediaType);
        if (*pp == 0) return (int)0x8007000E;
        return 0;
    }
};
int main()
{
    Sample s;
    AMT t; t.x = 42; AMT* p = &t; s.m_pMediaType = &t;
    if (s.GetMediaType(&p) != 1 || p != 0) { std::printf("00ca93b0_TEST FAIL\n"); return 1; }
    s.m_dwFlags = 8; if (s.GetMediaType(&p) != 0 || p != &gCopy || gCopy.x != 42) { std::printf("00ca93b0_TEST FAIL\n"); return 1; }
    gFail = 1; if (s.GetMediaType(&p) != (int)0x8007000E || p != 0) { std::printf("00ca93b0_TEST FAIL\n"); return 1; }
    std::printf("00ca93b0_TEST PASS\n");
    return 0;
}