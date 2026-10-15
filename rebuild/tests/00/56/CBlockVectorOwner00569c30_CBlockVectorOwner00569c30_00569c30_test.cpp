// Behaviour test for 0x00569C30 ~CBlockVectorOwner: Clear() first, then the buffer at +0xc is freed
// when non-null, then the block vector's own destructor runs last.
#include <cstdio>

static int g_log[16];
static int g_n;
static void* g_freed;
static void Log(int v) { g_log[g_n++] = v; }
static void test_free(void* p) { Log(2); g_freed = p; }

struct CBlockVector {
    void* m_pFirst; void* m_pLast; void* m_pEnd;
    ~CBlockVector() { Log(3); }
};
struct CRawBuffer {
    void* m_pFirst; void* m_pLast; void* m_pEnd;
    ~CRawBuffer() { if (m_pFirst != 0) test_free(m_pFirst); }
};
struct CBlockVectorOwner {
    CBlockVector m_blocks;
    CRawBuffer   m_buffer;
    void Clear() { Log(1); }
    ~CBlockVectorOwner() { Clear(); }
};

int main()
{
    int block;
    g_n = 0;
    { CBlockVectorOwner o; o.m_buffer.m_pFirst = &block; }
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3 || g_freed != &block) { std::printf("00569c30_TEST FAIL\n"); return 1; }
    g_n = 0; g_freed = 0;
    { CBlockVectorOwner o; o.m_buffer.m_pFirst = 0; }
    if (g_n != 2 || g_log[0] != 1 || g_log[1] != 3 || g_freed != 0) { std::printf("00569c30_TEST FAIL\n"); return 1; }
    std::printf("00569c30_TEST PASS\n");
    return 0;
}