#include <cstdio>
#include <stddef.h>
#include <new>
struct CEngineScene { int x; };
static int g_base = 0; static CEngineScene* g_scene = 0; static unsigned long g_levels = 0; static float g_minx = 0;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct C2DBoxF { float MinX, MinY, MaxX, MaxY; };
struct CCountedGrid { void* m_pObject; void* m_pCount; };
template<class T> struct CRetailArray { T* first; T* last; T* end_of_storage; CRetailArray() : first(0), last(0), end_of_storage(0) { } };
struct CEngineSceneMap : CBase {
    C2DBoxF Area; unsigned long MaxLevels; CRetailArray<CCountedGrid> GridLevels; CEngineScene* Scene; virtual void v00() { }
    CEngineSceneMap(CEngineScene* scene, unsigned long maxLevels, const C2DBoxF& area) : CBase(), GridLevels() { Initialise(scene, maxLevels, area); }
    void Initialise(CEngineScene* scene, unsigned long maxLevels, const C2DBoxF& area) { if (GridLevels.first || GridLevels.last || GridLevels.end_of_storage) return; Scene = scene; MaxLevels = maxLevels; Area = area; g_scene = scene; g_levels = maxLevels; g_minx = area.MinX; }
};
int main()
{
    CEngineScene scene; C2DBoxF box = { 1.0f, 2.0f, 3.0f, 4.0f };
    unsigned char raw[sizeof(CEngineSceneMap)]; for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xAA;
    CEngineSceneMap* m = new (raw) CEngineSceneMap(&scene, 5, box);
    if (g_base != 1 || g_scene != &scene || g_levels != 5 || g_minx != 1.0f || m->Scene != &scene || m->MaxLevels != 5 || m->Area.MaxY != 4.0f
        || offsetof(CEngineSceneMap, GridLevels) != 0x18 || offsetof(CEngineSceneMap, Scene) != 0x24 || sizeof(*m) != 0x28) { std::printf("00b94a40_TEST FAIL\n"); return 1; }
    std::printf("00b94a40_TEST PASS\n"); return 0;
}