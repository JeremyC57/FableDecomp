// Behaviour test for 0x00CA89F0 CBaseInputPin::GetAllocator (standalone copy of the logic; CoCreateInstance stubbed)
#include <cstdio>
typedef int HRESULT;
static int g_locked;
struct Lock { Lock() { ++g_locked; } ~Lock() { --g_locked; } };
struct Alloc { int refs; void AddRef() { ++refs; } };
static Alloc g_alloc = { 1 };
static HRESULT g_createResult; static int g_creates;
static HRESULT CreateMemoryAllocator(Alloc** pp) { ++g_creates; if (g_createResult < 0) return g_createResult; *pp = &g_alloc; return 0; }
struct Pin {
    Alloc* m_pAllocator;
    HRESULT GetAllocator(Alloc** ppAllocator)
    {
        if (ppAllocator == 0) return (HRESULT)0x80004003L;
        Lock cObjectLock;
        if (m_pAllocator == 0) {
            HRESULT hr = CreateMemoryAllocator(&m_pAllocator);
            if (hr < 0) return hr;
        }
        *ppAllocator = m_pAllocator;
        m_pAllocator->AddRef();
        return 0;
    }
};
int main()
{
    Pin p = { 0 };
    Alloc* out = 0;
    if (p.GetAllocator(0) != (HRESULT)0x80004003L) { std::printf("00ca89f0_TEST FAIL\n"); return 1; }
    g_createResult = (HRESULT)0x80040154L;
    if (p.GetAllocator(&out) != (HRESULT)0x80040154L || out != 0) { std::printf("00ca89f0_TEST FAIL\n"); return 1; }
    g_createResult = 0;
    if (p.GetAllocator(&out) != 0 || out != &g_alloc || g_alloc.refs != 2) { std::printf("00ca89f0_TEST FAIL\n"); return 1; }
    if (p.GetAllocator(&out) != 0 || g_creates != 2 || g_alloc.refs != 3 || g_locked != 0) { std::printf("00ca89f0_TEST FAIL\n"); return 1; }
    std::printf("00ca89f0_TEST PASS\n");
    return 0;
}