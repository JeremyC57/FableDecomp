// Behaviour test for 0x00CA8AC0 CBaseInputPin::NotifyAllocator (standalone copy of the logic)
#include <cstdio>
typedef int HRESULT;
static int g_locked;
struct Lock { Lock() { ++g_locked; } ~Lock() { --g_locked; } };
struct Alloc { int refs; void AddRef() { ++refs; } void Release() { --refs; } };
struct Pin {
    Alloc* m_pAllocator; unsigned char m_bReadOnly;
    HRESULT NotifyAllocator(Alloc* pAllocator, int bReadOnly)
    {
        if (pAllocator == 0) return (HRESULT)0x80004003L;
        Lock cObjectLock;
        Alloc* pOld = m_pAllocator;
        pAllocator->AddRef();
        m_pAllocator = pAllocator;
        if (pOld != 0) pOld->Release();
        m_bReadOnly = (unsigned char)bReadOnly;
        return 0;
    }
};
int main()
{
    Alloc a = { 1 }, b = { 1 };
    Pin p = { 0, 0 };
    if (p.NotifyAllocator(0, 1) != (HRESULT)0x80004003L) { std::printf("00ca8ac0_TEST FAIL\n"); return 1; }
    if (p.NotifyAllocator(&a, 1) != 0 || a.refs != 2 || p.m_bReadOnly != 1) { std::printf("00ca8ac0_TEST FAIL\n"); return 1; }
    if (p.NotifyAllocator(&b, 0) != 0 || a.refs != 1 || b.refs != 2 || p.m_pAllocator != &b || p.m_bReadOnly != 0 || g_locked != 0) { std::printf("00ca8ac0_TEST FAIL\n"); return 1; }
    std::printf("00ca8ac0_TEST PASS\n");
    return 0;
}