#include <cstdio>
#include <cstdlib>
#include <new>
static bool g_fail = false; static void* g_ctorMap = 0; static long g_ctorIndex = -1;
struct CEngineMap;
struct CEngineLandscapeMap { unsigned char _body[0x60]; CEngineLandscapeMap(CEngineMap* map, long index) { g_ctorMap = map; g_ctorIndex = index; }
    static void* operator new(size_t n, const std::nothrow_t&) throw() { return g_fail ? 0 : std::malloc(n); } static void operator delete(void* p) { std::free(p); } static void operator delete(void* p, const std::nothrow_t&) throw() { std::free(p); } };
struct CEngineMap { CEngineLandscapeMap* m_pLandscapeMap; };
struct CRenderer { void CreateMap(CEngineMap* map, long index) { map->m_pLandscapeMap = new (std::nothrow) CEngineLandscapeMap(map, index); } };
int main()
{
    CRenderer r; CEngineMap m; m.m_pLandscapeMap = (CEngineLandscapeMap*)1;
    r.CreateMap(&m, 7); if (!m.m_pLandscapeMap || g_ctorMap != &m || g_ctorIndex != 7) { std::printf("00b67140_TEST FAIL\n"); return 1; }
    delete m.m_pLandscapeMap; g_fail = true; g_ctorMap = 0; r.CreateMap(&m, 8); if (m.m_pLandscapeMap != 0 || g_ctorMap != 0) { std::printf("00b67140_TEST FAIL\n"); return 1; }
    std::printf("00b67140_TEST PASS\n"); return 0;
}