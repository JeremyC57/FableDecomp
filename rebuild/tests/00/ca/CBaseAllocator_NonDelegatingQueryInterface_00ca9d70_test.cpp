#include <cstdio>
#include <cstring>
// CBaseAllocator::NonDelegatingQueryInterface: IMemAllocator always, IMemAllocatorCallbackTemp only with release callbacks
// enabled -> GetInterface(the interface sub-object); else CUnknown's.
struct GUID { unsigned d1; unsigned short d2, d3; unsigned char d4[8]; };
static bool eq(const GUID& a, const GUID& b) { return !std::memcmp(&a, &b, sizeof(GUID)); }
static const GUID IID_IMemAllocator = { 0x56A8689C, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
static const GUID IID_IMemAllocatorCallbackTemp = { 0x379A0CF0, 0xC1DE, 0x11D2, { 0xAB, 0xF5, 0x00, 0xA0, 0xC9, 0x05, 0xF3, 0x75 } };
static int GetInterface(void* p, void** ppv) { *ppv = p; return 0; }
struct Unknown { int u; int NDQI(const GUID&, void** ppv) { *ppv = 0; return (int)0x80004002; } };
struct IAllocCb { int a; };
struct Allocator : Unknown, IAllocCb {
    int m_fEnableReleaseCallback;
    int NDQI(const GUID& riid, void** ppv)
    {
        if (eq(riid, IID_IMemAllocator) || (eq(riid, IID_IMemAllocatorCallbackTemp) && m_fEnableReleaseCallback))
            return GetInterface((IAllocCb*)this, ppv);
        else return Unknown::NDQI(riid, ppv);
    }
};
int main()
{
    Allocator a; a.m_fEnableReleaseCallback = 0; void* v = 0;
    if (a.NDQI(IID_IMemAllocator, &v) != 0 || v != (void*)(IAllocCb*)&a) { std::printf("00ca9d70_TEST FAIL\n"); return 1; }
    if (a.NDQI(IID_IMemAllocatorCallbackTemp, &v) != (int)0x80004002 || v != 0) { std::printf("00ca9d70_TEST FAIL\n"); return 1; }
    a.m_fEnableReleaseCallback = 1; if (a.NDQI(IID_IMemAllocatorCallbackTemp, &v) != 0 || v != (void*)(IAllocCb*)&a) { std::printf("00ca9d70_TEST FAIL\n"); return 1; }
    std::printf("00ca9d70_TEST PASS\n");
    return 0;
}