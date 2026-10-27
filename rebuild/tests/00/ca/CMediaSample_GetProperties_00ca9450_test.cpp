// Behaviour test for 0x00CA9450 CMediaSample::GetProperties (standalone copy of the logic)
#include <cstdio>
#include <cstring>
typedef int HRESULT;
struct Props { unsigned cbData, dwTypeSpecificFlags, dwSampleFlags; int lActual; long long tStart, tStop; unsigned dwStreamId; void* pMediaType; unsigned char* pbBuffer; int cbBuffer; };
struct Sample {
    unsigned m_dwFlags, m_dwTypeSpecificFlags; unsigned char* m_pBuffer; int m_lActual; long long m_Start, m_End; int m_cbBuffer; unsigned m_dwStreamId; void* m_pMediaType;
    HRESULT GetProperties(unsigned cb, unsigned char* out)
    {
        if (cb != 0) {
            if (out == 0) return (HRESULT)0x80004003L;
            Props p;
            p.cbData = cb < sizeof(p) ? cb : (unsigned)sizeof(p);
            p.dwSampleFlags = m_dwFlags & ~0x100u;
            p.dwTypeSpecificFlags = m_dwTypeSpecificFlags;
            p.pbBuffer = m_pBuffer; p.cbBuffer = m_cbBuffer; p.lActual = m_lActual;
            p.tStart = m_Start; p.tStop = m_End; p.dwStreamId = m_dwStreamId;
            p.pMediaType = (m_dwFlags & 8) ? m_pMediaType : 0;
            std::memcpy(out, &p, p.cbData);
        }
        return 0;
    }
};
int main()
{
    int mt = 0; unsigned char buf[4];
    Sample s = { 0x108, 5, buf, 77, 10, 20, 4, 2, &mt };
    if (s.GetProperties(0, 0) != 0 || s.GetProperties(4, 0) != (HRESULT)0x80004003L) { std::printf("00ca9450_TEST FAIL\n"); return 1; }
    Props p; std::memset(&p, 0xAB, sizeof(p));
    if (s.GetProperties(1000, (unsigned char*)&p) != 0 || p.cbData != sizeof(Props) || p.dwSampleFlags != 8 || p.pMediaType != &mt || p.tStop != 20 || p.lActual != 77) { std::printf("00ca9450_TEST FAIL\n"); return 1; }
    s.m_dwFlags = 0x100; std::memset(&p, 0xAB, sizeof(p));
    if (s.GetProperties(8, (unsigned char*)&p) != 0 || p.cbData != 8 || p.dwTypeSpecificFlags != 5 || p.dwSampleFlags != 0xABABABABu) { std::printf("00ca9450_TEST FAIL\n"); return 1; }
    std::printf("00ca9450_TEST PASS\n");
    return 0;
}