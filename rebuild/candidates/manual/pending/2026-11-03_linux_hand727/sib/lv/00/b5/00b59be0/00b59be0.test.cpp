// Behaviour test for CEngineLocalDetailGenerator::~CEngineLocalDetailGenerator (0x00B59BE0): for every engine map slot of the
// world map that holds a map, delete its local-detail cache map and clear the pointer; empty slots are skipped; then the
// ObjectCollectionTypes block is freed and ~CEngineComponent runs.
#include <cstdio>
#include <cstdlib>
static int g_cacheDeletes, g_freed, g_componentDtor;
struct LocalDetailCacheMapModel { ~LocalDetailCacheMapModel() { ++g_cacheDeletes; } };
struct EngineMapModel { void* LandscapeMap; LocalDetailCacheMapModel* LocalDetailCacheMap; };
struct WorldMapModel {
    EngineMapModel** Start; EngineMapModel** Finish;
    long GetMapCount() const { return (long)(Finish - Start); }
    EngineMapModel* GetMap(long i) const { return Start[i]; }
};
static WorldMapModel* g_pWorldMap;
struct ComponentModel { virtual ~ComponentModel() { ++g_componentDtor; } };
struct GeneratorModel : ComponentModel {
    void** TypesStart;
    ~GeneratorModel()
    {
        for (long i = 0; i < g_pWorldMap->GetMapCount(); ++i) {
            if (g_pWorldMap->GetMap(i)) {
                delete g_pWorldMap->GetMap(i)->LocalDetailCacheMap;
                g_pWorldMap->GetMap(i)->LocalDetailCacheMap = 0;
            }
        }
        if (TypesStart) { std::free(TypesStart); ++g_freed; }
    }
};
int main()
{
    EngineMapModel a = { 0, new LocalDetailCacheMapModel };
    EngineMapModel b = { 0, 0 };
    EngineMapModel c = { 0, new LocalDetailCacheMapModel };
    EngineMapModel* slots[4] = { &a, 0, &b, &c };
    WorldMapModel world = { slots, slots + 4 };
    g_pWorldMap = &world;
    ComponentModel* g = new GeneratorModel;
    ((GeneratorModel*)g)->TypesStart = (void**)std::malloc(16);
    delete g;
    if (g_cacheDeletes != 2 || a.LocalDetailCacheMap != 0 || b.LocalDetailCacheMap != 0 || c.LocalDetailCacheMap != 0
        || g_freed != 1 || g_componentDtor != 1) { std::printf("00b59be0_TEST FAIL\n"); return 1; }
    WorldMapModel empty = { slots, slots };
    g_pWorldMap = &empty;
    { GeneratorModel g2; g2.TypesStart = 0; }
    if (g_cacheDeletes != 2 || g_freed != 1 || g_componentDtor != 2) { std::printf("00b59be0_TEST FAIL\n"); return 1; }
    std::printf("00b59be0_TEST PASS\n");
    return 0;
}