// CEngineMap::PeekWaterType(long, long) const
// Leaf: ?PeekWaterType@CEngineMap@@QBE?AW4EWaterType@@JJ@Z
// Flags: /O2 /Oy /d2inlT403
// Inline-budget: 402 keeps the three top-level PeekThemeId calls out of line. 403 expands them and retains their recursive calls.
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CARRAY_RELEASE_INDEX_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#include "fable_string.h"
#define FABLE_RETAIL_CARRAY_DEBUG_MESSAGE_PASS29
#include "engine/CDebugManager.h"
#define FABLE_RETAIL_MAP_WATER_TYPE_PASS29
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
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER
#define FABLE_RETAIL_CENGINEMAP_GPENGINEWORLDMAP
#include "engine/EWaterType.h"
#include "engine/CEngineMap.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawMap.h"
#include "engine/CDefinitionManager.h"
#include "engine/CEngineLandscapeRenderer.h"

namespace NEngineInternal {
extern const CDefinitionManager* GPDefinitionManager;
}

EWaterType CEngineMap::PeekWaterType(const long mapx, const long mapy) const
{
    EWaterType water_type = WATER_TYPE_NULL;
    long theme0 = PeekThemeId(mapx, mapy, 0);
    long theme1 = PeekThemeId(mapx, mapy, 1);
    long theme2 = PeekThemeId(mapx, mapy, 2);
    theme0 = NEngineInternal::GPDefinitionManager->GetDefClassIndexFromGlobalIndex(theme0);
    theme1 = NEngineInternal::GPDefinitionManager->GetDefClassIndexFromGlobalIndex(theme1);
    theme2 = NEngineInternal::GPDefinitionManager->GetDefClassIndexFromGlobalIndex(theme2);
    if (NEngineInternal::GPLandscapeRenderer->PeekTheme(theme0).WaterType != WATER_TYPE_NULL)
        water_type = NEngineInternal::GPLandscapeRenderer->PeekTheme(theme0).WaterType;
    if (theme1 && NEngineInternal::GPLandscapeRenderer->PeekTheme(theme1).WaterType != WATER_TYPE_NULL) {
        if (water_type == WATER_TYPE_NULL || water_type == WATER_TYPE_DUMMY_SHORE_POINT)
            water_type = NEngineInternal::GPLandscapeRenderer->PeekTheme(theme1).WaterType;
        if (theme2 && NEngineInternal::GPLandscapeRenderer->PeekTheme(theme2).WaterType != WATER_TYPE_NULL)
            if (water_type == WATER_TYPE_NULL || water_type == WATER_TYPE_DUMMY_SHORE_POINT)
                water_type = NEngineInternal::GPLandscapeRenderer->PeekTheme(theme2).WaterType;
    }
    return water_type;
}