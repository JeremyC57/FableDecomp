#include <cstdio>
static int g_deleted = 0;
struct CUnknown {
    const void* m_pUnknown; volatile long m_cRef;
    CUnknown() : m_pUnknown(0), m_cRef(0) { }
    virtual ~CUnknown() { ++g_deleted; }
    unsigned long NonDelegatingRelease()
    {
        long lRef = --m_cRef;
        if (lRef == 0) { m_cRef++; delete this; return 0; }
        unsigned long cRef = (unsigned long)m_cRef;
        return cRef > 1ul ? cRef : 1ul;
    }
};
int main()
{
    CUnknown* u = new CUnknown; u->m_cRef = 3;
    bool ok = u->NonDelegatingRelease() == 2 && u->NonDelegatingRelease() == 1 && g_deleted == 0 && u->NonDelegatingRelease() == 0 && g_deleted == 1;
    if (!ok) { std::printf("00cacd20_TEST FAIL\n"); return 1; }
    std::printf("00cacd20_TEST PASS\n");
    return 0;
}
