#include <stdio.h>
#include <stdlib.h>

// Ego_r and FableWin define the same values, including DUMMY_SHORE_POINT = 7.
enum EWaterType {
    WATER_TYPE_NULL = 0,
    WATER_TYPE_LAKE,
    WATER_TYPE_RIVER,
    WATER_TYPE_SEA,
    WATER_TYPE_REFLECTIVE_SEA,
    WATER_TYPE_NON_REFLECTIVE_SEA,
    WATER_TYPE_OLD,
    WATER_TYPE_DUMMY_SHORE_POINT,
    WATER_TYPE_ICE,
    WATER_TYPE_COUNT
};
typedef char ConfirmDummyShorePoint[WATER_TYPE_DUMMY_SHORE_POINT == 7 ? 1 : -1];

struct CIDrawMap;
struct CEngineWorldMap;
struct CDefinitionManager;
struct CEngineLandscapeRenderer;

struct Trace {
    const CIDrawMap* draw;
    const CEngineWorldMap* world;
    const CDefinitionManager* definition;
    const CEngineLandscapeRenderer* renderer;
    long world_x, world_y, draw_x, draw_y;
    int draw_calls, world_calls, definition_calls, theme_calls;
    long definitions[3];
    unsigned long theme_indices[6];
};

static Trace trace;
static const char* current_scene;
static int current_type[3], current_presence[3], current_world;

static void Require(bool condition, const char* description)
{
    if (!condition) {
        fprintf(stderr, "%s world=%d types=%d/%d/%d IDs=%d/%d/%d: %s\n", current_scene,
            current_world, current_type[0], current_type[1], current_type[2],
            current_presence[0], current_presence[1], current_presence[2], description);
        exit(1);
    }
}

struct CIDrawMap {
    long layer[3];
    long DrawGetEngineThemeAt(long x, long y, long layer_index) const
    {
        Require(this == trace.draw, "draw map receiver");
        Require(trace.definition_calls == 0 && layer_index == trace.draw_calls && layer_index < 3, "draw layer order");
        Require(x == trace.draw_x && y == trace.draw_y, "draw coordinates");
        ++trace.draw_calls;
        return layer[layer_index];
    }
};

struct CEngineMap {
    long Width, Height, WorldX, WorldY;
    CIDrawMap* GameMap;
    long PeekWidth() const { return Width; }
    long PeekHeight() const { return Height; }
    long PeekWorldX() const { return WorldX; }
    long PeekWorldY() const { return WorldY; }
    bool MapHasDataAtPoint(long x, long y) const
    {
        return x < Width && y < Height && x >= 0 && y >= 0 && GameMap;
    }
    long PeekThemeId(long xp, long yp, long level) const;
    EWaterType PeekWaterType(long, long) const;
};

struct CEngineWorldMap {
    long MapOffsetX, MapOffsetY, MapArrayWidth, MapArrayHeight;
    short Map[6];
    CEngineMap* MapList[3];
    CEngineMap* GetMap(long world_x, long world_y) const
    {
        Require(this == trace.world, "GPEngineWorldMap receiver");
        Require(trace.definition_calls == 0 && trace.world_calls < 3, "world lookup order");
        Require(world_x == trace.world_x && world_y == trace.world_y, "unadjusted world coordinates");
        ++trace.world_calls;
        world_x -= MapOffsetX;
        world_y -= MapOffsetY;
        if (world_x >= 0 && world_y >= 0) {
            if (world_x < MapArrayWidth * 32 && world_y < MapArrayHeight * 32) {
                const long index = Map[(world_x >> 5) + (world_y >> 5) * MapArrayWidth];
                return index > 0 ? MapList[index] : 0;
            }
        }
        return 0;
    }
};

struct CDefinitionRecord {
    long ClassIndex;
};

struct CDefinitionManager {
    const CDefinitionRecord* definitions[55];
    long GetDefClassIndexFromGlobalIndex(long global_id) const
    {
        Require(this == trace.definition, "GPDefinitionManager receiver");
        Require(trace.definition_calls < 3 && global_id == trace.definitions[trace.definition_calls], "definition lookup value or order");
        Require(global_id >= 0 && global_id < 55 && definitions[global_id], "invalid test definition ID");
        ++trace.definition_calls;
        return definitions[global_id]->ClassIndex;
    }
};

struct CLandscapeTheme {
    EWaterType WaterType;
};

struct CEngineLandscapeRenderer {
    CLandscapeTheme themes[4];
    const CLandscapeTheme& PeekTheme(unsigned long index) const
    {
        Require(this == trace.renderer, "GPLandscapeRenderer receiver");
        Require(trace.definition_calls == 3 && trace.theme_calls < 6 && index < 4, "theme lookup order or index");
        trace.theme_indices[trace.theme_calls++] = index;
        return themes[index];
    }
};

namespace NEngineInternal {
CEngineWorldMap* GPEngineWorldMap;
const CDefinitionManager* GPDefinitionManager;
CEngineLandscapeRenderer* GPLandscapeRenderer;
}

inline long CEngineMap::PeekThemeId(long xp,long yp,long level) const 
{
    if (!MapHasDataAtPoint(xp,yp)) {
        CEngineMap* map = NEngineInternal::GPEngineWorldMap->GetMap(
            PeekWorldX() + xp,
            PeekWorldY() + yp);
        if (map && map->MapHasDataAtPoint(PeekWorldX() + xp - map->PeekWorldX(), PeekWorldY() + yp - map->PeekWorldY()))
            return map->PeekThemeId(PeekWorldX() + xp - map->PeekWorldX(), PeekWorldY() + yp - map->PeekWorldY(), level);
        if (xp < 0)
            xp = 0;
        else if (xp > PeekWidth() - 1)
            xp = PeekWidth() - 1;
        if (yp < 0)
            yp = 0;
        else if (yp > PeekHeight() - 1)
            yp = PeekHeight() - 1;
    }
    if (GameMap)
        return GameMap->DrawGetEngineThemeAt(xp, yp, level);
    return 0;
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

static bool IsFinalWater(EWaterType water)
{
    return water != WATER_TYPE_NULL && water != WATER_TYPE_DUMMY_SHORE_POINT;
}

// Only a non-null second layer opens the third. Each real water type has priority over the later layers.
static EWaterType ExpectedWater(EWaterType first, bool second_present, EWaterType second,
                                bool third_present, EWaterType third)
{
    if (IsFinalWater(first) || !second_present || second == WATER_TYPE_NULL)
        return first;
    if (IsFinalWater(second))
        return second;
    if (third_present && third != WATER_TYPE_NULL)
        return third;
    return WATER_TYPE_DUMMY_SHORE_POINT;
}

static void CheckThemeCalls(const int* indices, const EWaterType* types)
{
    const bool second_eligible = indices[1] != 0 && types[1] != WATER_TYPE_NULL;
    unsigned long expected[6];
    int count = 0;
    expected[count++] = indices[0];
    if (types[0] != WATER_TYPE_NULL)
        expected[count++] = indices[0];
    if (indices[1])
        expected[count++] = indices[1];
    if (second_eligible && !IsFinalWater(types[0]))
        expected[count++] = indices[1];
    if (second_eligible && indices[2])
        expected[count++] = indices[2];
    if (second_eligible && indices[2] && types[2] != WATER_TYPE_NULL &&
        !IsFinalWater(types[0]) && !IsFinalWater(types[1]))
        expected[count++] = indices[2];
    Require(trace.theme_calls == count, "theme lookup count");
    for (int i = 0; i < count; ++i)
        Require(trace.theme_indices[i] == expected[i], "theme indices or lookup order");
}

enum Adjustment {
    Default, SouthMap, SoutheastMap, EmptyCell, NegativeCell, MissingMap,
    ShortNeighborWidth, ShortNeighborHeight, NegativeNeighborX, NegativeNeighborY,
    MissingNeighborData, MissingBaseData, SmallerBaseMap
};

enum Draw { NoDraw, BaseDraw, NeighborDraw };

struct Scene {
    const char* name;
    long x, y;
    Adjustment adjustment;
    bool world_lookup;
    Draw draw;
    long draw_x, draw_y;
};

static const Scene scenes[] = {
    { "inside",                 1,  2, Default,             false, BaseDraw,      1,  2 },
    { "local last cell",       31, 31, Default,             false, BaseDraw,     31, 31 },
    { "world left",            -1,  2, Default,              true, BaseDraw,      0,  2 },
    { "world top",              2, -1, Default,              true, BaseDraw,      2,  0 },
    { "world right boundary",  64,  2, Default,              true, BaseDraw,     31,  2 },
    { "world bottom boundary",  2, 96, Default,              true, BaseDraw,      2, 31 },
    { "east neighbor",         32,  2, Default,              true, NeighborDraw,  0,  2 },
    { "south neighbor",         1, 32, SouthMap,             true, NeighborDraw,  1,  0 },
    { "southeast neighbor",    33, 34, SoutheastMap,         true, NeighborDraw,  1,  2 },
    { "empty cell",            36,  2, EmptyCell,            true, BaseDraw,     31,  2 },
    { "negative cell",         36,  2, NegativeCell,         true, BaseDraw,     31,  2 },
    { "empty map pointer",     36,  2, MissingMap,           true, BaseDraw,     31,  2 },
    { "neighbor x at width",   36,  2, ShortNeighborWidth,   true, BaseDraw,     31,  2 },
    { "neighbor y at height",  36,  5, ShortNeighborHeight,  true, BaseDraw,     31,  5 },
    { "neighbor x negative",   36,  2, NegativeNeighborX,    true, BaseDraw,     31,  2 },
    { "neighbor y negative",   36,  2, NegativeNeighborY,    true, BaseDraw,     31,  2 },
    { "neighbor has no data",  36,  2, MissingNeighborData,  true, BaseDraw,     31,  2 },
    { "base has no data",       4,  2, MissingBaseData,      true, NoDraw,        0,  0 },
    { "base no data with east",36,  2, MissingBaseData,      true, NeighborDraw,  4,  2 },
    { "clamp to map dimensions",20,16, SmallerBaseMap,       true, BaseDraw,     19, 15 }
};

static void ConfigureScene(const Scene& scene, CEngineMap& base, CEngineMap& neighbor,
                           CEngineWorldMap& world, CIDrawMap* draws)
{
    base.Width = base.Height = neighbor.Width = neighbor.Height = 32;
    base.WorldX = world.MapOffsetX = 5;
    base.WorldY = world.MapOffsetY = 7;
    neighbor.WorldX = 37;
    neighbor.WorldY = 7;
    base.GameMap = &draws[0];
    neighbor.GameMap = &draws[1];
    world.MapArrayWidth = 2;
    world.MapArrayHeight = 3;
    for (int i = 0; i < 6; ++i)
        world.Map[i] = 0;
    world.Map[0] = 1;
    world.Map[1] = 2;
    world.MapList[0] = 0;
    world.MapList[1] = &base;
    world.MapList[2] = &neighbor;
    switch (scene.adjustment) {
    case Default: break;
    case SouthMap:
        world.Map[2] = 2;
        neighbor.WorldX = 5;
        neighbor.WorldY = 39;
        break;
    case SoutheastMap:
        world.Map[3] = 2;
        neighbor.WorldY = 39;
        break;
    case EmptyCell: world.Map[1] = 0; break;
    case NegativeCell: world.Map[1] = -1; break;
    case MissingMap: world.MapList[2] = 0; break;
    case ShortNeighborWidth: neighbor.Width = 4; break;
    case ShortNeighborHeight: neighbor.Height = 5; break;
    case NegativeNeighborX: neighbor.WorldX = 42; break;
    case NegativeNeighborY: neighbor.WorldY = 10; break;
    case MissingNeighborData: neighbor.GameMap = 0; break;
    case MissingBaseData: base.GameMap = 0; break;
    case SmallerBaseMap:
        base.Width = 20;
        base.Height = 16;
        break;
    }
}

int main()
{
    CEngineMap base, neighbor;
    CIDrawMap draws[2];
    CEngineWorldMap worlds[2];
    CDefinitionManager managers[2];
    CEngineLandscapeRenderer renderers[2];
    // IDs 44 and 54 are synthetic aliases of slot zero. The native lookup returns the table record's class index.
    const long raw_ids[2][4] = { { 11, 22, 33, 44 }, { 21, 32, 43, 54 } };
    const CDefinitionRecord records[2][3] = { { { 2 }, { 3 }, { 1 } }, { { 3 }, { 1 }, { 2 } } };
    const CDefinitionRecord missing = { 0 };
    for (int variant = 0; variant < 2; ++variant) {
        managers[variant].definitions[0] = &missing;
        managers[variant].definitions[raw_ids[variant][3]] = &missing;
        for (int i = 0; i < 3; ++i)
            managers[variant].definitions[raw_ids[variant][i]] = &records[variant][i];
    }

    unsigned long cases = 0;
    for (unsigned scene_index = 0; scene_index < sizeof(scenes) / sizeof(scenes[0]); ++scene_index) {
        const Scene& scene = scenes[scene_index];
        current_scene = scene.name;
        for (int variant = 0; variant < 2; ++variant) {
            current_world = variant;
            ConfigureScene(scene, base, neighbor, worlds[variant], draws);
            trace.world = &worlds[variant];
            trace.definition = &managers[variant];
            trace.renderer = &renderers[variant];
            NEngineInternal::GPEngineWorldMap = &worlds[variant];
            NEngineInternal::GPDefinitionManager = &managers[variant];
            NEngineInternal::GPLandscapeRenderer = &renderers[variant];
            trace.draw = scene.draw == BaseDraw ? &draws[0] : scene.draw == NeighborDraw ? &draws[1] : 0;
            trace.world_x = base.WorldX + scene.x;
            trace.world_y = base.WorldY + scene.y;
            trace.draw_x = scene.draw_x;
            trace.draw_y = scene.draw_y;
            for (int presence = 0; presence < 27; ++presence) {
                int indices[3];
                int selections = presence;
                for (int i = 0; i < 3; ++i) {
                    const int mode = selections % 3;
                    selections /= 3;
                    current_presence[i] = mode;
                    const long raw = mode == 0 ? 0 : mode == 1 ? raw_ids[variant][3] : raw_ids[variant][i];
                    draws[0].layer[i] = trace.draw == &draws[0] ? raw : -1;
                    draws[1].layer[i] = trace.draw == &draws[1] ? raw : -1;
                    trace.definitions[i] = trace.draw ? raw : 0;
                    indices[i] = trace.draw && mode == 2 ? records[variant][i].ClassIndex : 0;
                }
                renderers[variant].themes[0].WaterType = WATER_TYPE_NULL;
                for (int types = 0; types < WATER_TYPE_COUNT * WATER_TYPE_COUNT * WATER_TYPE_COUNT; ++types) {
                    int values = types;
                    EWaterType effective[3];
                    for (int i = 0; i < 3; ++i) {
                        current_type[i] = values % WATER_TYPE_COUNT;
                        values /= WATER_TYPE_COUNT;
                        const EWaterType value = static_cast<EWaterType>(current_type[i]);
                        renderers[variant].themes[records[variant][i].ClassIndex].WaterType = value;
                        effective[i] = indices[i] ? value : WATER_TYPE_NULL;
                    }
                    trace.draw_calls = trace.world_calls = trace.definition_calls = trace.theme_calls = 0;
                    const EWaterType expected = ExpectedWater(effective[0], indices[1] != 0, effective[1], indices[2] != 0, effective[2]);
                    Require(base.PeekWaterType(scene.x, scene.y) == expected, "water type result");
                    Require(trace.world_calls == (scene.world_lookup ? 3 : 0), "world lookup count");
                    Require(trace.definition_calls == 3, "definition lookup count");
                    Require(trace.draw_calls == (trace.draw ? 3 : 0), "draw lookup count");
                    CheckThemeCalls(indices, effective);
                    ++cases;
                }
            }
        }
    }
    Require(cases == 787320, "fixture case count");
    puts("00b40670_TEST PASS");
    return 0;
}