// Behaviour test for 0x00B52DD0 CEngineWeatherRenderer::ClipBoxToLandscape (self-contained host model; plain C++98, no engine headers).
// Models: the world map's 32x32-cell index grid (CEngineWorldMap::GetMap(x, y)), the edge-sharing CEngineWorldMap::MapHasDataAtPoint
// ((x, y), (x - 1, y), (x, y - 1), (x - 1, y - 1)), CEngineWorldMap::PeekLandscapeHeight over CEngineMap::PeekLandscapeHeight (own cell,
// else the neighbouring map that holds the cell, else the clamped edge cell; heights quantised to 1/128), the GFFloatToLongFloor /
// GFFloatToLongCeil corner cells, and the four-corner test: VISIBLE (0) as soon as one corner has no map data or its landscape lies
// below the box top, CLIPPED (1) when all four corners are at or above it.
#include <cstdio>
#include <cmath>

struct GameMap {
    float Base, SlopeX, SlopeY;
    long Calls;
    GameMap(float base, float sx, float sy) : Base(base), SlopeX(sx), SlopeY(sy), Calls(0) {}
    virtual ~GameMap() {}
    virtual float DrawBlockGetSizeZAt(unsigned long x, unsigned long y, float scale)
    {
        ++Calls;
        return (Base + SlopeX * (float)x + SlopeY * (float)y) * scale;
    }
};

struct WorldMap;
static WorldMap* g_world = 0;

static long FloorToLong(float v) { return (long)std::floor((double)v); }   // GFFloatToLongFloor for non-tie inputs
static long CeilToLong(float v) { return (long)std::ceil((double)v); }     // GFFloatToLongCeil for non-tie inputs
static float Quantise(float h) { return (float)(long)std::floor((double)(h * 128.0f) + 0.5) * (1.0f / 128.0f); }

struct Map {
    long WorldX, WorldY, Width, Height;
    GameMap* Game;
    Map(long wx, long wy, long w, long h, GameMap* g) : WorldX(wx), WorldY(wy), Width(w), Height(h), Game(g) {}
    bool MapHasDataAtPoint(long x, long y) const
    {
        if (x < Width && y < Height && x >= 0 && y >= 0)
            return Game != 0;
        return false;
    }
    float PeekLandscapeHeight(long x, long y) const;
};

struct WorldMap {
    short* Grid;
    long ArrayWidth, ArrayHeight, OffsetX, OffsetY;
    Map** List;
    Map* GetMap(long x, long y) const
    {
        if (x < 0 || y < 0 || x >= ArrayWidth * 32 || y >= ArrayHeight * 32)
            return 0;
        long index = Grid[(y >> 5) * ArrayWidth + (x >> 5)];
        if (index <= 0)
            return 0;
        return List[index];
    }
    bool MapHasDataAtPoint(long x, long y) const
    {
        long mapX = x - OffsetX;
        long mapY = y - OffsetY;
        const Map* map = GetMap(mapX, mapY);
        if (map)
            return map->MapHasDataAtPoint(x - map->WorldX, y - map->WorldY);
        map = GetMap(mapX - 1, mapY);
        if (map)
            return map->MapHasDataAtPoint(x - map->WorldX, y - map->WorldY);
        map = GetMap(mapX, mapY - 1);
        if (map)
            return map->MapHasDataAtPoint(x - map->WorldX, y - map->WorldY);
        map = GetMap(mapX - 1, mapY - 1);
        if (map)
            return map->MapHasDataAtPoint(x - map->WorldX, y - map->WorldY);
        return false;
    }
    float PeekLandscapeHeight(long x, long y) const
    {
        long mapX = x - OffsetX;
        long mapY = y - OffsetY;
        const Map* map = GetMap(mapX, mapY);
        if (map)
            return map->PeekLandscapeHeight(x - map->WorldX, y - map->WorldY);
        return 0.0f;
    }
};

float Map::PeekLandscapeHeight(long x, long y) const
{
    if (x >= Width || y >= Height || x < 0 || y < 0 || !Game) {
        const Map* map = g_world->GetMap(WorldX - g_world->OffsetX + x, WorldY - g_world->OffsetY + y);
        if (map) {
            long mapY = WorldY - map->WorldY + y;
            long mapX = WorldX - map->WorldX + x;
            if (mapX < map->Width && mapY < map->Height && mapX >= 0 && mapY >= 0 && map->Game)
                return map->PeekLandscapeHeight(WorldX - map->WorldX + x, WorldY - map->WorldY + y);
        }
        if (x < 0)
            x = 0;
        else if (x > Width - 1)
            x = Width - 1;
        if (y < 0)
            y = 0;
        else if (y > Height - 1)
            y = Height - 1;
    }
    if (Game)
        return Quantise(Game->DrawBlockGetSizeZAt(x, y, 1.0f));
    return 0.0f;
}

struct Vec { float X, Y, Z; Vec(float x, float y, float z) : X(x), Y(y), Z(z) {} };

// 0 = VISIBLE, 1 = CLIPPED
static int ClipBoxToLandscape(const Vec& span, const Vec& pos)
{
    float halfX = span.X * 0.5f, halfY = span.Y * 0.5f, halfZ = span.Z * 0.5f;
    float top = pos.Z + halfZ;
    for (int corner = 0; corner < 4; ++corner) {
        long x = (corner & 2) ? CeilToLong(pos.X + halfX) : FloorToLong(pos.X - halfX);
        long y = (corner & 1) ? CeilToLong(pos.Y + halfY) : FloorToLong(pos.Y - halfY);
        if (!g_world->MapHasDataAtPoint(x, y) || g_world->PeekLandscapeHeight(x, y) < top)
            return 0;
    }
    return 1;
}

static int g_fail = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n", what); g_fail = 1; } }

int main()
{
    // Two 32x32-cell maps side by side (grid blocks 0 and 1), a third 16x32 map in block 2 (its right half has no cells), offset 0.
    GameMap gameA(10.0f, 0.25f, 0.0f), gameB(30.0f, 0.0f, 0.0f), gameC(5.0f, 0.0f, 0.0f);
    Map mapA(0, 0, 32, 32, &gameA), mapB(32, 0, 32, 32, &gameB), mapC(64, 0, 16, 32, &gameC);
    Map* list[4] = { 0, &mapA, &mapB, &mapC };
    short grid[3] = { 1, 2, 3 };
    WorldMap world = { grid, 3, 1, 0, 0, list };
    g_world = &world;

    // world-map lookups
    Check(world.GetMap(-1, 0) == 0 && world.GetMap(0, 32) == 0 && world.GetMap(96, 0) == 0, "GetMap off grid");
    Check(world.GetMap(31, 5) == &mapA && world.GetMap(32, 5) == &mapB && world.GetMap(70, 5) == &mapC, "GetMap blocks");
    Check(world.MapHasDataAtPoint(5, 5) && world.MapHasDataAtPoint(40, 31), "has data inside");
    Check(!world.MapHasDataAtPoint(-1, 5) && !world.MapHasDataAtPoint(5, -1), "no data off grid");
    Check(!world.MapHasDataAtPoint(0, 32), "cell y = 32 reaches map A through (x, y - 1) but lies outside its 32 rows");
    Check(world.MapHasDataAtPoint(32, 3), "cell x = 32 is map B's first column");
    Check(!world.MapHasDataAtPoint(85, 5), "map C right half has no cells, map C again through x - 1");
    Check(std::fabs(world.PeekLandscapeHeight(8, 3) - 12.0f) < 1e-4f, "height A");
    Check(std::fabs(world.PeekLandscapeHeight(40, 3) - 30.0f) < 1e-4f, "height B");
    Check(world.PeekLandscapeHeight(-5, 3) == 0.0f, "height off maps");
    // map C cell 20 is outside its 16 cells: the neighbour lookup finds map C again (not data), so the edge cell 15 is clamped
    Check(std::fabs(world.PeekLandscapeHeight(84, 3) - 5.0f) < 1e-4f, "clamped height C");
    Check(std::fabs(Quantise(1.0f / 3.0f) - 43.0f / 128.0f) < 1e-6f, "quantise");
    // the neighbour path: map A asked for its cell 33 (= world 33, map B's cell 1)
    Check(std::fabs(mapA.PeekLandscapeHeight(33, 2) - 30.0f) < 1e-4f, "neighbour map height");
    Check(std::fabs(mapA.PeekLandscapeHeight(-3, 2) - 10.0f) < 1e-4f, "clamped to edge");

    // boxes over map A: the landscape rises from 10 (x = 0) to 17.75 (x = 31)
    Check(ClipBoxToLandscape(Vec(4, 4, 2), Vec(10.5f, 10.5f, 5.0f)) == 1, "box well under the landscape is clipped");
    Check(ClipBoxToLandscape(Vec(4, 4, 2), Vec(10.5f, 10.5f, 20.0f)) == 0, "box above the landscape is visible");
    // top 12.2: the floor corners (x = 8, height 12.0) lie below the top, the ceil corners (x = 13, 13.25) above it
    Check(ClipBoxToLandscape(Vec(4, 4, 2), Vec(10.5f, 10.5f, 11.2f)) == 0, "one corner below the top is enough");
    // top 11.9: every corner is at or above it (x = 8 .. 13: 12.0 .. 13.25)
    Check(ClipBoxToLandscape(Vec(4, 4, 2), Vec(10.5f, 10.5f, 10.9f)) == 1, "all corners above the top");
    // a box hanging off the grid is visible (no map data at its floor corners)
    Check(ClipBoxToLandscape(Vec(4, 4, 2), Vec(0.5f, 10.5f, 0.0f)) == 0, "corner off the maps");
    // a box across the A / B seam: B (30) everywhere above, A at x = 30 (17.5) above a top of 14
    Check(ClipBoxToLandscape(Vec(4, 4, 2), Vec(31.5f, 10.5f, 13.0f)) == 1, "seam box under both maps");
    Check(ClipBoxToLandscape(Vec(4, 4, 2), Vec(31.5f, 10.5f, 18.0f)) == 0, "seam box above map A");
    Check(gameA.Calls > 0 && gameB.Calls > 0, "game maps queried");

    if (g_fail) { std::printf("00b52dd0_TEST FAIL\n"); return 1; }
    std::printf("00b52dd0_TEST PASS\n");
    return 0;
}
