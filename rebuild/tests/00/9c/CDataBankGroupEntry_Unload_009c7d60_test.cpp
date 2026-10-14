#include <cstdio>
static int g_destroyed = 0;
struct CGroupEntry {
    virtual void v00() { }
    virtual void Destroy() { ++g_destroyed; }
    long m_refCount;
    void Release() { if (--m_refCount == 0) Destroy(); }
};
struct CIVCountedPointer_CGroupEntry {
    CGroupEntry* m_p;
    CIVCountedPointer_CGroupEntry() : m_p(0) { }
    ~CIVCountedPointer_CGroupEntry() { Release(); }
    void Release() { if (m_p) { m_p->Release(); m_p = 0; } }
    CIVCountedPointer_CGroupEntry& operator=(const CIVCountedPointer_CGroupEntry& o) { Release(); m_p = o.m_p; if (m_p) ++m_p->m_refCount; return *this; }
};
struct CDataBankGroupEntry { CIVCountedPointer_CGroupEntry PGroup; void Unload() { PGroup = CIVCountedPointer_CGroupEntry(); } };
int main()
{
    CGroupEntry shared; shared.m_refCount = 2;
    CDataBankGroupEntry a, b; a.PGroup.m_p = &shared; b.PGroup.m_p = &shared;
    a.Unload(); if (a.PGroup.m_p != 0 || shared.m_refCount != 1 || g_destroyed != 0) { std::printf("009c7d60_TEST FAIL\n"); return 1; }
    b.Unload(); if (b.PGroup.m_p != 0 || shared.m_refCount != 0 || g_destroyed != 1) { std::printf("009c7d60_TEST FAIL\n"); return 1; }
    b.Unload(); if (g_destroyed != 1) { std::printf("009c7d60_TEST FAIL\n"); return 1; }
    std::printf("009c7d60_TEST PASS\n"); return 0;
}