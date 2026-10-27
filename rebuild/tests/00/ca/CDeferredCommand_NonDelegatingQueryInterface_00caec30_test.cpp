#include <cstdio>
#include <cstring>
// CDeferredCommand::NonDelegatingQueryInterface: IID_IDeferredCommand -> the IDeferredCommand base, else CUnknown's.
struct GUID { unsigned d1; unsigned short d2, d3; unsigned char d4[8]; };
static bool eq(const GUID& a, const GUID& b) { return !std::memcmp(&a, &b, sizeof(GUID)); }
static const GUID IID_IDeferredCommand = { 0x56A868B8, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
static int GetInterface(void* p, void** ppv) { *ppv = p; return 0; }
struct Unknown { int u[3]; int NDQI(const GUID&, void** ppv) { *ppv = 0; return (int)0x80004002; } };
struct IDefCmd { int d; };
struct DeferredCommand : Unknown, IDefCmd {
    int NDQI(const GUID& r, void** ppv) { if (eq(r, IID_IDeferredCommand)) return GetInterface((IDefCmd*)this, ppv); else return Unknown::NDQI(r, ppv); }
};
int main()
{
    DeferredCommand c; void* v = 0; GUID g = IID_IDeferredCommand; g.d1 = 0x56A868B7;
    if (c.NDQI(IID_IDeferredCommand, &v) != 0 || v != (void*)(IDefCmd*)&c || v == (void*)&c) { std::printf("00caec30_TEST FAIL\n"); return 1; }
    if (c.NDQI(g, &v) != (int)0x80004002 || v != 0) { std::printf("00caec30_TEST FAIL\n"); return 1; }
    std::printf("00caec30_TEST PASS\n");
    return 0;
}