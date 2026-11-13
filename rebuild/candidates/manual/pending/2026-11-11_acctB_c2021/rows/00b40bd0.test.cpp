// Behaviour test for 0x00B40BD0 CEngineMap::PeekWaterDepth(long x, long y) const: a host copy of the row's logic (the header
// inline PeekThemeId / PeekThemeBlend with the neighbouring-map lookup through the engine world map's 32x32-cell grid, the
// definition manager's global -> class index, the landscape renderer's theme water heights) checked on hand-made maps.
#include <cstdio>
#include <cmath>

struct DrawMap {                         // CIDrawMap slots 6 / 7: theme id and blend of (x, y, layer)
    long theme[4][4][3];
    unsigned char blend[4][4][3];
    virtual long DrawGetEngineThemeAt(long x, long y, long layer) const { return theme[y][x][layer]; }
    virtual unsigned char DrawGetEngineBlendAt(long x, long y, long layer) const { return blend[y][x][layer]; }
    virtual ~DrawMap() {}
};

struct EngineMap;
struct WorldMap {                         // CEngineWorldMap: Map grid, MapArrayWidth / Height, MapOffsetX / Y, MapList
    short grid[4];
    long MapArrayWidth, MapArrayHeight, MapOffsetX, MapOffsetY;
    EngineMap* list[3];
    EngineMap* GetMap(long x, long y) const
    {
        if (x < 0 || y < 0 || x >= MapArrayWidth * 32 || y >= MapArrayHeight * 32)
            return 0;
        long index = grid[(y >> 5) * MapArrayWidth + (x >> 5)];
        if (index <= 0)
            return 0;
        return list[index];
    }
};
static WorldMap g_world;

struct LandscapeTheme { float WaterHeight; };
static LandscapeTheme g_themes[8];        // CEngineLandscapeRenderer::LandscapeThemes, by def class index
static long ClassIndexFromGlobal(long g) { return g - 100; }   // CDefinitionManager::GetDefClassIndexFromGlobalIndex
static int g_calls;

struct EngineMap {
    const DrawMap* GameMap;
    unsigned short MapWidth, MapHeight, WorldPosX, WorldPosY;
    long PeekWidth() const { return MapWidth; }
    long PeekHeight() const { return MapHeight; }
    long PeekWorldX() const { return WorldPosX; }
    long PeekWorldY() const { return WorldPosY; }
    bool MapHasDataAtPoint(long x, long y) const { return x < PeekWidth() && y < PeekHeight() && x >= 0 && y >= 0 && GameMap != 0; }
    long PeekThemeId(long x, long y, long layer) const
    {
        if (x >= PeekWidth() || y >= PeekHeight() || x < 0 || y < 0 || !GameMap) {
            long worldX = PeekWorldX() - g_world.MapOffsetX + x;
            long worldY = PeekWorldY() - g_world.MapOffsetY + y;
            const EngineMap* map = g_world.GetMap(worldX, worldY);
            if (map && map->MapHasDataAtPoint(PeekWorldX() - map->PeekWorldX() + x, PeekWorldY() - map->PeekWorldY() + y))
                return map->PeekThemeId(PeekWorldX() - map->PeekWorldX() + x, PeekWorldY() - map->PeekWorldY() + y, layer);
            if (x < 0) x = 0; else if (x > PeekWidth() - 1) x = PeekWidth() - 1;
            if (y < 0) y = 0; else if (y > PeekHeight() - 1) y = PeekHeight() - 1;
        }
        if (GameMap)
            return GameMap->DrawGetEngineThemeAt(x, y, layer);
        return 0;
    }
    unsigned char PeekThemeBlend(long x, long y, long layer) const
    {
        if (x >= PeekWidth() || y >= PeekHeight() || x < 0 || y < 0 || !GameMap) {
            long worldX = PeekWorldX() - g_world.MapOffsetX + x;
            long worldY = PeekWorldY() - g_world.MapOffsetY + y;
            const EngineMap* map = g_world.GetMap(worldX, worldY);
            if (map && map->MapHasDataAtPoint(PeekWorldX() - map->PeekWorldX() + x, PeekWorldY() - map->PeekWorldY() + y))
                return map->PeekThemeBlend(PeekWorldX() - map->PeekWorldX() + x, PeekWorldY() - map->PeekWorldY() + y, layer);
            if (x < 0) x = 0; else if (x > PeekWidth() - 1) x = PeekWidth() - 1;
            if (y < 0) y = 0; else if (y > PeekHeight() - 1) y = PeekHeight() - 1;
        }
        if (GameMap)
            return GameMap->DrawGetEngineBlendAt(x, y, layer);
        return 0;
    }
    float PeekWaterDepth(long x, long y) const
    {
        long theme0 = PeekThemeId(x, y, 0);
        if (theme0 > 0) {
            ++g_calls;
            long index0 = ClassIndexFromGlobal(theme0);
            unsigned char blend0 = PeekThemeBlend(x, y, 0);
            float depth = (blend0 * g_themes[index0].WaterHeight) * (1.0f / 255.0f);
            long theme1 = PeekThemeId(x, y, 1);
            if (theme1 > 0) {
                ++g_calls;
                long index1 = ClassIndexFromGlobal(theme1);
                unsigned char blend1 = PeekThemeBlend(x, y, 1);
                depth += (blend1 * g_themes[index1].WaterHeight) * (1.0f / 255.0f);
                long theme2 = PeekThemeId(x, y, 2);
                if (theme2 > 0) {
                    ++g_calls;
                    long index2 = ClassIndexFromGlobal(theme2);
                    unsigned char blend2 = 255 - blend1 - blend0;
                    depth += (blend2 * g_themes[index2].WaterHeight) * (1.0f / 255.0f);
                }
            }
            return depth;
        }
        return 0.0f;
    }
};

static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    int fail = 0;
    g_themes[1].WaterHeight = 2.0f; g_themes[2].WaterHeight = 4.0f; g_themes[3].WaterHeight = 8.0f;
    g_themes[4].WaterHeight = 10.0f;
    static DrawMap a, b;
    for (int y = 0; y < 4; ++y)
        for (int x = 0; x < 4; ++x)
            for (int l = 0; l < 3; ++l) {
                a.theme[y][x][l] = 0; a.blend[y][x][l] = 0; b.theme[y][x][l] = 0; b.blend[y][x][l] = 0;
            }
    // map A cell (1, 1): three layers, blends 100 / 50 / (rest 105)
    a.theme[1][1][0] = 101; a.blend[1][1][0] = 100;
    a.theme[1][1][1] = 102; a.blend[1][1][1] = 50;
    a.theme[1][1][2] = 103; a.blend[1][1][2] = 7;      // the third blend is never read
    // map A cell (2, 1): one layer only
    a.theme[1][2][0] = 101; a.blend[1][2][0] = 255;
    // map A cell (3, 3): two layers, the third has theme 0
    a.theme[3][3][0] = 102; a.blend[3][3][0] = 51; a.theme[3][3][1] = 103; a.blend[3][3][1] = 204;
    // map B (world 4..7, 0..3) cell (0, 2): one layer of theme 104
    b.theme[2][0][0] = 104; b.blend[2][0][0] = 255;

    static EngineMap ma, mb;
    ma.GameMap = &a; ma.MapWidth = 4; ma.MapHeight = 4; ma.WorldPosX = 0; ma.WorldPosY = 0;
    mb.GameMap = &b; mb.MapWidth = 4; mb.MapHeight = 4; mb.WorldPosX = 4; mb.WorldPosY = 0;
    g_world.MapArrayWidth = 1; g_world.MapArrayHeight = 1; g_world.MapOffsetX = 0; g_world.MapOffsetY = 0;
    g_world.grid[0] = 0; g_world.list[0] = 0; g_world.list[1] = &ma; g_world.list[2] = &mb;

    float d = ma.PeekWaterDepth(1, 1);
    float want = 100 * 2.0f / 255.0f + 50 * 4.0f / 255.0f + 105 * 8.0f / 255.0f;
    if (!near(d, want) || g_calls != 3) fail = 1;
    g_calls = 0;
    if (!near(ma.PeekWaterDepth(2, 1), 2.0f) || g_calls != 1) fail = 2;
    if (!near(ma.PeekWaterDepth(3, 3), 51 * 4.0f / 255.0f + 204 * 8.0f / 255.0f)) fail = 3;
    if (ma.PeekWaterDepth(0, 0) != 0.0f) fail = 4;                     // no theme in layer 0
    // off the map, no neighbour on the grid (grid cell 0 = none): clamped to (3, 3)
    if (!near(ma.PeekWaterDepth(9, 7), 51 * 4.0f / 255.0f + 204 * 8.0f / 255.0f)) fail = 5;
    if (!near(ma.PeekWaterDepth(-5, -1), 0.0f)) fail = 6;              // clamped to (0, 0)
    // a world grid that maps world (4..31) to map B: (4, 2) of A is (0, 2) of B
    g_world.grid[0] = 2;
    if (!near(ma.PeekWaterDepth(4, 2), 10.0f)) fail = 7;
    // map without a game map: everything 0
    static EngineMap mc; mc.GameMap = 0; mc.MapWidth = 4; mc.MapHeight = 4; mc.WorldPosX = 100; mc.WorldPosY = 100;
    if (mc.PeekWaterDepth(1, 1) != 0.0f) fail = 8;
    if (fail) { std::printf("00b40bd0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00b40bd0_TEST PASS\n");
    return 0;
}
