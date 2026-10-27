// Behaviour test for 0x00CAA0A0 CBaseAllocator::Commit (standalone copy of the logic; Alloc and the lock stubbed)
#include <cstdio>
typedef int HRESULT;
static int g_locked;
struct Lock { Lock() { ++g_locked; } ~Lock() { --g_locked; } };
struct Allocator {
    int m_bCommitted, m_bDecommitInProgress, m_refs, m_allocs; HRESULT m_allocResult;
    HRESULT Alloc() { ++m_allocs; return m_allocResult; }
    void AddRef() { ++m_refs; }
    HRESULT Commit()
    {
        Lock l;
        if (m_bCommitted) return 0;
        m_bCommitted = 1;
        if (m_bDecommitInProgress) { m_bDecommitInProgress = 0; return 0; }
        HRESULT hr = Alloc();
        if (hr < 0) { m_bCommitted = 0; return hr; }
        AddRef();
        return 0;
    }
};
int main()
{
    Allocator a = { 0, 0, 1, 0, (HRESULT)0x8007000EL };
    if (a.Commit() != (HRESULT)0x8007000EL || a.m_bCommitted || a.m_refs != 1) { std::printf("00caa0a0_TEST FAIL\n"); return 1; }
    a.m_allocResult = 0;
    if (a.Commit() != 0 || !a.m_bCommitted || a.m_refs != 2 || a.m_allocs != 2) { std::printf("00caa0a0_TEST FAIL\n"); return 1; }
    if (a.Commit() != 0 || a.m_allocs != 2) { std::printf("00caa0a0_TEST FAIL\n"); return 1; }
    a.m_bCommitted = 0; a.m_bDecommitInProgress = 1;
    if (a.Commit() != 0 || a.m_bDecommitInProgress || !a.m_bCommitted || a.m_allocs != 2 || g_locked != 0) { std::printf("00caa0a0_TEST FAIL\n"); return 1; }
    std::printf("00caa0a0_TEST PASS\n");
    return 0;
}