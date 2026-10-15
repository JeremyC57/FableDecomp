#include <cstdio>
// Behaviour of CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip (0x00459B73): four owned blocks freed
// last-to-first (null ones skipped), then the base destructor.
static void* g_freed[8]; static int g_freeCount = 0; static int g_baseOrder = -1;
static void TestFree(void* p) { g_freed[g_freeCount++] = p; }

struct CPatchStripBase {
    unsigned char m_baseData[0x3c];
    ~CPatchStripBase() { g_baseOrder = g_freeCount; }
};

struct CPatchTesselationEdgeStrip : CPatchStripBase {
    void* m_pIndicesA; unsigned char m_recordA[8];
    void* m_pIndicesB; unsigned char m_recordB[8];
    void* m_pIndicesC; unsigned char m_recordC[8];
    void* m_pIndicesD; unsigned char m_recordD[8];
    ~CPatchTesselationEdgeStrip()
    {
        if (m_pIndicesD != 0) TestFree(m_pIndicesD);
        if (m_pIndicesC != 0) TestFree(m_pIndicesC);
        if (m_pIndicesB != 0) TestFree(m_pIndicesB);
        if (m_pIndicesA != 0) TestFree(m_pIndicesA);
    }
};

int main()
{
    int a, b, d;
    {
        CPatchTesselationEdgeStrip s;
        s.m_pIndicesA = &a; s.m_pIndicesB = &b; s.m_pIndicesC = 0; s.m_pIndicesD = &d;
    }
    if (g_freeCount != 3 || g_freed[0] != &d || g_freed[1] != &b || g_freed[2] != &a || g_baseOrder != 3) { std::printf("00459b73_TEST FAIL\n"); return 1; }
    std::printf("00459b73_TEST PASS\n");
    return 0;
}