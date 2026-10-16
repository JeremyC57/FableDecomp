#include <cstdio>
struct C3DVector
{
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct C2DCoordI
{
    long x, y;
    C2DCoordI() {}
    C2DCoordI(long ax, long ay) : x(ax), y(ay) {}
};
struct C2DBoxI { long TLX, TLY, BRX, BRY; };
struct CMapInfo { C2DBoxI Area; };

static C2DCoordI g_lastCoord;
static C3DVector g_lastVec;
static long      g_lastLong;
static int       g_calls;

struct CMap
{
    bool  BoolResult;
    float FloatResult;
    long  LongResult;
    bool  QueryBool(const C2DCoordI& p) const { g_lastCoord = p; ++g_calls; return BoolResult; }
    float QueryFloat(const C2DCoordI& p) const { g_lastCoord = p; ++g_calls; return FloatResult; }
    long  QueryLong(const C2DCoordI& p) const { g_lastCoord = p; ++g_calls; return LongResult; }
    float QueryFloat3(const C3DVector& p) const { g_lastVec = p; ++g_calls; return FloatResult; }
    C3DVector QueryVec3(const C3DVector& p) const { g_lastVec = p; ++g_calls; return C3DVector(1.0f, 2.0f, 3.0f); }
    long  QueryTheme(const C2DCoordI& p, long* s) const { g_lastCoord = p; ++g_calls; if (s) *s = 7; return LongResult; }
};

struct CWorldMap
{
    CMapInfo Maps[4];
    CMap     TheMap;
    long     MapAt;
    bool     Loaded;
    bool     InMap;
    long GetMapNumberAt(const C2DCoordI&) const { return MapAt; }
    long GetMapNumberAt(const C3DVector&) const { return MapAt; }
    bool IsMapLoaded(long) const { return Loaded; }
    bool IsPosInMap(const C2DCoordI&) const { return InMap; }
    bool IsPosInMap(const C3DVector&) const { return InMap; }
    const CMap& PeekMap(long) const { return TheMap; }
    C2DCoordI ConvertWorldMapCoordsToLocalCoords(long map, const C2DCoordI& pos) const
    {
        return C2DCoordI(pos.x - Maps[map].Area.TLX, pos.y - Maps[map].Area.TLY);
    }
    C3DVector ConvertWorldMapCoordsToLocalCoords(long map, const C3DVector& pos) const
    {
        return C3DVector(pos.X - (float)Maps[map].Area.TLX, pos.Y - (float)Maps[map].Area.TLY, pos.Z);
    }
    C3DVector GetGroundFaceNormalAt(const C3DVector& pos) const;
    void Setup(long m, bool loaded, bool inMap)
    {
        for (int i = 0; i < 4; ++i) { Maps[i].Area.TLX = i * 100; Maps[i].Area.TLY = i * 1000; Maps[i].Area.BRX = 0; Maps[i].Area.BRY = 0; }
        MapAt = m; Loaded = loaded; InMap = inMap;
        TheMap.BoolResult = true; TheMap.FloatResult = 5.5f; TheMap.LongResult = 42;
        g_calls = 0;
    }
};

C3DVector CWorldMap::GetGroundFaceNormalAt(const C3DVector& pos) const
{
    long map = GetMapNumberAt(pos);
    if (IsMapLoaded(map))
    {
        C3DVector local = ConvertWorldMapCoordsToLocalCoords(map, pos);
        const CMap& theMap = PeekMap(map);
        return theMap.QueryVec3(local);
    }
    return C3DVector(0.0f, 0.0f, 1.0f);
}

#define FAIL() do { std::printf("0063ab10_TEST FAIL\n"); return 1; } while (0)
int main()
{
    CWorldMap w;
    C3DVector p(250.5f, 3010.25f, 9.0f);
    w.Setup(2, true, true);
    C3DVector n = w.GetGroundFaceNormalAt(p);
    if (n.X != 1.0f || n.Y != 2.0f || n.Z != 3.0f || g_lastVec.X != 50.5f || g_lastVec.Y != 1010.25f) FAIL();
    w.Setup(2, false, true);
    n = w.GetGroundFaceNormalAt(p);
    if (n.X != 0.0f || n.Y != 0.0f || n.Z != 1.0f || g_calls != 0) FAIL();
    std::printf("0063ab10_TEST PASS\n");
    return 0;
}
