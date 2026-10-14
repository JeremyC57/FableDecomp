#include <cstdio>
#include <cstddef>
// Behaviour model of C3DMeshStripifier::C3DMeshStripifier @ 0x00A8DF70.
struct CBase { virtual void v00() {} CBase() {} };
struct SStripifierBuffers { void* m_pVertices; void* m_pIndices; void* m_pStrips; void* m_pAdjacency;
    SStripifierBuffers() : m_pVertices(0), m_pIndices(0), m_pStrips(0), m_pAdjacency(0) {} };
struct C3DMeshStripifier : CBase { SStripifierBuffers m_buffers; float m_cacheWeight; long m_maxStripLength; virtual void v00() {}
    C3DMeshStripifier() : CBase(), m_buffers(), m_cacheWeight(3.0f), m_maxStripLength(500) {} };
int main()
{
    C3DMeshStripifier s;
    bool ok = s.m_buffers.m_pVertices == 0 && s.m_buffers.m_pIndices == 0 && s.m_buffers.m_pStrips == 0 && s.m_buffers.m_pAdjacency == 0 && s.m_cacheWeight == 3.0f && s.m_maxStripLength == 500;
    if (sizeof(void*) == 4) ok = ok && offsetof(C3DMeshStripifier, m_cacheWeight) == 0x14 && offsetof(C3DMeshStripifier, m_maxStripLength) == 0x18;
    if (!ok) { std::printf("00a8df70_TEST FAIL\n"); return 1; }
    std::printf("00a8df70_TEST PASS\n");
    return 0;
}
