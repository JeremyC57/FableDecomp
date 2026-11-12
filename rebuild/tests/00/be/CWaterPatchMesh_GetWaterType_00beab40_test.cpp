// Behaviour test for CWaterPatchMesh::GetWaterType (0x00BEAB40): a copy of the model with a table-driven map,
// definition manager and theme list; checks the scan range, the layer rules and the none / overridable priority.
#include <cstdio>
#include <cstring>

enum EWaterType { WATER_TYPE_NONE = 0, WATER_TYPE_OVERRIDABLE = 7 };

struct CLandscapeTheme { long pad[12]; EWaterType WaterType; float WaterHeight; };

static const long GRID = 40;
static long g_themeIds[3][GRID][GRID];      // [layer][x][y] global theme ids
static long g_peekCalls;

class CEngineMap { public:
    long PeekThemeId(long x, long y, long layer) const
    {
        ++g_peekCalls;
        if (x < 0 || y < 0 || x >= GRID || y >= GRID) return 0;
        return g_themeIds[layer][x][y];
    }
};
class CEngineLandscapeMap { public:
    CEngineMap* Map;
    const CEngineMap* PeekEngineMap() const { return Map; }
};
class CDefinitionManager { public:
    long GetDefClassIndexFromGlobalIndex(long globalIndex) const { return globalIndex >= 100 ? globalIndex - 100 : 0; }
};
class CEngineLandscapeRenderer { public:
    CLandscapeTheme Themes[8];
    const CLandscapeTheme& PeekTheme(unsigned long index) const { return Themes[index]; }
};
static CDefinitionManager       g_manager;
static CEngineLandscapeRenderer g_renderer;
static CDefinitionManager*       g_pEngineDefinitionManager = &g_manager;
static CEngineLandscapeRenderer* g_pLandscapeRenderer = &g_renderer;

static inline EWaterType CombineWaterTypes(EWaterType current, EWaterType themeType)
{
    return (current == WATER_TYPE_NONE || current == WATER_TYPE_OVERRIDABLE) ? themeType : current;
}

class CWaterPatchMesh { public:
    const CEngineLandscapeMap* Map;
    long OffsetX, OffsetY;
    const EWaterType GetWaterType() const
    {
        long startX = OffsetX;
        long endX = startX + 16;
        long startY = OffsetY;
        long endY = startY + 16;
        EWaterType waterType = WATER_TYPE_NONE;
        for (long x = startX; x <= endX; ++x) {
            for (long y = startY; y <= endY; ++y) {
                long theme0 = Map->PeekEngineMap()->PeekThemeId(x, y, 0);
                long theme1 = Map->PeekEngineMap()->PeekThemeId(x, y, 1);
                long theme2 = Map->PeekEngineMap()->PeekThemeId(x, y, 2);
                theme0 = g_pEngineDefinitionManager->GetDefClassIndexFromGlobalIndex(theme0);
                theme1 = g_pEngineDefinitionManager->GetDefClassIndexFromGlobalIndex(theme1);
                theme2 = g_pEngineDefinitionManager->GetDefClassIndexFromGlobalIndex(theme2);
                if (waterType == WATER_TYPE_NONE || waterType == WATER_TYPE_OVERRIDABLE) {
                    EWaterType vertexType = WATER_TYPE_NONE;
                    if (g_pLandscapeRenderer->PeekTheme(theme0).WaterType)
                        waterType = CombineWaterTypes(vertexType, g_pLandscapeRenderer->PeekTheme(theme0).WaterType);
                    if (theme1 && g_pLandscapeRenderer->PeekTheme(theme1).WaterType) {
                        waterType = CombineWaterTypes(waterType, g_pLandscapeRenderer->PeekTheme(theme1).WaterType);
                        if (theme2 && g_pLandscapeRenderer->PeekTheme(theme2).WaterType) {
                            waterType = CombineWaterTypes(waterType, g_pLandscapeRenderer->PeekTheme(theme2).WaterType);
                        }
                    }
                }
            }
        }
        return waterType;
    }
};

static void Reset()
{
    std::memset(g_themeIds, 0, sizeof(g_themeIds));
    g_peekCalls = 0;
}
// theme class index n is global id 100 + n; index 0 is "no theme"
static void Put(long layer, long x, long y, long themeIndex) { g_themeIds[layer][x][y] = 100 + themeIndex; }

int main()
{
    // theme water types: 0 none (index 0), 1 -> 3, 2 -> 7 (overridable), 3 -> 5, 4 -> 2, 5 -> none
    std::memset(&g_renderer, 0, sizeof(g_renderer));
    g_renderer.Themes[1].WaterType = (EWaterType)3;
    g_renderer.Themes[2].WaterType = WATER_TYPE_OVERRIDABLE;
    g_renderer.Themes[3].WaterType = (EWaterType)5;
    g_renderer.Themes[4].WaterType = (EWaterType)2;
    CEngineMap engineMap;
    CEngineLandscapeMap landscapeMap; landscapeMap.Map = &engineMap;
    CWaterPatchMesh mesh; mesh.Map = &landscapeMap; mesh.OffsetX = 4; mesh.OffsetY = 6;
    bool ok = true;

    Reset();                                               // 1: no water at all, 17 x 17 vertices x 3 layers scanned
    ok = ok && mesh.GetWaterType() == WATER_TYPE_NONE && g_peekCalls == 17 * 17 * 3;

    Reset(); Put(0, 10, 10, 1);                            // 2: layer 0 water
    ok = ok && mesh.GetWaterType() == 3;

    Reset(); Put(0, 4, 6, 2); Put(0, 20, 22, 4);           // 3: overridable first corner, real type at the last corner
    ok = ok && mesh.GetWaterType() == 2;

    Reset(); Put(0, 4, 6, 2);                              // 4: only the overridable type stays
    ok = ok && mesh.GetWaterType() == WATER_TYPE_OVERRIDABLE;

    Reset(); Put(0, 5, 6, 1); Put(0, 6, 6, 4);             // 5: the first real type wins, later vertices do not replace it
    ok = ok && mesh.GetWaterType() == 3;

    Reset(); Put(1, 8, 8, 3);                              // 6: layer 1 alone gives its type
    ok = ok && mesh.GetWaterType() == 5;

    Reset(); Put(2, 8, 8, 3);                              // 7: layer 2 is ignored when layer 1 has no theme
    ok = ok && mesh.GetWaterType() == WATER_TYPE_NONE;

    Reset(); Put(1, 8, 8, 5); Put(2, 8, 8, 3);             // 8: layer 2 is ignored when layer 1 has no water
    ok = ok && mesh.GetWaterType() == WATER_TYPE_NONE;

    Reset(); Put(1, 8, 8, 2); Put(2, 8, 8, 4);             // 9: layer 1 overridable, layer 2 replaces it
    ok = ok && mesh.GetWaterType() == 2;

    Reset(); Put(0, 8, 8, 2); Put(1, 8, 8, 3); Put(2, 8, 8, 4);   // 10: layer 0 overridable, layer 1 replaces, layer 2 does not
    ok = ok && mesh.GetWaterType() == 5;

    Reset(); Put(0, 8, 8, 1); Put(1, 8, 8, 3); Put(2, 8, 8, 4);   // 11: layer 0 real type is kept over layers 1 and 2
    ok = ok && mesh.GetWaterType() == 3;

    Reset(); Put(0, 21, 10, 1); Put(0, 10, 23, 1); Put(0, 3, 10, 1); Put(0, 10, 5, 1);   // 12: just outside the patch
    ok = ok && mesh.GetWaterType() == WATER_TYPE_NONE;

    Reset(); Put(0, 20, 22, 1);                            // 13: the far corner (offset + 16) is inside
    ok = ok && mesh.GetWaterType() == 3;

    Reset(); Put(0, 8, 8, 5); Put(1, 8, 8, 4);             // 14: layer 0 theme without water, layer 1 with water
    ok = ok && mesh.GetWaterType() == 2;

    if (!ok) { std::printf("00beab40_TEST FAIL\n"); return 1; }
    std::printf("00beab40_TEST PASS\n");
    return 0;
}