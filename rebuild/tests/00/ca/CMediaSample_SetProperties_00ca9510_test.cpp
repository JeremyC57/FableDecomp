// Behaviour test for CMediaSample::SetProperties (0x00CA9510): under 4 bytes nothing happens (S_OK); a null buffer is
// E_POINTER; bad flags / a time flag without tStop / a foreign buffer or size / lActual over cbBuffer are E_INVALIDARG;
// a full block stores stream id, flags (keeping MediaTimeValid), type flags, lActual, times and the copied media type.
#include <cstdio>
#include <cstring>

typedef long HR;
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
struct MT { int majortype; };
struct Props {                                  // AM_SAMPLE2_PROPERTIES, 0x30 bytes on x86
    unsigned int cbData; unsigned int dwTypeSpecificFlags; unsigned int dwSampleFlags; int lActual;
    i64 tStart; i64 tStop; unsigned int dwStreamId; MT* pMediaType; unsigned char* pbBuffer; int cbBuffer;
};
static MT g_copy;
static int g_deleted;
static MT* CreateMediaType(const MT* p) { g_copy = *p; return &g_copy; }
static void DeleteMediaType(MT*) { ++g_deleted; }

struct Sample {
    unsigned int m_dwFlags, m_dwTypeSpecificFlags; unsigned char* m_pBuffer; int m_lActual, m_cbBuffer;
    i64 m_Start, m_End; MT* m_pMediaType; unsigned int m_dwStreamId;
    // field offsets inside the 0x30-byte x86 AM_SAMPLE2_PROPERTIES (pointers are 4 bytes there)
    HR SetProperties(unsigned int cb, const Props* p)
    {
        MT* pmt = 0;
        if (cb >= 4) {
            if (p == 0) return (HR)0x80004003L;
            if (p->cbData < cb) cb = p->cbData;
            if (cb > 0x30 || p->cbData > 0x30) return (HR)0x80070057L;
            if (cb >= 0xC) {
                if (p->dwSampleFlags & 0xFFFFFE20u) return (HR)0x80070057L;
                if ((p->dwSampleFlags & 0x10) && !(m_dwFlags & 0x10) && !(cb >= 0x20)) return (HR)0x80070057L;
            }
            if (cb >= 0x2C && p->pbBuffer != 0 && p->pbBuffer != m_pBuffer) return (HR)0x80070057L;
            if (cb >= 0x30 && p->cbBuffer != 0 && p->cbBuffer != m_cbBuffer) return (HR)0x80070057L;
            if (cb >= 0x30 && cb >= 0x10 && p->cbBuffer < p->lActual) return (HR)0x80070057L;
            if (cb >= 0x28 && (p->dwSampleFlags & 8)) {
                if (p->pMediaType == 0) return (HR)0x80004003L;
                pmt = CreateMediaType(p->pMediaType);
                if (pmt == 0) return (HR)0x8007000EL;
            }
            if (cb >= 0x24) m_dwStreamId = p->dwStreamId;
            if (cb >= 0xC) { m_dwFlags = p->dwSampleFlags | (m_dwFlags & 0x20); m_dwTypeSpecificFlags = p->dwTypeSpecificFlags; }
            else if (cb >= 8) m_dwTypeSpecificFlags = p->dwTypeSpecificFlags;
            if (cb >= 0x10) m_lActual = p->lActual;
            if (cb >= 0x20) m_End = p->tStop;
            if (cb >= 0x18) m_Start = p->tStart;
            if (cb >= 0x28 && (p->dwSampleFlags & 8)) { if (m_pMediaType) DeleteMediaType(m_pMediaType); m_pMediaType = pmt; }
            if (m_pMediaType) m_dwFlags |= 8; else m_dwFlags &= ~8u;
        }
        return 0;
    }
};

int main()
{
    int ok = 1;
    unsigned char buf[16];
    MT old = { 1 }, nt = { 7 };
    Sample s; std::memset(&s, 0, sizeof(s));
    s.m_pBuffer = buf; s.m_cbBuffer = 16; s.m_dwFlags = 0x20; s.m_pMediaType = &old;
    Props p; std::memset(&p, 0, sizeof(p));
    ok &= s.SetProperties(3, 0) == 0;
    ok &= s.SetProperties(4, 0) == (HR)0x80004003L;
    p.cbData = 0x30; p.dwSampleFlags = 0x20;
    ok &= s.SetProperties(0x30, &p) == (HR)0x80070057L;      // MediaTimeValid may not be set
    p.dwSampleFlags = 0x10; p.cbData = 0x18;
    ok &= s.SetProperties(0x30, &p) == (HR)0x80070057L;      // time flag without tStop (cb clamps to 0x18)
    p.cbData = 0x30; p.dwSampleFlags = 0x19; p.pbBuffer = buf + 1;
    ok &= s.SetProperties(0x30, &p) == (HR)0x80070057L;      // foreign buffer
    p.pbBuffer = 0; p.cbBuffer = 16; p.lActual = 17;
    ok &= s.SetProperties(0x30, &p) == (HR)0x80070057L;      // lActual over the size
    p.lActual = 12; p.tStart = 100; p.tStop = 200; p.dwStreamId = 5; p.dwTypeSpecificFlags = 3; p.pMediaType = &nt;
    ok &= s.SetProperties(0x30, &p) == 0;
    ok &= s.m_dwFlags == (0x19u | 0x20u) && s.m_dwTypeSpecificFlags == 3 && s.m_lActual == 12;
    ok &= s.m_Start == 100 && s.m_End == 200 && s.m_dwStreamId == 5 && s.m_pMediaType == &g_copy && g_copy.majortype == 7;
    ok &= g_deleted == 1;
    if (!ok) { std::printf("00ca9510_TEST FAIL\n"); return 1; }
    std::printf("00ca9510_TEST PASS\n");
    return 0;
}