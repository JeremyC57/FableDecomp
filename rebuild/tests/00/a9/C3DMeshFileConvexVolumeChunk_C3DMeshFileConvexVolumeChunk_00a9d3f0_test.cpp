#include <cstdio>
#include <cstdlib>
#include <cstring>
// Behaviour test for 0x00a9d3f0: Planes block freed, then Name destroyed, then ~CChunkedFileSuperChunk.
static char g_log[16]; static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
class CCharString { public: void* m_pStringData; ~CCharString() { Log('S'); } };
struct CBlockArray { void* m_pData; long m_count; long m_capacity; ~CBlockArray() { if (m_pData) { Log('F'); std::free(m_pData); } } };
class CChunkedFileSuperChunk { public: virtual void v00() {} ~CChunkedFileSuperChunk() { Log('B'); } };
class C3DMeshFileConvexVolumeChunk : public CChunkedFileSuperChunk { public:
    CCharString Name;
    CBlockArray Planes;
    virtual void v00() {}
    ~C3DMeshFileConvexVolumeChunk();
};
C3DMeshFileConvexVolumeChunk::~C3DMeshFileConvexVolumeChunk()
{
}
int main()
{
    { C3DMeshFileConvexVolumeChunk c; c.Planes.m_pData = std::malloc(16); }
    if (std::strcmp(g_log, "FSB") != 0) { std::printf("00a9d3f0_TEST FAIL\n"); return 1; }
    g_n = 0; g_log[0] = 0;
    { C3DMeshFileConvexVolumeChunk c; c.Planes.m_pData = 0; }
    if (std::strcmp(g_log, "SB") != 0) { std::printf("00a9d3f0_TEST FAIL\n"); return 1; }
    std::printf("00a9d3f0_TEST PASS\n");
    return 0;
}