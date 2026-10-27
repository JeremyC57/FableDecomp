#include <cstdio>
#include <cstring>
// CPosPassThru::NonDelegatingQueryInterface: E_POINTER for null ppv; *ppv = 0; IMediaSeeking -> the seeking base;
// IMediaPosition -> the position base (inlined CMediaPosition NDQI); else CUnknown's.
struct GUID { unsigned d1; unsigned short d2, d3; unsigned char d4[8]; };
static bool eq(const GUID& a, const GUID& b) { return !std::memcmp(&a, &b, sizeof(GUID)); }
static const GUID IID_IMediaSeeking = { 0x36B73880, 0xC2C8, 0x11CF, { 0x8B, 0x46, 0x00, 0x80, 0x5F, 0x6C, 0xEF, 0x60 } };
static const GUID IID_IMediaPosition = { 0x56A868B2, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
static int GetInterface(void* p, void** ppv) { *ppv = p; return 0; }
struct ISeek { int s; }; struct IPos { int p; };
struct Unknown { int u; int NDQI(const GUID&, void**) { return (int)0x80004002; } };
struct MediaPosition : IPos, Unknown {
    int NDQI(const GUID& r, void** ppv) { if (eq(r, IID_IMediaPosition)) return GetInterface((IPos*)this, ppv); return Unknown::NDQI(r, ppv); }
};
struct PosPassThru : ISeek, MediaPosition {
    int NDQI(const GUID& r, void** ppv)
    {
        if (ppv == 0) return (int)0x80004003;
        *ppv = 0;
        if (eq(r, IID_IMediaSeeking)) return GetInterface(static_cast<ISeek*>(this), ppv);
        return MediaPosition::NDQI(r, ppv);
    }
};
int main()
{
    PosPassThru p; void* v = (void*)1; GUID g = IID_IMediaSeeking; g.d2 = 0;
    if (p.NDQI(IID_IMediaSeeking, 0) != (int)0x80004003) { std::printf("00cad380_TEST FAIL\n"); return 1; }
    if (p.NDQI(IID_IMediaSeeking, &v) != 0 || v != (void*)(ISeek*)&p) { std::printf("00cad380_TEST FAIL\n"); return 1; }
    if (p.NDQI(IID_IMediaPosition, &v) != 0 || v != (void*)(IPos*)&p) { std::printf("00cad380_TEST FAIL\n"); return 1; }
    v = (void*)1; if (p.NDQI(g, &v) != (int)0x80004002 || v != 0) { std::printf("00cad380_TEST FAIL\n"); return 1; }
    std::printf("00cad380_TEST PASS\n");
    return 0;
}