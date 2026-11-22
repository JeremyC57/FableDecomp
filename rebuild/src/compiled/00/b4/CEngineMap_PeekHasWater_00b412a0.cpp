// CEngineMap::PeekHasWater — water-type and depth test for up to three landscape theme layers.
// Leaf: ?PeekHasWater@CEngineMap@@QBE_NJJ@Z
// Flags: /d2inlT1267
// Inline-budget: expand all three retail PeekThemeId sites while retaining
// the original self-recursive COMDAT for translated coordinates in other maps.
#define FABLE_RETAIL_PASS22_WATER_PATCH_LOAD
#define FABLE_RETAIL_CENGINEMAP_PEEK
#define FABLE_RETAIL_CENGINEMAP_PEEKWATER
#define FABLE_RETAIL_CENGINEMAP_PEEK_BODIES
#define FABLE_RETAIL_CENGINEMAP_PEEKTHEMEID_ORIGINAL_PASS27
#define FABLE_RETAIL_CIDRAWMAP
#define FABLE_RETAIL_CIDRAWMAP_DRAW
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GETMAPXY
#define FABLE_RETAIL_CENGINEWORLDMAP_GETMAP_ORIGINAL_PASS26
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEFCLASSINDEX
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_PEEKTHEME
#define FABLE_RETAIL_CENGINEMAP_GPENGINEWORLDMAP
#include "engine/EWaterType.h"
#include "engine/CEngineMap.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawMap.h"
#include "engine/CDefinitionManager.h"
#include "engine/CEngineLandscapeRenderer.h"

namespace NEngineInternal {
extern const CDefinitionManager* GPDefinitionManager;
extern CEngineLandscapeRenderer* GPLandscapeRenderer;
}

bool CEngineMap::PeekHasWater(const long mapx, const long mapy) const
{
    long t0 = PeekThemeId(mapx, mapy, 0);
    if (t0 > 0) {
        t0 = NEngineInternal::GPDefinitionManager->GetDefClassIndexFromGlobalIndex(t0);
        bool has_water = NEngineInternal::GPLandscapeRenderer->PeekTheme(t0).WaterType != WATER_TYPE_NULL;
        long t1 = PeekThemeId(mapx, mapy, 1);
        if (t1 > 0) {
            t1 = NEngineInternal::GPDefinitionManager->GetDefClassIndexFromGlobalIndex(t1);
            has_water |= NEngineInternal::GPLandscapeRenderer->PeekTheme(t1).WaterType != WATER_TYPE_NULL;
            long t2 = PeekThemeId(mapx, mapy, 2);
            if (t2 > 0) {
                t2 = NEngineInternal::GPDefinitionManager->GetDefClassIndexFromGlobalIndex(t2);
                has_water |= NEngineInternal::GPLandscapeRenderer->PeekTheme(t2).WaterType != WATER_TYPE_NULL;
            }
        }
        return has_water && PeekWaterDepth(mapx, mapy) > 0.001f;
    }
    return false;
}