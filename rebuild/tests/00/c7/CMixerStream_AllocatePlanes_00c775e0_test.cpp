// Behaviour test for 00c775e0: CMixerStream::AllocatePlanes -- free the old block, allocate 3 * planeSize zeroed bytes, split into three planes
#include <cstdio>
#include <cstdlib>
#include <cstring>

static int g_allocs = 0, g_frees = 0, g_fail = 0;
static unsigned long g_lastSize = 0;
void* MixerAlloc(unsigned long size) { g_lastSize = size; if (g_fail) return 0; ++g_allocs; void* p = std::malloc(size); std::memset(p, 0xAB, size); return p; }
void MixerFree(void* block) { ++g_frees; std::free(block); }

struct CMixerStream {
    unsigned char _unk00[4];
    unsigned char* m_pPlaneA;
    unsigned char* m_pPlaneB;
    unsigned char* m_pPlaneC;
    unsigned long m_planeSize;
    bool AllocatePlanes();
};

bool CMixerStream::AllocatePlanes()
{
    if (m_pPlaneA != 0) {
        MixerFree(m_pPlaneA);
        m_pPlaneA = 0;
        m_pPlaneB = 0;
        m_pPlaneC = 0;
    }
    m_pPlaneA = (unsigned char*)MixerAlloc(m_planeSize * 3);
    if (m_pPlaneA == 0)
        return false;
    m_pPlaneB = m_pPlaneA + m_planeSize;
    m_pPlaneC = m_pPlaneA + m_planeSize * 2;
    std::memset(m_pPlaneA, 0, m_planeSize * 3);
    return true;
}

int main()
{
    CMixerStream s;
    std::memset(&s, 0, sizeof(s));
    s.m_planeSize = 7;
    int ok = s.AllocatePlanes() && g_allocs == 1 && g_frees == 0 && g_lastSize == 21
        && s.m_pPlaneB == s.m_pPlaneA + 7 && s.m_pPlaneC == s.m_pPlaneA + 14;
    for (int i = 0; ok && i < 21; i++) if (s.m_pPlaneA[i] != 0) ok = 0;
    s.m_planeSize = 5;
    ok = ok && s.AllocatePlanes() && g_frees == 1 && g_allocs == 2 && s.m_pPlaneC == s.m_pPlaneA + 10;
    g_fail = 1;
    ok = ok && !s.AllocatePlanes() && g_frees == 2 && s.m_pPlaneA == 0 && s.m_pPlaneB == 0 && s.m_pPlaneC == 0;
    if (!ok) { std::printf("00c775e0_TEST FAIL\n"); return 1; }
    std::printf("00c775e0_TEST PASS\n");
    return 0;
}