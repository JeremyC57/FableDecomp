#include <cstdio>
#include <cstring>
// CMediaSample::QueryInterface: IMediaSample / IMediaSample2 / IUnknown -> GetInterface(this), else E_NOINTERFACE.
struct GUID { unsigned d1; unsigned short d2, d3; unsigned char d4[8]; };
static bool eq(const GUID& a, const GUID& b) { return !std::memcmp(&a, &b, sizeof(GUID)); }
static const GUID IID_IMediaSample = { 0x56A8689A, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
static const GUID IID_IMediaSample2 = { 0x36B73884, 0xC2C8, 0x11CF, { 0x8B, 0x46, 0x00, 0x80, 0x5F, 0x6C, 0xEF, 0x60 } };
static const GUID IID_IUnknown = { 0, 0, 0, { 0xC0, 0, 0, 0, 0, 0, 0, 0x46 } };
static int gRefs;
static int GetInterface(void* p, void** ppv) { *ppv = p; ++gRefs; return 0; }
struct Sample {
    int QueryInterface(const GUID& riid, void** ppv)
    {
        if (eq(riid, IID_IMediaSample) || eq(riid, IID_IMediaSample2) || eq(riid, IID_IUnknown)) return GetInterface(this, ppv);
        else return (int)0x80004002;
    }
};
int main()
{
    Sample s; void* v = 0; GUID g = IID_IMediaSample; g.d4[7] = 0;
    if (s.QueryInterface(IID_IMediaSample, &v) != 0 || v != &s) { std::printf("00ca9000_TEST FAIL\n"); return 1; }
    v = 0; if (s.QueryInterface(IID_IMediaSample2, &v) != 0 || v != &s) { std::printf("00ca9000_TEST FAIL\n"); return 1; }
    v = 0; if (s.QueryInterface(IID_IUnknown, &v) != 0 || v != &s || gRefs != 3) { std::printf("00ca9000_TEST FAIL\n"); return 1; }
    v = 0; if (s.QueryInterface(g, &v) != (int)0x80004002 || v != 0 || gRefs != 3) { std::printf("00ca9000_TEST FAIL\n"); return 1; }
    std::printf("00ca9000_TEST PASS\n");
    return 0;
}