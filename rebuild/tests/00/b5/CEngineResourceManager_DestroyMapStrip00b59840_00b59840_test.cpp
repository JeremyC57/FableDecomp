#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_dtor = 0;
struct CPatchTesselationEdgeStrip { ~CPatchTesselationEdgeStrip() { ++g_dtor; } };
struct SMapEntry_00b59840 { long m_id; CPatchTesselationEdgeStrip* m_pStrip; };
struct CEngineResourceManager { void DestroyMapStrip_00b59840(SMapEntry_00b59840& e) { if (e.m_pStrip) { delete e.m_pStrip; e.m_pStrip = 0; } } };
int main() {
    CEngineResourceManager m; SMapEntry_00b59840 e = { 1, new CPatchTesselationEdgeStrip }; m.DestroyMapStrip_00b59840(e); if (g_dtor != 1 || e.m_pStrip) { std::printf("00b59840_TEST FAIL\n"); return 1; }
    m.DestroyMapStrip_00b59840(e); if (g_dtor != 1) { std::printf("00b59840_TEST FAIL\n"); return 1; }
    std::printf("00b59840_TEST PASS\n"); return 0;
}