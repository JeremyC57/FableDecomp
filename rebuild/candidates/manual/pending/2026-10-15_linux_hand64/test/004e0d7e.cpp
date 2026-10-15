#include <cstdio>
// Behaviour of CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip (0x004E0D7E): the three 12-byte array
// records free their blocks last-to-first, then the CDefPointeeBase chain's root destructor runs.
static void* g_freed[8]; static int g_freeCount = 0; static int g_rootOrder = -1;
static void TestFree(void* p) { g_freed[g_freeCount++] = p; }

struct CHeapArray {
    void* m_pData; long m_size; long m_capacity;
    ~CHeapArray() { if (m_pData != 0) TestFree(m_pData); }
};

struct CBase_009fc550 {
    virtual void v00() { }
    ~CBase_009fc550() { g_rootOrder = g_freeCount; }
};

struct CDefPointeeBase : CBase_009fc550 {
    virtual void v00() { }
    ~CDefPointeeBase() { }
};

struct CPatchTesselationEdgeStrip : CDefPointeeBase {
    unsigned char m_pad04[0x24];
    CHeapArray m_arrayA;
    CHeapArray m_arrayB;
    CHeapArray m_arrayC;
    virtual void v00() { }
    ~CPatchTesselationEdgeStrip() { }
};

int main()
{
    int a, b, c;
    {
        CPatchTesselationEdgeStrip s;
        s.m_arrayA.m_pData = &a; s.m_arrayB.m_pData = &b; s.m_arrayC.m_pData = &c;
    }
    {
        CPatchTesselationEdgeStrip t;
        t.m_arrayA.m_pData = 0; t.m_arrayB.m_pData = 0; t.m_arrayC.m_pData = 0;
    }
    if (g_freeCount != 3 || g_freed[0] != &c || g_freed[1] != &b || g_freed[2] != &a || g_rootOrder != 3) { std::printf("004e0d7e_TEST FAIL\n"); return 1; }
    std::printf("004e0d7e_TEST PASS\n");
    return 0;
}
