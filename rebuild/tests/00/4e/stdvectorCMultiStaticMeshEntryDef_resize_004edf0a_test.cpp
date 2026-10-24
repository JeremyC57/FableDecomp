// behaviour test for 004edf0a: vector<CMultiStaticMeshEntryDef>::resize(n) builds ONE default CMultiStaticMeshEntryDef temporary,
// forwards to resize(n, value) and destroys the temporary afterwards (base destructor last).
#include <cstdio>
static int g_constructed = 0;
static int g_destroyed = 0;
static int g_resizeCalls = 0;
static int g_liveDuringResize = 0;
struct CResource {
    unsigned long m_refCount;
    CResource() : m_refCount(0) { }
    ~CResource() { ++g_destroyed; }
};
struct CDefPointeeBase : CResource { };
struct CSubDefClassBase : CDefPointeeBase {
    long m_defIndex;
    CSubDefClassBase() : m_defIndex(0) { ++g_constructed; }
};
struct CMultiStaticMeshEntryDef : CSubDefClassBase {
    CMultiStaticMeshEntryDef() { }
};
struct CVector {
    unsigned int Size;
    void resize(unsigned int n, const CMultiStaticMeshEntryDef& value)
    {
        ++g_resizeCalls;
        g_liveDuringResize = g_constructed - g_destroyed;
        if (value.m_defIndex == 0) Size = n;
    }
    void resize(unsigned int n)
    {
        resize(n, CMultiStaticMeshEntryDef());
    }
};
int main()
{
    CVector v;
    v.Size = 0;
    v.resize(5);
    int ok = 1;
    if (v.Size != 5 || g_resizeCalls != 1 || g_constructed != 1 || g_destroyed != 1 || g_liveDuringResize != 1) ok = 0;
    v.resize(0);
    if (v.Size != 0 || g_constructed != 2 || g_destroyed != 2) ok = 0;
    if (!ok) { std::printf("004edf0a_TEST FAIL\n"); return 1; }
    std::printf("004edf0a_TEST PASS\n");
    return 0;
}