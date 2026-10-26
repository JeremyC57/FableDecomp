// Behaviour test for 0x00BBA140 ~CEngineInternalPrimitiveParticleGroup: FreePrimitiveIDs, then the
// iterator array buffer, then the Primitives list (clear + node free), then the root base.
#include <cstdio>

static int g_log[16];
static int g_n;
static void* g_freed[4];
static int g_nf;
static void Log(int v) { g_log[g_n++] = v; }
static void test_free(void* p) { Log(9); g_freed[g_nf++] = p; }

struct CPrimitiveHandleList {
    void* m_pNode;
    void clear() { Log(3); }
    ~CPrimitiveHandleList() { clear(); void* node = m_pNode; if (node != 0) test_free(node); }
};
struct CIteratorArray {
    void* m_pFirst; void* m_pLast; void* m_pEnd;
    ~CIteratorArray() { if (m_pFirst != 0) test_free(m_pFirst); }
};
struct CIdList { void* m_pNode; ~CIdList() { Log(1); } };
class CIVCountedPointeeBase { public: ~CIVCountedPointeeBase() { Log(5); } };
struct CGroup : CIVCountedPointeeBase {
    CPrimitiveHandleList Primitives;
    CIteratorArray       PrimitiveIDToIterator;
    CIdList              FreePrimitiveIDs;
};

int main()
{
    int node, array;
    g_n = 0; g_nf = 0;
    { CGroup g; g.Primitives.m_pNode = &node; g.PrimitiveIDToIterator.m_pFirst = &array; g.FreePrimitiveIDs.m_pNode = 0; }
    const int want[] = { 1, 9, 3, 9, 5 };
    for (int i = 0; i < 5; ++i) if (g_n != 5 || g_log[i] != want[i]) { std::printf("00bba140_TEST FAIL\n"); return 1; }
    if (g_nf != 2 || g_freed[0] != &array || g_freed[1] != &node) { std::printf("00bba140_TEST FAIL\n"); return 1; }
    g_n = 0; g_nf = 0;
    { CGroup g; g.Primitives.m_pNode = 0; g.PrimitiveIDToIterator.m_pFirst = 0; g.FreePrimitiveIDs.m_pNode = 0; }
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 3 || g_log[2] != 5 || g_nf != 0) { std::printf("00bba140_TEST FAIL\n"); return 1; }
    std::printf("00bba140_TEST PASS\n");
    return 0;
}