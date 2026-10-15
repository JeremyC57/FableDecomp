#include <cstdio>
static int g_decommits = 0, g_releases = 0; static int g_hr = 0;
struct IMemAllocatorLike {
    virtual unsigned long Release() { ++g_releases; return 0; }
    virtual int Decommit() { ++g_decommits; return g_hr; }
    virtual ~IMemAllocatorLike() {}
};
struct CBaseInputPin {
    IMemAllocatorLike* m_pAllocator;
    int BreakConnect()
    {
        if (m_pAllocator) {
            int hr = m_pAllocator->Decommit();
            if (hr < 0) return hr;
            m_pAllocator->Release();
            m_pAllocator = 0;
        }
        return 0;
    }
};
int main()
{
    IMemAllocatorLike alloc;
    CBaseInputPin pin; pin.m_pAllocator = &alloc;
    g_hr = (int)0x80004005L;
    if (pin.BreakConnect() != (int)0x80004005L || pin.m_pAllocator != &alloc || g_releases != 0) { std::printf("00ca8b50_TEST FAIL\n"); return 1; }
    g_hr = 1;
    if (pin.BreakConnect() != 0 || pin.m_pAllocator != 0 || g_releases != 1 || g_decommits != 2) { std::printf("00ca8b50_TEST FAIL\n"); return 1; }
    if (pin.BreakConnect() != 0 || g_decommits != 2) { std::printf("00ca8b50_TEST FAIL\n"); return 1; }
    std::printf("00ca8b50_TEST PASS\n");
    return 0;
}