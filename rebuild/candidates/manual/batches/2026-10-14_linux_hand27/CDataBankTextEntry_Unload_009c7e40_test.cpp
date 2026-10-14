#include <cstdio>
static int g_destroyed = 0;
struct CTextEntry {
    virtual void v00() { }
    virtual void Destroy() { ++g_destroyed; }
    long m_refCount;
    void Release() { if (--m_refCount == 0) Destroy(); }
};
struct CIVCountedPointer_CTextEntry {
    CTextEntry* m_p;
    CIVCountedPointer_CTextEntry() : m_p(0) { }
    ~CIVCountedPointer_CTextEntry() { Release(); }
    void Release() { if (m_p) { m_p->Release(); m_p = 0; } }
    CIVCountedPointer_CTextEntry& operator=(const CIVCountedPointer_CTextEntry& o) { Release(); m_p = o.m_p; if (m_p) ++m_p->m_refCount; return *this; }
};
struct CDataBankTextEntry { CIVCountedPointer_CTextEntry PText; void Unload() { PText = CIVCountedPointer_CTextEntry(); } };
int main()
{
    CTextEntry shared; shared.m_refCount = 2;
    CDataBankTextEntry a, b; a.PText.m_p = &shared; b.PText.m_p = &shared;
    a.Unload(); if (a.PText.m_p != 0 || shared.m_refCount != 1 || g_destroyed != 0) { std::printf("009c7e40_TEST FAIL\n"); return 1; }
    b.Unload(); if (b.PText.m_p != 0 || shared.m_refCount != 0 || g_destroyed != 1) { std::printf("009c7e40_TEST FAIL\n"); return 1; }
    b.Unload(); if (g_destroyed != 1) { std::printf("009c7e40_TEST FAIL\n"); return 1; }
    std::printf("009c7e40_TEST PASS\n"); return 0;
}
