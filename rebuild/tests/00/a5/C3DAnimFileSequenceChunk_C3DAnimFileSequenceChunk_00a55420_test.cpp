#include <cstdio>
#include <cstdlib>
#include <cstring>
// Behaviour test for 0x00a55420: Matrices block freed, then BoneName destroyed, then ~CChunkedFileSuperChunk.
static char g_log[16]; static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
class CCharString { public: void* m_pStringData; ~CCharString() { Log('S'); } };
struct CBlockArray { void* m_pData; long m_count; long m_capacity; ~CBlockArray() { if (m_pData) { Log('F'); std::free(m_pData); } } };
class CChunkedFileSuperChunk { public: virtual void v00() {} ~CChunkedFileSuperChunk() { Log('B'); } };
class C3DAnimFileSequenceChunk : public CChunkedFileSuperChunk { public:
    CCharString BoneName;
    CBlockArray Matrices;
    virtual void v00() {}
    ~C3DAnimFileSequenceChunk();
};
C3DAnimFileSequenceChunk::~C3DAnimFileSequenceChunk()
{
}
int main()
{
    { C3DAnimFileSequenceChunk c; c.Matrices.m_pData = std::malloc(16); }
    if (std::strcmp(g_log, "FSB") != 0) { std::printf("00a55420_TEST FAIL\n"); return 1; }
    g_n = 0; g_log[0] = 0;
    { C3DAnimFileSequenceChunk c; c.Matrices.m_pData = 0; }
    if (std::strcmp(g_log, "SB") != 0) { std::printf("00a55420_TEST FAIL\n"); return 1; }
    std::printf("00a55420_TEST PASS\n");
    return 0;
}