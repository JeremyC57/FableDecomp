#include <cstdio>

// Model of CEngineSurfaceCompositionManager::ProcessLoading(bool, double) @ 0x00B2AD90: the same loading tick is
// forwarded to both banks, first the one at +0x4fc, then the one at +0x808.
static int g_calls = 0; static int g_order[2];
struct CBank {
    int id; bool lastForce; double lastTime;
    void ProcessLoading(bool force, double time) { lastForce = force; lastTime = time; g_order[g_calls++] = id; }
};
class CEngineSurfaceCompositionManager { public:
    CBank m_bankAt4FC; CBank m_bankAt808;
    void ProcessLoading(bool force, double time) { m_bankAt4FC.ProcessLoading(force, time); m_bankAt808.ProcessLoading(force, time); }
};

int main()
{
    CEngineSurfaceCompositionManager m; m.m_bankAt4FC.id = 1; m.m_bankAt808.id = 2;
    m.ProcessLoading(true, 0.25);
    if (g_calls != 2 || g_order[0] != 1 || g_order[1] != 2 || !m.m_bankAt4FC.lastForce || !m.m_bankAt808.lastForce ||
        m.m_bankAt4FC.lastTime != 0.25 || m.m_bankAt808.lastTime != 0.25) { std::printf("00b2ad90_TEST FAIL\n"); return 1; }
    std::printf("00b2ad90_TEST PASS\n");
    return 0;
}