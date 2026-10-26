#include <cstdio>
class C3DVector { public: float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
class C2DCoordI { public: long x, y; C2DCoordI() {} C2DCoordI(long a, long b) : x(a), y(b) {} };
static C2DCoordI g_last; static int g_calls;
class CMap
{ public:
    long GetStrongestEngineThemeAt(const C2DCoordI& p, long* s) const { g_last = p; ++g_calls; if (s) *s = 7; return 42; }
};
class CWorldMap
{ public:
    long MapAt; bool Loaded; float OffX, OffY; CMap TheMap;
    long GetMapNumberAt(const C3DVector&) const { return MapAt; }
    bool IsMapLoaded(long) const { return Loaded; }
    const CMap& PeekMap(long) const { return TheMap; }
    C3DVector ConvertWorldMapCoordsToLocalCoords(long, const C3DVector& p) const { return C3DVector(p.X - OffX, p.Y - OffY, p.Z); }
    long GetStrongestEngineThemeAt(const C3DVector& pos, long* strength) const;
};
long CWorldMap::GetStrongestEngineThemeAt(const C3DVector& pos, long* strength) const
{
    long map = GetMapNumberAt(pos);
    if (map != 0 && IsMapLoaded(map))
    {
        C3DVector local = ConvertWorldMapCoordsToLocalCoords(map, pos);
        C2DCoordI block((long)local.X, (long)local.Y);
        return PeekMap(map).GetStrongestEngineThemeAt(block, strength);
    }
    return 0;
}
int main()
{
    CWorldMap w; w.MapAt = 2; w.Loaded = true; w.OffX = 100.0f; w.OffY = 200.0f; g_calls = 0;
    long s = 0;
    if (w.GetStrongestEngineThemeAt(C3DVector(150.9f, 260.2f, 0.0f), &s) != 42 || s != 7 || g_last.x != 50 || g_last.y != 60) { std::printf("0063adf0_TEST FAIL\n"); return 1; }
    w.MapAt = 0; g_calls = 0;
    if (w.GetStrongestEngineThemeAt(C3DVector(1, 1, 1), &s) != 0 || g_calls != 0) { std::printf("0063adf0_TEST FAIL\n"); return 1; }
    w.MapAt = 1; w.Loaded = false;
    if (w.GetStrongestEngineThemeAt(C3DVector(1, 1, 1), &s) != 0 || g_calls != 0) { std::printf("0063adf0_TEST FAIL\n"); return 1; }
    std::printf("0063adf0_TEST PASS\n");
    return 0;
}