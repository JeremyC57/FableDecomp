// Behaviour test for 0x00B3F5E0 CEngineMap::PeekMapNormal(long x, long y) const: a host copy of the row's logic (the header inline
// PeekLandscapeHeight with the neighbouring-map lookup through the engine world map grid and the 1/128 quantisation, the two
// normalised 2D slope vectors, the combined normal) checked on hand-made height fields. The retail normalisations are the fast
// table-plus-Newton reciprocal square roots (C2DVector::Normalise 0x00A14540, GFFastOneOverSqrt); this model uses the exact
// 1/sqrt and compares with a tolerance.
#include <cstdio>
#include <cmath>

static long RoundNearestEven(float v)        // GFFloatToLongNear under the default x87 control word
{
    double f = std::floor((double)v);
    double d = (double)v - f;
    long r = (long)f;
    if (d > 0.5 || (d == 0.5 && (r & 1))) ++r;
    return r;
}

struct DrawMap {                             // CIDrawMap slot 5: block height of (x, y) times scale
    float h[4][4];
    virtual float DrawBlockGetSizeZAt(unsigned long x, unsigned long y, float scale) const { return h[y][x] * scale; }
    virtual ~DrawMap() {}
};

struct EngineMap;
struct WorldMap {
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

struct Vec2 { float X, Y; Vec2(float x, float y) : X(x), Y(y) {} void Normalise() { float r = 1.0f / std::sqrt(X * X + Y * Y); X *= r; Y *= r; } };
struct Vec3 { float X, Y, Z; Vec3(float x, float y, float z) : X(x), Y(y), Z(z) {} void Normalise() { float r = 1.0f / std::sqrt(X * X + Y * Y + Z * Z); X *= r; Y *= r; Z *= r; } };

struct EngineMap {
    const DrawMap* GameMap;
    unsigned short MapWidth, MapHeight, WorldPosX, WorldPosY;
    long PeekWidth() const { return MapWidth; }
    long PeekHeight() const { return MapHeight; }
    long PeekWorldX() const { return WorldPosX; }
    long PeekWorldY() const { return WorldPosY; }
    bool MapHasDataAtPoint(long x, long y) const { return x < PeekWidth() && y < PeekHeight() && x >= 0 && y >= 0 && GameMap != 0; }
    float QuantiseLandscapeHeight(float height) const { return (float)RoundNearestEven(height * 128.0f) * (1.0f / 128.0f); }
    float PeekLandscapeHeight(long x, long y) const
    {
        if (x >= PeekWidth() || y >= PeekHeight() || x < 0 || y < 0 || !GameMap) {
            long worldX = PeekWorldX() - g_world.MapOffsetX + x;
            long worldY = PeekWorldY() - g_world.MapOffsetY + y;
            const EngineMap* map = g_world.GetMap(worldX, worldY);
            if (map && map->MapHasDataAtPoint(PeekWorldX() - map->PeekWorldX() + x, PeekWorldY() - map->PeekWorldY() + y))
                return map->PeekLandscapeHeight(PeekWorldX() - map->PeekWorldX() + x, PeekWorldY() - map->PeekWorldY() + y);
            if (x < 0) x = 0; else if (x > PeekWidth() - 1) x = PeekWidth() - 1;
            if (y < 0) y = 0; else if (y > PeekHeight() - 1) y = PeekHeight() - 1;
        }
        if (GameMap)
            return QuantiseLandscapeHeight(GameMap->DrawBlockGetSizeZAt(x, y, 1.0f));
        return 0.0f;
    }
    Vec3 PeekMapNormal(long x, long y) const
    {
        float left = PeekLandscapeHeight(x - 1, y);
        float right = PeekLandscapeHeight(x + 1, y);
        float down = PeekLandscapeHeight(x, y - 1);
        float up = PeekLandscapeHeight(x, y + 1);
        Vec2 dx(left - right, 2.0f);
        Vec2 dy(down - up, 2.0f);
        dx.Normalise();
        dy.Normalise();
        Vec3 normal(dx.X, dy.X, dx.Y * dy.Y);
        normal.Normalise();
        return normal;
    }
};

static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
static bool check(const Vec3& n, float hx, float hy)     // the expected normal for slopes hx = left - right, hy = down - up
{
    float ax = hx / std::sqrt(hx * hx + 4.0f), ay = 2.0f / std::sqrt(hx * hx + 4.0f);
    float bx = hy / std::sqrt(hy * hy + 4.0f), by = 2.0f / std::sqrt(hy * hy + 4.0f);
    float z = ay * by, len = std::sqrt(ax * ax + bx * bx + z * z);
    return near(n.X, ax / len) && near(n.Y, bx / len) && near(n.Z, z / len)
        && near(n.X * n.X + n.Y * n.Y + n.Z * n.Z, 1.0f);
}

int main()
{
    int fail = 0;
    static DrawMap a, b;
    for (int y = 0; y < 4; ++y)
        for (int x = 0; x < 4; ++x) { a.h[y][x] = 0.0f; b.h[y][x] = 0.0f; }
    static EngineMap ma, mb;
    ma.GameMap = &a; ma.MapWidth = 4; ma.MapHeight = 4; ma.WorldPosX = 0; ma.WorldPosY = 0;
    mb.GameMap = &b; mb.MapWidth = 4; mb.MapHeight = 4; mb.WorldPosX = 4; mb.WorldPosY = 0;
    g_world.MapArrayWidth = 1; g_world.MapArrayHeight = 1; g_world.MapOffsetX = 0; g_world.MapOffsetY = 0;
    g_world.grid[0] = 0; g_world.list[0] = 0; g_world.list[1] = &ma; g_world.list[2] = &mb;

    // flat map: straight up
    Vec3 n = ma.PeekMapNormal(1, 1);
    if (!near(n.X, 0.0f) || !near(n.Y, 0.0f) || !near(n.Z, 1.0f)) fail = 1;
    // a ramp in x (h = x): left - right = -2 at an inner cell
    for (int y = 0; y < 4; ++y) for (int x = 0; x < 4; ++x) a.h[y][x] = (float)x;
    if (!check(ma.PeekMapNormal(1, 2), -2.0f, 0.0f)) fail = 2;
    // at the left edge the left neighbour clamps to column 0: left - right = 0 - 1
    if (!check(ma.PeekMapNormal(0, 2), -1.0f, 0.0f)) fail = 3;
    // a ramp in y (h = 0.5 y) with quantisation: heights snap to 1/128 (0.5 y is exact)
    for (int y = 0; y < 4; ++y) for (int x = 0; x < 4; ++x) a.h[y][x] = 0.5f * y + 0.001f;   // 0.001 rounds away
    if (!check(ma.PeekMapNormal(2, 1), 0.0f, -1.0f)) fail = 4;
    // the right neighbour of column 3 comes from map B through the world grid: B's column 0 holds 7
    for (int y = 0; y < 4; ++y) for (int x = 0; x < 4; ++x) { a.h[y][x] = 1.0f; b.h[y][x] = 7.0f; }
    g_world.grid[0] = 2;     // the 32x32 world cell (0, 0) points at map B
    if (!check(ma.PeekMapNormal(3, 1), 1.0f - 7.0f, 0.0f)) fail = 5;
    g_world.grid[0] = 0;     // no neighbour: column 3 clamps to itself
    if (!check(ma.PeekMapNormal(3, 1), 0.0f, 0.0f)) fail = 6;
    // a map without a game map reads 0 everywhere
    static EngineMap mc; mc.GameMap = 0; mc.MapWidth = 4; mc.MapHeight = 4; mc.WorldPosX = 200; mc.WorldPosY = 200;
    n = mc.PeekMapNormal(1, 1);
    if (!near(n.Z, 1.0f)) fail = 7;
    if (RoundNearestEven(2.5f) != 2 || RoundNearestEven(3.5f) != 4 || RoundNearestEven(-1.5f) != -2) fail = 8;
    if (fail) { std::printf("00b3f5e0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00b3f5e0_TEST PASS\n");
    return 0;
}
