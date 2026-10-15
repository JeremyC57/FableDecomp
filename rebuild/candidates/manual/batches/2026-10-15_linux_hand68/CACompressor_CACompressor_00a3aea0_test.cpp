// Behaviour test for CACompressor::CACompressor(long) (0x00A3AEA0): the owned workspace is resized to the requested
// size with zero bytes, Workspace points at it and WorkspaceSize records the size.
#include <cstdio>
#include <cstdlib>
static int g_baseCalls = 0;
struct CBaseClass { CBaseClass() { ++g_baseCalls; } virtual ~CBaseClass() { } };
struct CByteVector {
    unsigned char* m_pFirst; unsigned char* m_pLast; unsigned char* m_pEnd;
    CByteVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    void resize(unsigned int n, const unsigned char& v)
    {
        m_pFirst = (unsigned char*)std::malloc(n ? n : 1); for (unsigned int i = 0; i < n; ++i) m_pFirst[i] = v; m_pLast = m_pEnd = m_pFirst + n;
    }
};
struct CACompressor : CBaseClass {
    void* Workspace; unsigned long WorkspaceSize; bool CompressionOn; CByteVector OwnWorkspace;
    CACompressor(long workspaceSize)
    {
        OwnWorkspace.resize(workspaceSize, 0);
        Workspace = OwnWorkspace.m_pFirst;
        WorkspaceSize = workspaceSize;
    }
};
int main()
{
    CACompressor c(64);
    bool ok = g_baseCalls == 1 && c.Workspace == c.OwnWorkspace.m_pFirst && c.WorkspaceSize == 64 && c.OwnWorkspace.m_pLast - c.OwnWorkspace.m_pFirst == 64;
    for (int i = 0; ok && i < 64; ++i) ok = c.OwnWorkspace.m_pFirst[i] == 0;
    std::free(c.OwnWorkspace.m_pFirst);
    if (!ok) { std::printf("00a3aea0_TEST FAIL\n"); return 1; }
    std::printf("00a3aea0_TEST PASS\n");
    return 0;
}
