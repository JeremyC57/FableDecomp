#include <cstdio>
// Behaviour of CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip (0x004DDF6F): the intelligent pointer
// member is destroyed first, then the two array records free their blocks (B then A), then the base destructor.
static int g_log[8]; static int g_n = 0;
static void* g_freed[8]; static int g_freeCount = 0;
static void TestFree(void* p) { g_freed[g_freeCount++] = p; g_log[g_n++] = 2; }

struct CBaseIntelligentPointer {
    void* m_pItem;
    ~CBaseIntelligentPointer() { g_log[g_n++] = 1; }
};

struct CHeapArray {
    void* m_pData; long m_size; long m_capacity;
    ~CHeapArray() { if (m_pData != 0) TestFree(m_pData); }
};

struct CChunkedFileChunkBase {
    unsigned char m_baseData[0x0c];
    ~CChunkedFileChunkBase() { g_log[g_n++] = 3; }
};

class CPatchTesselationEdgeStrip : public CChunkedFileChunkBase { public:
    CHeapArray m_arrayA;
    CHeapArray m_arrayB;
    unsigned char m_pad24[8];
    CBaseIntelligentPointer m_pOwner;
    ~CPatchTesselationEdgeStrip() { }
};

int main()
{
    int a, b;
    {
        CPatchTesselationEdgeStrip s;
        s.m_arrayA.m_pData = &a; s.m_arrayB.m_pData = &b; s.m_pOwner.m_pItem = 0;
    }
    if (g_n != 4 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 2 || g_log[3] != 3 || g_freed[0] != &b || g_freed[1] != &a) { std::printf("004ddf6f_TEST FAIL\n"); return 1; }
    std::printf("004ddf6f_TEST PASS\n");
    return 0;
}