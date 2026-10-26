#include <cstdio>
// Behaviour of CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip (0x004E424B): four 16-byte array records
// free their blocks last-to-first (null ones skipped), then the root destructor runs.
static void* g_freed[8]; static int g_freeCount = 0; static int g_rootOrder = -1;
static void TestFree(void* p) { g_freed[g_freeCount++] = p; }

struct CHeapArray16 {
    void* m_pData; long m_size; long m_capacity; long m_growBy;
    ~CHeapArray16() { if (m_pData != 0) TestFree(m_pData); }
};

struct CBase_009fc550 {
    virtual void v00() { }
    ~CBase_009fc550() { g_rootOrder = g_freeCount; }
};

class CDefPointeeBase : public CBase_009fc550 { public:
    virtual void v00() { }
    ~CDefPointeeBase() { }
};

class CPatchTesselationEdgeStrip : public CDefPointeeBase { public:
    unsigned char m_pad04[0x24];
    CHeapArray16 m_arrayA;
    CHeapArray16 m_arrayB;
    CHeapArray16 m_arrayC;
    CHeapArray16 m_arrayD;
    virtual void v00() { }
    ~CPatchTesselationEdgeStrip() { }
};

int main()
{
    int a, c, d;
    {
        CPatchTesselationEdgeStrip s;
        s.m_arrayA.m_pData = &a; s.m_arrayB.m_pData = 0; s.m_arrayC.m_pData = &c; s.m_arrayD.m_pData = &d;
    }
    if (g_freeCount != 3 || g_freed[0] != &d || g_freed[1] != &c || g_freed[2] != &a || g_rootOrder != 3) { std::printf("004e424b_TEST FAIL\n"); return 1; }
    std::printf("004e424b_TEST PASS\n");
    return 0;
}