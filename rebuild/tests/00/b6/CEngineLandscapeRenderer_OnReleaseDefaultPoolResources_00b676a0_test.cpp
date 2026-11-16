// Behaviour test for CEngineLandscapeRenderer::OnReleaseDefaultPoolResources @ 0x00B676A0 (NF46 round 7: rewritten on the shared headers the
// row now uses; the old test linked the candidates-header DAT_01436e8c / CEngineLandscapeMapList stand-ins the row no longer has).
// Links the row object: it supplies NEngineInternal::GPEngineWorldMap and CEngineLandscapeMap::ReleaseDefaultPoolSurfaces, builds a world
// map whose MapList holds {null, a map without a landscape map, a map with one} and expects exactly one release.
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_ONRELEASEDEFAULTPOOLRESOURCES
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEMAP
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CEngineMap.h"
#include <stdio.h>

class CEngineLandscapeMap
{
public:
    void ReleaseDefaultPoolSurfaces();
};

static int g_releaseCount = 0;
void CEngineLandscapeMap::ReleaseDefaultPoolSurfaces() { ++g_releaseCount; }

namespace NEngineInternal { CEngineWorldMap* GPEngineWorldMap = 0; }

struct MapListAccess : CArray<CEngineMap*> {
    static void Set(CArray<CEngineMap*>& list, CEngineMap** first, CEngineMap** last)
    {
        MapListAccess& access = static_cast<MapListAccess&>(list);
        access._M_start = first;
        access._M_finish = last;
    }
};

static double g_worldStorage[0x200 / sizeof(double)];
static double g_mapStorage[2][0x200 / sizeof(double)];

int main()
{
    CEngineWorldMap* world = reinterpret_cast<CEngineWorldMap*>(g_worldStorage);
    CEngineMap* withoutLandscape = reinterpret_cast<CEngineMap*>(g_mapStorage[0]);
    CEngineMap* withLandscape = reinterpret_cast<CEngineMap*>(g_mapStorage[1]);
    CEngineLandscapeMap landscape;
    withoutLandscape->SetLandscapeMap(0);
    withLandscape->SetLandscapeMap(&landscape);
    CEngineMap* maps[3] = { 0, withoutLandscape, withLandscape };
    MapListAccess::Set(world->MapList, maps, maps + 3);
    NEngineInternal::GPEngineWorldMap = world;
    CEngineLandscapeRenderer* renderer = reinterpret_cast<CEngineLandscapeRenderer*>(g_mapStorage[0]);
    renderer->CEngineLandscapeRenderer::OnReleaseDefaultPoolResources();
    MapListAccess::Set(world->MapList, 0, 0);
    if (g_releaseCount != 1) { printf("LANDSCAPE_RENDERER_RELEASE_POOL_TEST FAIL\n"); return 1; }
    printf("LANDSCAPE_RENDERER_RELEASE_POOL_TEST PASS\n");
    return 0;
}
